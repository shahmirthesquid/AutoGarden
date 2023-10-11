#include "growboxFunctions.h"


/*
extern AsyncWebServer server;
extern NTPClient timeClient;
extern const int output;
extern int relay1;
extern int relay2;
extern String formattedTime;
extern String Date;
extern int Day;
extern int Month;
extern int Year;
extern int hour;
extern int minute;
extern int second;
extern const char index_html[];

extern const char* PARAM_INPUT_1;
extern const char* PARAM_STRING ;
extern const char* PARAM_INT ;
extern const char* PARAM_FLOAT ;
*/
void serverSetup(){

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    /*
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime(); 
    String formattedTime = timeClient.getFormattedTime();
    
    struct tm *ptm = gmtime ((time_t *)&epochTime); 

    hour = ptm->tm_hour;
    minute = ptm->tm_min;
    second =ptm->tm_sec;
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;
  
    formattedTime = timeClient.getFormattedTime(); 
    Date = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    */
    request->send_P(200, "text/html", index_html, processor);
  });
  
  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    String inputMessage;

    for(int i = 0;i<16;i++){
      //delay(5);
      char name[16];
      char startTime[16];
      char timeLen[16];
      char relayFile[16];

      sprintf(name,"name%d",i+1);
      sprintf(startTime,"start%d",i+1);
      sprintf(timeLen,"len%d",i+1);
      sprintf(relayFile,"relay%d",i+1);

      //String name = "name"+String(i+1);
      //String startTime = "start"+String(i+1);
      //String timeLen = "len"+String(i+1);
      //String relayFile = "relay"+String(i+1);

      if (request->hasParam(name)) {
        inputMessage = request->getParam(name)->value();
        Serial.println(inputMessage);

        char fileName[16];
        sprintf(fileName,"/%s.txt",name);

        writeFile(LittleFS, fileName, inputMessage.c_str());
        names[i]=inputMessage;
      }
      // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
      else if (request->hasParam(startTime)) {
        //Serial.println("GOT HERE!!!!!!!!");
        inputMessage = request->getParam(startTime)->value();

        char fileName[16];
        sprintf(fileName,"/%s.txt",startTime);
        writeFile(LittleFS, fileName, inputMessage.c_str());

        startTimes[i]=inputMessage;


        relay[i]=HIGH; // turn off relay
        
        sprintf(fileName,"/%s.txt",relayFile);
        writeFile(LittleFS, fileName, "1"); 
      }
      // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
      else if (request->hasParam(timeLen)) {
        inputMessage = request->getParam(timeLen)->value();

        char fileName[16];
        sprintf(fileName,"/%s.txt",timeLen);
        writeFile(LittleFS, fileName, inputMessage.c_str());

        timeLengths[i]=inputMessage.toFloat();

        relay[i]=HIGH; // turn off relay
        
        sprintf(fileName,"/%s.txt",relayFile);
        writeFile(LittleFS, fileName, "1"); 
      }
      else {
        inputMessage = "No message sent";
      }

    }

    Serial.println(inputMessage);
    request->send(200, "text/text", "OK");
  });
  
    // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {

    int debug =0;
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {//relay state update
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;

      if(debug){
        Serial.println("/update inputMessage:");
        Serial.print(inputMessage);
        Serial.println();
      }

      char strings[4][10] = {0};
      byte index = 0;  // index to index the strings array
      char* token = strtok((char*)inputMessage.c_str(), "_");  // get the first part
      while (token != NULL )
      {
        strcpy(strings[index], token);  //safer: use strncpy() instead
        Serial.println( strings[index] ); //print each substring
        index++;                 // increment the array position
        token = strtok(NULL, "_");  // get the next part
      }

   
      updateRelay(strings[0],strings[1]);

      //ledState = !ledState;
    }
    else if (request->hasParam(PARAM_INPUT_2)) {//timer state update
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;

      if(debug){
        Serial.println("/update inputMessage:");
        Serial.print(inputMessage);
        Serial.println();
      }

        char strings[4][10] = {0};
        byte index = 0;  // index to index the strings array
        char* token = strtok((char*)inputMessage.c_str(), "_");  // get the first part
        while (token != NULL )
        {
          strcpy(strings[index], token);  //safer: use strncpy() instead
          Serial.println( strings[index] ); //print each substring
          index++;                 // increment the array position
          token = strtok(NULL, "_");  // get the next part
        }

   
      updateTimer(strings[0],strings[1]);

      //ledState = !ledState;
    }
    

    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    //Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;

      for(int i = 0; i <16;i++){
        char tmp[16];
        sprintf(tmp,"%d",i+1);
        if(strcmp(inputMessage.c_str(),tmp)==0) {
          sprintf(tmp,"%d",!relay[i]);
          request->send(200, "text/plain",tmp);
        }        
      }
    }
  });

    // Send a GET request to <ESP_IP>/state
  server.on("/timer", HTTP_GET, [] (AsyncWebServerRequest *request) {//get state of timer checks c1,c2,c3,etc
    
    int debug =0;

    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if(debug){
        Serial.print("inputMessage:");
        Serial.println(inputMessage);

        Serial.print("inputParam:");
        Serial.println(inputParam);


      }
      for(int i = 0; i <16;i++){
        char tmp[16];
        sprintf(tmp,"%d",i+1);
        if(strcmp(inputMessage.c_str(),tmp)==0) {
          sprintf(tmp,"%d",timer[i]);
          request->send(200, "text/plain", tmp );

          if(debug){
            Serial.printf("!inputMessage = %d!\nactual state: %d\n",i+1,timer[i]);
            Serial.println("sent state: ");
            Serial.println(tmp );
          }
          //Serial.println(timer[i]);
        }        
      }
    }
  });
  
  server.on("/time", HTTP_GET, [] (AsyncWebServerRequest *request) {
    char tmp[64];
    sprintf(tmp,"%s  --  Bytes Free: %d",formattedTime.c_str(), ESP.getFreeHeap());
    request->send(200, "text/plain", tmp);

  });

  server.on("/BlueIrisSession", HTTP_GET, [] (AsyncWebServerRequest *request) {//get blue iris session
    char tmpsession[80];
    request->send(200, "text/plain", tmpsession);
  });
  
  server.onNotFound(notFound);

}

// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  for(int i = 0;i<16;i++){
    char nameStart[16];
    char timerStart[16] ;
    char isHiddenStart[16] ;
    char startStart[16];
    char lenStart[16];
    char relayStart[16];
    char relayOnOff[16];

    sprintf(nameStart,"NAME%d",i+1);
    sprintf(timerStart,"TIMER%d",i+1);
    sprintf(isHiddenStart,"ISHIDDEN%d",i+1);
    sprintf(startStart,"START%d",i+1);
    sprintf(lenStart,"LEN%d",i+1);
    sprintf(relayStart,"RELAY%d",i+1);
    sprintf(relayOnOff,"RELAYONOFF%d",i+1);

    if(strcmp(var.c_str(),nameStart)==0){
      return names[i];
    }
    else if(strcmp(var.c_str(),timerStart)==0){
      char timerStateOutput[16]; 
      return outputStateChar (timerStateOutput, !timer[i] );
    }
    else if(strcmp(var.c_str(),isHiddenStart)==0){
      
      if(timer[i]){
        return "block";
      }
      else{
        return "none";
      } 
      //char stateOutput[16];
      //return outputStateChar(stateOutput, relay[i] );
    }
    else if(strcmp(var.c_str(),startStart)==0){
      return startTimes[i];
    }
    else if(strcmp(var.c_str(),lenStart)==0){
      char output[STR_BUFFER];
      sprintf(output,"%.3f",timeLengths[i]);
      return output;
      //return String(timeLengths[i]);
    }
    else if(strcmp(var.c_str(),relayStart)==0){

      char output[16];
      outputStateChar(output, relay[i] );
      //sprintf(output,"%d",relay[i]);
      return output;
      //return String(relay[i]);
    }
    else if(strcmp ( var.c_str() , relayOnOff ) == 0){
      if(!relay[i]){
        return "On";
      }
      else{return "Off";}
    }

  }

  if(strcmp(var.c_str(),"inputString")==0){
    char tmp[STR_BUFFER];
    return readFileAsChar(tmp,STR_BUFFER,LittleFS, "/inputString.txt");
  }
  else if(strcmp(var.c_str(),"inputInt")==0){
    char fileOutput[16];
    readFileAsChar(fileOutput,16, LittleFS, "/inputInt.txt");

    if(strcmp( fileOutput,"1")==0){
      return "\"1\" Checked";
    }
    else{
      return "\"0\"";
    }
  }
  else if(strcmp(var.c_str(),"inputFloat")==0){
    char tmp[16];
    return readFileAsChar(tmp,16,LittleFS, "/inputFloat.txt");
  }
  else if(strcmp(var.c_str(),"STREAM")==0){
    char tmp[150];
    char tmpsession[80];
    strcpy(tmpsession,"test");
    sprintf(tmp,"http://192.168.0.210:8080/image/GrowTent?session=%s",tmpsession);
    return tmp;
  }
  else if(strcmp(var.c_str(),"BUTTONPLACEHOLDER")==0){
    char buttons[2000 * 16] ;//2000byte buffer for each relay * 16 relays
    //String buttons ="";

    for(int i = 0;i<13;i++){

      char relayStateValue[16];
      char timerStateValue[16];
      char isHidden[16];
      char stateOnOff[16];
      if(!relay[i]){
        strcpy(relayStateValue,"checked");
        strcpy(stateOnOff,"On");
      }
      else{
        strcpy(relayStateValue,"");
        strcpy(stateOnOff,"Off");
      }
      if(timer[i]){
        strcpy(timerStateValue,"checked");
      }
      else{
        strcpy(timerStateValue,"");
      }
      //strcpy(relayStateValue, outputStateChar(relay[i]));
      //strcpy(timerStateValue, outputStateChar(!timer[i]));

      if(timer[i]){strcpy(isHidden,"block");}
      else{strcpy(isHidden,"none");}

      int curNum = i+1;
      //char curNum[16];
      //char curTime[strlen(timeLengths[i]) + 16]; //issue

      //sprintf(curNum,"%d",i+1);
      //sprintf(curTime,"%f",timeLengths[i])

      char tmpLine[256];

      strcat(buttons,"\n\n<div class=\"double\">\n");
      strcat(buttons,"<form action=\"/get\" target=\"hidden-form\">\n");
      sprintf(tmpLine,"Set Name: <input type=\"text\" name=\"name%d\" value=\"%s\"> <input type=\"submit\" value=\"Submit\" onclick=\"submitMessage()\">\n",curNum,names[i]);
      strcat(buttons,tmpLine);
      strcat(buttons,"</form><br>\n");
      sprintf(tmpLine,"<input type=\"checkbox\" id=\"c%d\" onclick=\"showMe(this,'timer%d')\" %s>Is this a timer?\n",curNum,curNum,timerStateValue);
      strcat(buttons,tmpLine);
      sprintf(tmpLine,"<div id=\"timer%d\" style=\"display: %s;\">\n",curNum,isHidden);
      strcat(buttons,tmpLine);
      strcat(buttons,"<form action=\"/get\" target=\"hidden-form\">\n");
      sprintf(tmpLine,"When to turn ON (current value %s): <input type=\"time\" name=\"start%d\" value=\"%s\"> <input type=\"submit\" value=\"Submit\" onclick=\"submitMessage()\">\n",startTimes[i],curNum,startTimes[i]);
      strcat(buttons,tmpLine);
      strcat(buttons,"</form><br>\n");
      strcat(buttons,"<form action=\"/get\" target=\"hidden-form\">\n");
      sprintf(tmpLine,"How many minutes to stay ON? (current value %.1f): <input type=\"number\" min=\"0\" max=\"1440\" step=\"0.1\" name=\"len%d\"> <input type=\"submit\" value=\"Submit\" onclick=\"submitMessage()\">\n",timeLengths[i],curNum);
      strcat(buttons,tmpLine);
      strcat(buttons,"</form>\n");
      strcat(buttons,"</div>\n");
      sprintf(tmpLine,"<h4>%s - State <span id=\"outputState%d\">%s</span></h4><label class=\"switch\"><input type=\"checkbox\" name=\"relay%d\" onchange=\"toggleCheckbox(this)\" id=\"output%d\" %s><span class=\"slider\"></span></label>",names[i],curNum,stateOnOff,curNum,curNum,relayStateValue);
      strcat(buttons,tmpLine);
      strcat(buttons,"</div>\n\n\n");


    }


    return buttons;
  }
  /*
  else if(var == "BUTTONINTERVAL"){
    String buttons ="";

    for(int i = 0;i <0;i++){

      buttons+="setInterval(function ( ) {\n";
      buttons+="  var xhttp = new XMLHttpRequest();\n";
      buttons+="  xhttp.onreadystatechange = function() {\n";
      buttons+="    if (this.readyState == 4 && this.status == 200) {\n";
      buttons+="      var inputChecked;\n";
      buttons+="      var outputStateM;\n";
      buttons+="      if( this.responseText == 1){ \n";
      buttons+="        inputChecked = true;\n";
      buttons+="        outputStateM = \"On\";\n";
      buttons+="      }\n";
      buttons+="      else { \n";
      buttons+="        inputChecked = false;\n";
      buttons+="        outputStateM = \"Off\";\n";
      buttons+="      }\n";
      buttons+="      document.getElementById(\"output"+String(i+1)+"\").checked = inputChecked;\n";
      buttons+="      document.getElementById(\"outputState"+String(i+1)+"\").innerHTML = outputStateM;\n";
      buttons+="    }\n";
      buttons+="  };\n";
      buttons+="  xhttp.open(\"GET\", \"/state?state="+String(i+1)+"\", true);\n";
      buttons+="  xhttp.send();\n";
      buttons+="}, 1000 ) ;\n";

    }
    return buttons;
  }
  else if(var == "TIMERINTERVAL"){
    String buttons ="";

    for(int i = 0;i <0;i++){

      buttons+="setInterval(function ( ) {\n";
      buttons+="  var xhttp = new XMLHttpRequest();\n";
      buttons+="  xhttp.onreadystatechange = function() {\n";
      buttons+="    if (this.readyState == 4 && this.status == 200) {\n";
      buttons+="      var inputChecked;\n";
      buttons+="      var outputStateM;\n";
      buttons+="      if( this.responseText == 1){ \n";
      buttons+="        inputChecked = true;\n";
      buttons+="        outputStateM = \"block\";\n";
      //buttons+="        alert(\"Saved value to ESP SPIFFS\");\n";
      buttons+="      }\n";
      buttons+="      else { \n";
      buttons+="        inputChecked = false;\n";
      buttons+="        outputStateM = \"none\";\n";
      buttons+="      }\n";
      buttons+="      document.getElementById(\"c"+String(i+1)+"\").checked = inputChecked;\n";
      buttons+="      document.getElementById(\"timer"+String(i+1)+"\").style.display = outputStateM;\n";
      buttons+="    }\n";
      buttons+="  };\n";
      buttons+="  xhttp.open(\"GET\", \"/timer?state="+String(i+1)+"\", true);\n";
      buttons+="  xhttp.send();\n";
      buttons+="}, 1000 ) ;\n";

    }
    return buttons;
  }
  else if(var == "TIMEPLACEHOLDER"){
    return "<span id=\"internalTime\"></span>";
  }
  */
  char emptyStr[16] = "";
  //strcpy(emptyStr,"");
  return emptyStr;
}

