//Shahmir Khan
//All rights reserved
//growbox-mega

#include <Wire.h> //i2c library
#include <ArduinoJson.h> //json parser library
unsigned long timeSinceLastMsg; //time variable to keep track of error state


void setup() {

  Serial.begin(115200); //start serial

  timeSinceLastMsg = millis();// set time to current time 

  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */ // do receive event when master tries to talk 
  Wire.onRequest(requestEvent); // do requestEvent when you want to update master about sensors

  //Setup Relay output pins
  for(int i =22;i<=37;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }

  //Setup Sensor Input pins
  for(int i = 1;i<=10;i++){
    pinMode(i,INPUT_PULLUP);
  }


}



// the loop function runs over and over again forever
void loop() {
  //If no msgs from master for more than 5 sec turn everything off
  if( (unsigned long)   (millis() - timeSinceLastMsg) > 5000){ // Does not get messed up when internal arduino time int resets to 0 after a month
    Serial.println("NO MESSAGES RECEIVED IN LAST 5 SEC!!!");
    Serial.println("TURNING OFF RELAYS FOR SAFETY!");
    for(int i =22;i<=37;i++){
      pinMode(i,OUTPUT);
      digitalWrite(i,HIGH);
    }
  }

  delay(100); //might not be needed. Test without and then remove (september 4 2023)

}

void processCall(String command){

  StaticJsonDocument<100> doc; //create local json doc, only needs 64 but i gave 100

  DeserializationError error = deserializeJson(doc,command); //store errors during deserialization

  if(error){ //check for errors, if error then print and return
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  //only gets here if there are no errors

  timeSinceLastMsg = millis(); //time since last msg from master is reset 

  int gpio = doc["gpio"];
  Serial.print("GPIO was parsed as: ");
  Serial.println(gpio);

  int state = doc["state"];
  Serial.print("State was parsed as: ");
  Serial.println(state);

  //set GPIO state  
  digitalWrite(gpio, state);
       
}


// function that executes when data is received from master
void receiveEvent(int howMany) {
  String data="";
  while (0 < Wire.available()) {
    char c = Wire.read();     
    data += c;
  }
  Serial.print("RECEIVED:  ");
  Serial.println(data);           
  processCall(data);         
}

void requestEvent(){

  
  int max = 10;
  String tmp;
  for(int i = 0;i<max;i++){
    if(digitalRead(i+1)==HIGH){
      tmp+=0;
    }
    else{
      tmp+=1;
    }
  }
  //tmp[max+1]='\0';

  //String temp;
  //temp = "hello11";
  Wire.write(tmp.c_str());

  /*

  int max = 2;

  String tmp;
  tmp+="{";
  for(int i = 1 ; i <= max; i ++){
    tmp+="\"gpio";
    tmp+=i;
    tmp+="\":";
    if(digitalRead(i)==LOW){
      tmp+="1";
      //Wire.write(tmp.c_str());   
      //Wire.endTransmission();
    }
    else if(digitalRead(i)==HIGH){
      tmp+="0";
    }

    if(i<max){
      tmp+=",";
    }
    else{
      tmp+="}";
    }
      //String tmp;
      //tmp+="{\"gpio\":";
      //tmp+=i;
      //tmp+=",\"state\":0}";
      //Wire.endTransmission(); 
    

  }
  Serial.println("SendingMsg: ");
  Serial.println(tmp.c_str());
  //Wire.write(tmp.c_str());
  //Wire.write("hello");
  Wire.write(tmp.c_str() );
  //Wire.endTransmission();
  */


}
