/*
 * Author: Damaris M. Ferreira and Margot Delorme.
 *
 * This program is meant to be used with an Arduino UNO, conencted to an RNxx3 radio module.
 * This program monitor the level of CO2 in a room and send this information to a LoRaWAN using The Things Network site.
 * The gas sensor measure the level of CO2 and the Arduino evaluate the level by taking a threshold as comparative.
 * If the quantity crosses a threshold, a buzzer is activated and a red LED is fired.
 * When the level of gas returns to its normal state, the buzzer is turned off and a green LED is turned on indicating a normal situation.
 *
 * Connect the RN2xx3 as follows:
 * RN2xx3 -- Arduino
 * Uart TX -- 10
 * Uart RX -- 11
 * Reset -- 12
 * Vcc -- 3.3V
 * Gnd -- Gnd
 *
 */

/**********************************************************/
/******************* INCLUDES *****************************/
/**********************************************************/

// Important librarys
// Available to dowload on -> https://github.com/jpmeijers/RN2483-Arduino-Library 
#include <rn2xx3.h>
#include <SoftwareSerial.h>

// Define the ports to RX, TX
SoftwareSerial mySerial(10, 11); 

//  Pins to the LEDs
int redLed = 13;
int greenLed = 7;
// Pin to buzzer
int buzzer = 6;
// Pin to the gas sensor
int smokeA0 = A0;

// Threshold value to the alarm
int sensorThres = 40;

// Value readed by the sensor.
int analogSensor = 0;


// Create an instance of the rn2xx3 library,
// giving the software serial as port to use
rn2xx3 myLora(mySerial);



/**********************************************************/
/********************* SETUP ******************************/
/**********************************************************/
// The setup routine runs once when you press reset:
// Iniciating ports and LoRa.
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600); //serial port to computer
  mySerial.begin(9600); //serial port to radio
  Serial.println("Startup");

  initialize_radio();

  // Transmit a startup message
  myLora.tx("TTN Mapper on TTN Enschede node");

  // Initializing ports
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);

  // Configurate interruption for timmer.
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 31250;          // 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12);  // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // Active comparison mode

  interrupts(); // Active all interruptions

  delay(2000);
}

/**********************************************************/
/******************* FUNCTIONS ****************************/
/**********************************************************/
// Iniciating and configuring LoRa
void initialize_radio()
{
  // Reset rn2483
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);

  delay(100); // Wait for the RN2xx3's startup message
  mySerial.flush();

  // Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  myLora.autobaud();

  // Check communication with radio
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccesful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  // Print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  // Configure keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;

  //OTAA: initOTAA(String AppEUI, String AppKey);
  join_result = myLora.initOTAA("70B3D57ED0012F1A", "A23C96EE13804963F8C2BD6285448198");

  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");

}

// Timer to read the sensor level.
ISR(TIMER1_COMPA_vect){
    analogSensor = analogRead(smokeA0);

    // Checks if it has reached the threshold value.
    if (analogSensor > sensorThres)
    {
      // Gas level too high. Alert message
      led_on(redLed);
      led_off(greenLed);
      tone(buzzer, 1000, 200);
      Serial.println("ALARM ALARM");
    }
    else
    {
      // Gas level normal. No alert.
      led_on(greenLed);
      led_off(redLed);
      noTone(buzzer);
    }
}

// The loop routine to send message to LoRaWAN.
void loop()
{
    // Send message to LoRaWAN.
    Serial.print("TXing");
    Serial.print("Pin A0: ");
    Serial.println(analogSensor);

    String sensor = ""+analogSensor;

    // Transmit a one byte packet via TTN, using confirmed messages,
    // waiting for an acknowledgement or a downlink message.
    switch(myLora.tx(sensor)) //one byte, blocking function
    {
      case TX_FAIL:
      {
        Serial.println("TX unsuccessful or not acknowledged");
        break;
      }
      case TX_SUCCESS:
      {
        Serial.println("TX successful and acknowledged");
        break;
      }
      case TX_WITH_RX:
      {
        String received = myLora.getRx();
        received = myLora.base16decode(received);
        Serial.print("Received downlink: " + received);
        break;
      }
      default:
      {
        Serial.println("Unknown response from TX function");
      }
    }
    delay(10000);
}

// Turn on led.
void led_on(int led)
{
  digitalWrite(led, 1);
}

// Turn on off.
void led_off(int led)
{
  digitalWrite(led, 0);
}