String outputState(int relay){
  if(!relay){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

char* outputStateChar(char* destination, int relay){
  if(!relay){
    strcpy(destination,"checked");
    return destination;
  }
  else {
    strcpy(destination,"");
    return destination;
  }
  strcpy(destination,"");
  return destination;
}

void updateTimer(char* state,char* timerString){

  //String numberString = timerString;

  int debug = 0;

  char filename[16];
  sprintf(filename,"/%s.txt",timerString);

  //String filename= "/";
  //filename += timerString;
  //filename+=".txt";

  //String value;
  //value = state;
  int selectedTimer = strtol(strtok(timerString,"timer"),NULL,10);
  //int selectedTimer = timerString[5] - '0'; //strtol(numberString.substring(5).c_str(),NULL,10);
  if(debug){
    Serial.println("SELECTED TIMER IS "+String(selectedTimer));
    printf("timerString is: %s\n\n",timerString);

  }

  for(int i = 0; i <16;i++){

    if((selectedTimer-1) == i){

      timer[i] = state[0] - '0';

      if(debug){printf("timer[%d] state: %d ... should be: %s\n\n",i,timer[i],state);}

      writeFile(LittleFS,filename,state);
      if(!timer[i]){relay[i]=HIGH;}

    }

  }
}
void updateRelay(char* state,char* relayString){

  int debug = 0;

  char filename[16];
  sprintf(filename,"/%s.txt",relayString);

  //String numberString = relayString;
  //String serialOutput;
  //serialOutput+="{\"gpio\":";
  //int selectedRelay = relayString[5] -'0'; //strtol(numberString.substring(5).c_str(),NULL,10);  
  int selectedRelay = strtol(strtok(relayString,"relay"),NULL,10);
  
  if(debug){

    Serial.println("SELECTED RELAY IS "+String(selectedRelay+21));
    printf("relayString is: %s\n\n",relayString);
  }
  //serialOutput+=selectedRelay;
  //serialOutput+=",\"state\":";
  //serialOutput+=state;
  //serialOutput+="}";
      //Wire.beginTransmission(8); 
      //Wire.write(tmp.c_str());  
      //Wire.endTransmission();    
  //String filename= "/";
  //filename += relayString;
  //filename+=".txt";
  //String value;
  //value = state;

  for(int i = 0; i <16;i++){
    if((selectedRelay-1) == i  )       {
      relay[i] = state[0]-'0';

      if(debug){printf("relay[%d] state: %d ... should be: %s\n\n",i,relay[i],state);}

      writeFile(LittleFS,filename,state);

    }

  }



}

void readSetRelays(){

  for(int i = 0;i<16;i++){

    //Generate filepaths-----------------------
    char nameFile[16];
    char startFile[16];
    char lenFile[16];
    char timerFile[16];
    char relayFile[16];

    sprintf(nameFile,"/name%d.txt",i+1);
    sprintf(startFile,"/start%d.txt",i+1);
    sprintf(lenFile,"/len%d.txt",i+1);
    sprintf(timerFile,"/timer%d.txt",i+1);
    sprintf(relayFile,"/relay%d.txt",i+1);
    //---------------------------------------------------------

    //String nameFile = "name"+String(i+1);
    //String startFile = "start"+String(i+1);
    //String lenFile = "len"+String(i+1);
    //String timerFile = "timer"+String(i+1);
    //String relayFile = "relay"+String(i+1);

    //String nameString = readFile(LittleFS, nameFile);
    //String startString = readFile(LittleFS, startFile);
    //String lenFloat = readFile(LittleFS, lenFile);
    //String timerInt = readFile(LittleFS, timerFile);
    //String relayInt = readFile(LittleFS, relayFile);


    // Extract data from files ----------------------------------
    char nameString[STR_BUFFER];
    char startString[16];
    char lenFloat[16];
    char timerInt[16];
    char relayInt[16];

    readFileAsChar(nameString,STR_BUFFER,LittleFS,nameFile);
    readFileAsChar(startString,16,LittleFS,startFile);
    readFileAsChar(lenFloat,16,LittleFS,lenFile);
    readFileAsChar(timerInt,16,LittleFS,timerFile);
    readFileAsChar(relayInt,16,LittleFS,relayFile);
    //-----------------------------------------------------------------------------



    // If data not set, then set it to defaults, otherwise save data to program memory -----------
    if(strcmp(nameString,"")==0){
      char tmp[16];
      //strcpy(names[i],tmp);
      names[i]=tmp;//"GPIO "+String(i+22)+" ";
      sprintf(tmp,"Relay%d / GPIO %d",i+1,i+22);
    }
    else{
      //strcpy(names[i],nameString);
      names[i]=nameString;
    }

    if(strcmp(startString,"")==0){
      //strcpy(startTimes[i],"00:00");
      startTimes[i]="00:00";
    }
    else{
      //strcpy(startTimes[i],startString);
      startTimes[i]=startString;
    }

    if(strcmp(lenFloat,"")==0){
      timeLengths[i]=0;
    }
    else{
      timeLengths[i]=atof(lenFloat);
      //timeLengths[i]=lenFloat.toFloat();
    }

    if(strcmp(timerInt,"")==0){
      timer[i]=0;
    }
    else{
      timer[i]=atol(timerInt);
      //timer[i]=timerInt.toInt();
    }

    if(strcmp(relayInt,"")==0){
      relay[i]=HIGH;
    }
    else{
      relay[i]=atol(relayInt);
      //relay[i]=relayInt.toInt();
    }

    //--------------------------------------------------------------------------------------------------------
    

  }

}

void writeRelays(){

  for(int i = 0;i<16;i++){
    //delay(10);
    if(relay[i]==LOW){
        char tmp[32];
        sprintf(tmp,"{\"gpio\":%d,\"state\":0}",i+22);
        //String tmp;
        //tmp+="{\"gpio\":";
        //tmp+=i+22;
        //tmp+=",\"state\":1}";
        Wire.beginTransmission(8); 
        Wire.write(tmp);  
        Wire.endTransmission();     
    }
    else{
        //String tmp;
        //tmp+="{\"gpio\":";
        //tmp+=i+22;
        //tmp+=",\"state\":0}";

        char tmp[32];
        sprintf(tmp,"{\"gpio\":%d,\"state\":1}",i+22);

        Wire.beginTransmission(8); 
        Wire.write(tmp);  
        Wire.endTransmission();   
    }

  }

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

char* readFileAsChar(char* destination,int bufsize,fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    
    strcpy(destination,"");
    return destination;
  }
  Serial.println("- read from file:");
  //char* fileContent = (char*) malloc((sizeof(char) * STR_BUFFER)+1);
  if(bufsize>STR_BUFFER){bufsize = STR_BUFFER;}
  int filePosition=0;
  while(file.available() && (filePosition<bufsize) ){
    destination[filePosition]=(char)file.read();
    filePosition++;
  }
  file.close();
  Serial.println(destination);
  return destination;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}













const char* login( const char* hostIP, const char* user, const char* pass) {  // Method/function defined inside the class
  //Serial.println("blueIris.login() Started");return "test";
  //if(isDebug()){Serial.println("blueIris.login() Started");return "test";}

  //return "test2";
  bool debug = true;
  
  //debug = false;
  const char* host = (const char*)hostIP;
  const char* username = (const char*) user;
  const char* password = (const char*)pass;

  if(debug){Serial.println("host,user,and pass assigned)");}

  HTTPClient http;
  WiFiClient client;
  http.useHTTP10(true);

  if(debug){Serial.println("HTTP Client Started");}


  //Generate initial JSON login command
  StaticJsonDocument<200> message;//(2048); //allocate
  message["cmd"] = "login"; //set JSON 

  String initialMsg;//char initialMsg[128]; //Allocate string
  serializeJson(message,initialMsg); //convert JSON object to string

  if(debug){
    Serial.println("Sending msg:\n-------------");
    Serial.println(initialMsg);

  }


  //Begin JSON Transmission over HTTP
  http.begin(client,host);//,(uint16_t)8080);     //Specify POST destination 
  http.addHeader("Content-Type", "application/json"); //Add header to let server know this is a JSON msg
  int postCode = http.POST(initialMsg) ;
  if(debug){

    Serial.print("Connectiong to: ");
    Serial.println(host);
    Serial.print("PostCode:");
    Serial.println(postCode);
  }


  if(postCode == 200){

  
    // Parse retrieved JSON object
    //DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);//(192);
    StaticJsonDocument<500> doc;
    auto error = deserializeJson(doc, http.getStream());
    if (error) {

      if(debug){
        Serial.println(F("Parsing response to initial login failed!"));
      }
      http.end();
      return NULL;
    }
    http.end();

    const char* result = doc["result"]; // "fail"
    const char* tmpSession = doc["session"]; // "0e885c285f45408520193ece11f8419f"

    if(strcmp(result,"fail") == 0){
      if(debug){
        const char* data_reason = doc["data"]["reason"]; // "missing response"
        Serial.println(data_reason);
      }
      
      //return NULL; ITS OK TO FAIL THE FIRST TIME
    }
    
    if(debug){
      String debugOutput;
      serializeJson(doc,debugOutput);
      Serial.println("Recieved Response\n------------");
      Serial.println(debugOutput);
    }

    //Create login string with session information
    char loginString[strlen(username)+strlen(password)+80] ;
    sprintf(loginString,"%s:%s:%s",username,tmpSession,password);

    unsigned char* hash = MD5::make_hash(loginString);
    char* md5str = MD5::make_digest(hash,16);

    //Generate JSON Message
    message.clear(); //clear JSON doc
    message["cmd"] = "login";
    message["session"] = tmpSession;
    message["response"] = md5str; //Set MD5 response

    free(hash);
    free(md5str);

    //Allocate string
    //char msgString[1024];
    String msgString;
    //Convert JSON object to string
    serializeJson(message,msgString);

    if(debug){
      Serial.println("Sending Message2:\n-------------------");
      Serial.println(msgString);
    }

    //Begin transmission over HTTP
    http.begin(client,host);//,(uint16_t)8080);     //Specify POST destination
    http.addHeader("Content-Type", "application/json");
    int postCode2 = http.POST(msgString);
    if(debug){
      Serial.print("PostCode: ");
      Serial.println(postCode2);
    }
    if( postCode2== 200){

      if(debug){
        Serial.println("Second MSG Recieved by Server OK");
      }
      
      doc.clear();
      auto error = deserializeJson(doc, http.getStream());
      if (error) {
        //http.end();

        if(debug){
          Serial.println(F("Parsing response to seconday login prompt failed!"));
        }
        //return NULL;
      }

      http.end();

      

      const char* result = doc["result"]; // "fail"
      const char* tmpSession = doc["session"]; // "0e885c285f45408520193ece11f8419f"
      if(debug){
        Serial.println("Deserialization of response complete");
        //Serial.print("result=");
        //Serial.println(result);
        //Serial.print("Session=");
        //Serial.println(tmpSession);
      }

      if(strcmp(result,"fail") == 0){
        if(debug){
          const char* data_reason = doc["data"]["reason"]; // "missing response"
          Serial.println(data_reason);
        }
        
        return NULL;
      }

      if(debug){
        //char debugOutput[1500];
        //serializeJson(doc,debugOutput);
        //Serial.println("Recieved Response\n------------");
        //Serial.println(debugOutput);
      }
      //debug = true;
      return tmpSession;
    }
    else{
      http.end();  //Close connection
      if(debug){
        Serial.println("Error in response to MD5");
      }
      return NULL;

    }
  


  }
  else
  {
    http.end();  //Close connection
    if(debug){
      Serial.println("Error in response to initial login request");

    }
    return NULL;
  }



}
