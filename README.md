# GrowBox
 Shahmir Khan's custom GrowBox automation software suite.
 
 This is an arduino software designed to automate the control of and indoor growing environment. It provides a control webpage accessible over LAN/WLAN, timer functionality, and water drainage functionality.
 
 
 
 
 Hardware Requirements:
 
 This is just what I used, the code could easily be adapted to work with different setups
 
 
 -NodeMCU 1.0 ESP-12E (Or ESP8266)
 
 -Arduino Mega (Or any arduino that have enough output pins for your application)
 
 -16 Channel Relay module (Or any relay module, max is 16channels)
 
 -16x Wall outlets (Or however many you want, max is 16)
 
 
 
 
 
 
 
 
 
 Auto Drainage Functionality:
 
 "Drain Pump" = D5 pin on ESP8266, Stays on for 30s after being triggered (Will be moved to a pin on the MEGA in the future)
 
 "Runoff Pumps" = Pin 1 on MEGA board, Stays on only while the trigger is held
 
 The automatic drainage functionality is implemented by supporting up to 2 water sensors and associated pumps. These pumps will automatically turn on when their associated sensor is triggered. The trigger activates when the associated pin is brought LOW to ground.
 
 
 Any of the wall outlets and relays in the system can be used for this functionality. The user simply has to assign a name to one of the wall outlets on the web interface. Assigning the name "Drain Pump" (without quotations"") will give that wall outlet the "Drain Pump" functionality. Similarly, assigning the name "Runoff Pumps" (without quotations"") will give the "Runoff Pumps" functionality to that wall outlet.
 
 ![image](https://github.com/shahmirthesquid/GrowBox/assets/89569533/7ca1c471-964f-4b89-862a-e051ac5f4bfa)
 
 ![image](https://github.com/shahmirthesquid/GrowBox/assets/89569533/3bee6638-fb24-4a03-9377-4725c32bae62)
 
 ![image](https://github.com/shahmirthesquid/GrowBox/assets/89569533/8356e4de-630d-4699-be1d-80124291b400)



