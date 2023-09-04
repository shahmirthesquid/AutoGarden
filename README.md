# GrowBox
 Shahmir Khan's custom GrowBox automation software suite.
 
 This is an arduino software designed to automate the control of and indoor growing environment. It provides a control webpage accessible over LAN/WLAN, timer functionality, and water drainage functionality.
 
 
 
 
 # Hardware Requirements:
 
 This is just what I used, the code could easily be adapted to work with different setups
 
 
 - NodeMCU 1.0 ESP-12E (Or ESP8266)
 
 - Arduino Mega (Or any arduino that have enough output pins for your application)
 
 - 16 Channel Relay module (Or any relay module, max is 16channels)
 
 - 16x Wall outlets (Or however many you want, max is 16)
 
 
 
 
 
 
 
 
 
 # Auto Drainage Functionality:
 
 "Drain Pump" = ~~D5 pin on ESP8266 board, Stays on for 30s after being triggered (Will be moved to a pin on the MEGA in the future)~~
                 Pin 2 on MEGA board
 
 "Runoff Pumps" = Pin 1 on MEGA board, Stays on only while the trigger is held
 
 The automatic drainage functionality is implemented by supporting up to 2 water sensors and associated pumps. These pumps will automatically turn on when their associated sensor is triggered. The trigger activates when the associated pin is brought LOW to ground.
 
 
 Any of the wall outlets and relays in the system can be used for this functionality. The user simply has to assign a name to one of the wall outlets on the web interface. Assigning the name "Drain Pump" (without quotations"") will give that wall outlet the "Drain Pump" functionality. Similarly, assigning the name "Runoff Pumps" (without quotations"") will give the "Runoff Pumps" functionality to that wall outlet.
 
 # Build Process
 ![20221013_111347](https://github.com/shahmirthesquid/GrowBox/assets/89569533/7dd46ae5-7ba0-4a94-8c7f-073120954661)
![20221013_111337](https://github.com/shahmirthesquid/GrowBox/assets/89569533/1ef41c8b-68bb-4716-9226-a3d1bf5b6319)
![20230115_161147](https://github.com/shahmirthesquid/GrowBox/assets/89569533/715ffc1a-0511-4d14-a58a-d1d07fe7fda1)
![20230115_161137](https://github.com/shahmirthesquid/GrowBox/assets/89569533/0497dc17-f84a-4e86-9b7b-a93549a255b6)
![20230115_163222](https://github.com/shahmirthesquid/GrowBox/assets/89569533/75c1bb1c-d867-4738-bf6b-5179d3e424ef)
![20230115_163214](https://github.com/shahmirthesquid/GrowBox/assets/89569533/91b3972d-fcdc-4a0f-a38a-e18dd0a42223)
![20221209_215727](https://github.com/shahmirthesquid/GrowBox/assets/89569533/ca805582-65f6-473b-acd2-e8c112c73e9b)
![20221209_215717](https://github.com/shahmirthesquid/GrowBox/assets/89569533/afb76175-89de-48cb-872f-4e2db24b5406)
![d057301b-20cf-48f6-ae15-6ec4465ed5aa](https://github.com/shahmirthesquid/GrowBox/assets/89569533/014d1398-287d-4d48-a35c-0906fad2a967)


 # Web Interface
 
 The web interface was kept simple to reduce memory size so that it could fit onto the NodeMCU ESP-12E microcontroller. 
 
 ![image](https://github.com/shahmirthesquid/GrowBox/assets/89569533/7ca1c471-964f-4b89-862a-e051ac5f4bfa)
 
 ![image](https://github.com/shahmirthesquid/GrowBox/assets/89569533/3bee6638-fb24-4a03-9377-4725c32bae62)
 
 ![image](https://github.com/shahmirthesquid/GrowBox/assets/89569533/8356e4de-630d-4699-be1d-80124291b400)



