/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

//const int pin[16] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PC7,PC6,PC5,PC4,PC3,PC2,PC1,PC0};
#include <Wire.h>
#include <ArduinoJson.h>
unsigned long time;
// the setup function runs once when you press reset or power the board
void setup() {
  //Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  time = millis();
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  //pinMode(3, OUTPUT);
  //pinMode(PE0,OUTPUT);
  for(int i =22;i<=37;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }
}

// the loop function runs over and over again forever
void loop() {
  if(millis() > time+5000){
    for(int i =22;i<=37;i++){
      pinMode(i,OUTPUT);
      digitalWrite(i,HIGH);
    }
  }
 delay(100);
}

void processCall(String command){
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root= jsonBuffer.parseObject(command);
      
       if (root.success()) {
          time = millis();
          int gpio = atoi(root["gpio"]);
          Serial.println(gpio);
          int state = atoi(root["state"]);
          Serial.println(state);

          //set GPIO state  
          digitalWrite(gpio, state);
          //digitalWrite(PE0, state);
       }
}


// function that executes when data is received from master
void receiveEvent(int howMany) {
  String data="";
 while (0 <Wire.available()) {
    char c = Wire.read();     
    data += c;
    
  }
    Serial.println(data);           
    processCall(data);         
}
