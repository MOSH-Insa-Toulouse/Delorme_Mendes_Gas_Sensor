# KiCad Project

Damaris Mendes Ferreira and Margot Delorme.

### Introduction

In the MOSH (Microcontroller and Open Source Hardware) course we developed a gas sensor shield for an Arduino Uno board. The goal of this project is to construct a PCB that contains a gas sensor, LEDs, a buzzer and a LoRa module. This system should be capable to monitor the level of CO2 in a room and then send this information to a LoRaWAN using The Things Network site. The gas sensor is going to measure the level of CO2 and the Arduino is going to evaluate this level by taking a threshold as comparative. If the quantity of this gas crosses this threshold, a buzzer is activated and a red LED is fired. When the level of gas returns to its normal state, the buzzer is turned off and a green LED is turned on indicating a normal situation.

The followings sections are going to show all the results that we obtained in this project to make the final PCB layout.

### Assemblage and tests

The first step of this project was to assembly all the components on a protoboard to make sure that our system works. In order to validate our circuit and to make sure it works, we first assembled the electronic elements using a protoboard.

<p align="center"><img src="https://github.com/MOSH-Insa-Toulouse/Delorme_Mendes_Gas_Sensor/blob/master/Images/IMG_20181128_160956658.jpg" width="400"></p>

After that, we built the communication between our Lora module and The Things Network site. To do that, we added some configurations in the web site. We created our device in the web site, we configured the Device EUI to our Lora module and we configured the code on the Arduino using the Application EUI and the App Key according to the web site. The configurations were done as followed:

<p align="center"><img src="https://github.com/DamarisMenfer/KiCad/blob/master/Images/PCB 3D Up side.PNG" width="400"></p>

Once we made sure the LoRa communication was working, , we tested the circuit on the protoboard. We managed to get live information from the circuit to The Things Network site: 

<p align="center"><img src="https://github.com/MOSH-Insa-Toulouse/Delorme_Mendes_Gas_Sensor/tree/master/Images/Capture5.PNG" width="400"></p>

When the CO2 threshold is crossed, the buzzer is activate and the red led is fired. When the CO2 level goes back to normal, the buzzer is stop and the green led is fired. Our circuit was working properly:

<p align="center"><img src="https://github.com/DamarisMenfer/KiCad/blob/master/Images/PCB 3D Up side.PNG" width="400"></p>

The LoRa module send the information about the level of gas to the web site all the time. When the Level of gas is too high, LoRa sends and alert message.

<p align="center"><img src="https://github.com/DamarisMenfer/KiCad/blob/master/Images/PCB 3D Up side.PNG" width="400"></p>

The Arduino code is available in the “Arduino Code” folder. 

### Shield construction on KICAD

To design our PCB, we used the KICAD open-source software that was used to make the electric scheme and the routing for the PCB. The following sections are going to explain the results that we obtained.

#### Complete PCB

First, to create the PCB, we wanted to use as all the listed elements whether mandatory or optional: the leds, the resistances, the capacitors, the buzzer, the LoRa, the amplificator, and all the electronic elements. To do so, we used Kicad and created a first draft of the PCB. So, we drew the graphs and then selected the different items we wanted to use for each element.

The hardest part for us was to place the electronic elements on the PCB and routing it without the routes crossing and using as much as possible only one side of the PCB. For routing, on this first draft we routed on 2 different sides of the PCB.

Once we finished this first version of the PCB, we wanted to go further and have the routing on only one side of the PCB. We used the previous version of our PCB and rearranged it. In order to do this, we had to get rid of a few elements: one led and one button. We chose these 2 elements because the PCB still works without them.

So that you could have an overview of our work, we sent you both PCB. The folder called  “Complete Version” contains the PCB with all the components. The folder called “Final Version” is the latest version of the project. This version of the PCB has just one face of routing (the one abiding by the guidelines) and we are going to show the results of this version in the next sections.

#### Final PCB

##### Schema

First of all, we created the project based into an Arduino UNO template. After that, we started to construct our components. We created the LTC1050 library that is located in the “\MesLibrairies\LTC1050.lib” that is an operational amplifier that is going to be used to amplificate the signal of the gas sensor. After that, we constructed our second library, the RN2483 that is our LORA module. This library is located in the “\MesLibrairies\RN2483.lib”.

Finally, we connected all the elements into a schema as we can see in the following figure.

<p align="center"><img src="https://github.com/MOSH-Insa-Toulouse/Delorme_Mendes_Gas_Sensor/blob/master/Images/Schema.PNG"></p>

This schema is called “SchieldProject.sch”. We divided the schema into 5 different blocks. The first one is the circuit to amplificate the signal of the gas sensor. After that we have 2 minors blocks: the buzzer and the LED. The fourth block is composed by the Lora module and to finish we have the Arduino block.

The gas sensor block is composed by a non-inverting amplifier that is going to amplify the analog signal of the gas sensor that is going to be treated by the ADC of the Arduino. The output of the circuit was connected to the A0 pin. The signal is also filtered by this circuit in its entry and in its output to eliminate the noise.

The buzzer was connected into the pin 6 of the Arduino and the LED was connected in the pin 13. For the Lora module, we connected TX in the pin 10, Rx in the pin 11 and RST in the pin 12. This is a really important part of the circuit that is going to enable us to communicate with the TTN (The Things Network).

##### Layout

After doing the circuit schema, we created some footprints that we needed. The first one was the Lora module footprint that is available in the folder “\MesEmpreintes\Module_LoRa_RN2483.kicad_mod”. After that, we modified the footprint of the component TO-5-4 to match the specification of our gas sensor. This footprint is available in the folder “\MesEmpreintes\TO-5-4.kicad_mod”.

Then, we associated the components with their respective footprints and we made the routing of the PCB. The result is shown in the following figure.

<p align="center"><img src="https://github.com/MOSH-Insa-Toulouse/Delorme_Mendes_Gas_Sensor/blob/master/Images/Bottom View.PNG" width="400"></p>

3D top view of the PCB:

<p align="center"><img src="https://github.com/MOSH-Insa-Toulouse/Delorme_Mendes_Gas_Sensor/blob/master/Images/Top 3D View.PNG" width="400"></p>

3D bottom view of the PCB:

<p align="center"><img src="https://github.com/MOSH-Insa-Toulouse/Delorme_Mendes_Gas_Sensor/blob/master/Images/Bottom 3D View.PNG" width="400"></p>

### Conclusion

To conclude, we created our PCBs using Kicad, Arduino and The Things Network. We created 2 PCB, one with all the elements (mandatory and optional). Then we created our final version of the PCB thanks to this first draft but using only one side of the PCB. 

What we think could be improved in our PCB is the number of electronic components. We decided to create our final version with only one side of the PCB routed. In order to do so, we got rid of 2 elements.
