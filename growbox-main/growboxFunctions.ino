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
      String name = "name"+String(i+1);
      String startTime = "start"+String(i+1);
      String timeLen = "len"+String(i+1);
      String relayFile = "relay"+String(i+1);

      if (request->hasParam(name)) {
        inputMessage = request->getParam(name)->value();
        Serial.println(inputMessage);
        writeFile(LittleFS, ("/"+name+".txt").c_str(), inputMessage.c_str());
        names[i]=inputMessage;
      }
      // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
      else if (request->hasParam(startTime)) {
        //Serial.println("GOT HERE!!!!!!!!");
        inputMessage = request->getParam(startTime)->value();
        writeFile(LittleFS, ("/"+startTime+".txt").c_str(), inputMessage.c_str());
        startTimes[i]=inputMessage;

        relay[i]=HIGH; // turn off relay
        writeFile(LittleFS, ("/"+relayFile+".txt").c_str(), "1"); 
      }
      // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
      else if (request->hasParam(timeLen)) {
        inputMessage = request->getParam(timeLen)->value();
        writeFile(LittleFS, ("/"+timeLen+".txt").c_str(), inputMessage.c_str());
        timeLengths[i]=inputMessage.toFloat();

        relay[i]=HIGH; // turn off relay
        writeFile(LittleFS, ("/"+relayFile+".txt").c_str(), "1"); 
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
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {//relay state update
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;

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
        if(inputMessage==String(i+1)) {request->send(200, "text/plain", String(!relay[i]).c_str());}        
      }
    }
  });

    // Send a GET request to <ESP_IP>/state
  server.on("/timer", HTTP_GET, [] (AsyncWebServerRequest *request) {//get state of timer checks c1,c2,c3,etc
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      for(int i = 0; i <16;i++){
        if(inputMessage==String(i+1)) {
          request->send(200, "text/plain", String(timer[i]).c_str());
          //Serial.println(timer[i]);
        }        
      }
    }
  });
  
  server.on("/time", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", formattedTime);

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
    String nameStart ="NAME";
    String timerStart = "TIMER";
    String isHiddenStart = "ISHIDDEN";
    String startStart = "START";
    String lenStart = "LEN";
    String relayStart = "RELAY";
    nameStart += (i+1);
    timerStart += (i+1);
    isHiddenStart += (i+1);
    startStart += (i+1);
    lenStart += (i+1);
    relayStart += (i+1);
    if(var == nameStart){
      return names[i];
    }
    else if( var == timerStart){
      return outputState(!timer[i]);
    }
    else if( var == isHiddenStart){
      return outputState(relay[i]);
    }
    else if( var == startStart){
      return startTimes[i];
    }
    else if( var == lenStart){
      return String(timeLengths[i]);
    }
    else if( var == relayStart){
      return String(relay[i]);
    }

  }

  if(var == "inputString"){
    return readFile(LittleFS, "/inputString.txt");
  }
  else if(var == "inputInt"){
    if( readFile(LittleFS, "/inputInt.txt")=="1"){
      return "\"1\" Checked";
    }
    else{
      return "\"0\"";
    }
  }
  else if(var == "inputFloat"){
    return readFile(LittleFS, "/inputFloat.txt");
  }
  else if(var == "STREAM"){
    char tmp[150];
    char tmpsession[80];
    sprintf(tmp,"http://192.168.0.210:8080/image/GrowTent?session=%s",tmpsession);
    return tmp;
  }
  else if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";

    for(int i = 0;i<14;i++){
      //delay(10);
      String relayStateValue = outputState(relay[i]);
      String timerStateValue = outputState(!timer[i]);
      String isHidden;
      if(timer[i]){isHidden = "block";}
      else{isHidden="none";}
      String curNum = "";
      curNum+=(i+1);
      String curTime = "";
      curTime+=timeLengths[i];

      //Serial.println("relayStateValue: "+relayStateValue);
      //Serial.println("timerStateValue: "+timerStateValue);
      //Serial.println("isHidden: "+isHidden);

      buttons+="<div class=\"double\">\n";
      buttons+="<form action=\"/get\" target=\"hidden-form\">\n";
      buttons+="Set Name: <input type=\"text\" name=\"name"+curNum+"\" value=\""+names[i]+"\"> <input type=\"submit\" value=\"Submit\" onclick=\"submitMessage()\">\n";
      buttons+="</form><br>\n";
      buttons+= "<input type=\"checkbox\" id=\"c"+curNum+"\" onclick=\"showMe(this,'timer"+curNum+"')\" " + timerStateValue + ">Is this a timer?\n";
      buttons+="<div id=\"timer"+curNum+"\" style=\"display: "+isHidden+";\">\n";
      //buttons+="<script>hide('timer1')</script>\n";
      buttons+="<form action=\"/get\" target=\"hidden-form\">\n";
      buttons+="When to turn ON (current value "+startTimes[i]+"): <input type=\"time\" name=\"start"+curNum+"\" value=\""+startTimes[i]+"\"> <input type=\"submit\" value=\"Submit\" onclick=\"submitMessage()\">\n";
      buttons+="</form><br>\n";
      buttons+="<form action=\"/get\" target=\"hidden-form\">\n";
      buttons+= "How many minutes to stay ON? (current value "+curTime+"): <input type=\"number\" min=\"0\" step=\"0.1\" name=\"len"+curNum+"\"> <input type=\"submit\" value=\"Submit\" onclick=\"submitMessage()\">\n";
      buttons+="</form>\n";
      buttons+="</div>\n";
      buttons+= "<h4>"+names[i]+" - State <span id=\"outputState"+curNum+"\"></span></h4><label class=\"switch\"><input type=\"checkbox\" name=\"relay"+curNum+"\" onchange=\"toggleCheckbox(this)\" id=\"output"+curNum+"\" " + relayStateValue + "><span class=\"slider\"></span></label>";
      buttons+="</div>\n";

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
  return String();
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
void updateTimer(char* state,char* timerString){

  String numberString = timerString;
  String filename= "/";
  filename += timerString;
  filename+=".txt";
  String value;
  value = state;
  //int selectedTimer = strtol(strtok(timerString,"timer"),NULL,10);
  int selectedTimer = strtol(numberString.substring(5).c_str(),NULL,10);

  Serial.println("SELECTED TIMER IS "+String(selectedTimer));

  for(int i = 0; i <16;i++){

    if((selectedTimer-1) == i){

      timer[i] = value.toInt();
      writeFile(LittleFS,filename.c_str(),state);
      if(!timer[i]){relay[i]=HIGH;}

    }

  }
}
void updateRelay(char* state,char* relayString){
  String numberString = relayString;
  //String serialOutput;
  //serialOutput+="{\"gpio\":";
  int selectedRelay = strtol(numberString.substring(5).c_str(),NULL,10);  //strtol(strtok(relayString,"relay"),NULL,10)+21;
  Serial.println("SELECTED RELAY IS "+String(selectedRelay));
  //serialOutput+=selectedRelay;
  //serialOutput+=",\"state\":";
  //serialOutput+=state;
  //serialOutput+="}";
      //Wire.beginTransmission(8); 
      //Wire.write(tmp.c_str());  
      //Wire.endTransmission();    
  String filename= "/";
  filename += relayString;
  filename+=".txt";
  String value;
  value = state;

  for(int i = 0; i <16;i++){
    if((selectedRelay-1) == i  )       {
      relay[i] = value.toInt();
      writeFile(LittleFS,filename.c_str(),state);

    }

  }



}

void readSetRelays(){

  for(int i = 0;i<16;i++){
    String nameFile = "name"+String(i+1);
    String startFile = "start"+String(i+1);
    String lenFile = "len"+String(i+1);
    String timerFile = "timer"+String(i+1);
    String relayFile = "relay"+String(i+1);

    String nameString = readFile(LittleFS, ("/"+nameFile+".txt").c_str());
    String startString = readFile(LittleFS, ("/"+startFile+".txt").c_str());
    String lenFloat = readFile(LittleFS, ("/"+lenFile+".txt").c_str());
    String timerInt = readFile(LittleFS, ("/"+timerFile+".txt").c_str());
    String relayInt = readFile(LittleFS, ("/"+relayFile+".txt").c_str());

    if(nameString==""){
      names[i]="GPIO "+String(i+22)+" ";
    }
    else{
      names[i]=nameString;
    }

    if(startString==""){
      startTimes[i]="00:00";
    }
    else{
      startTimes[i]=startString;
    }

    if(lenFloat==""){
      timeLengths[i]=0;
    }
    else{
      timeLengths[i]=lenFloat.toFloat();
    }

    if(timerInt==""){
      timer[i]=0;
    }
    else{
      timer[i]=timerInt.toInt();
    }

    if(relayInt==""){
      relay[i]=HIGH;
    }
    else{
      relay[i]=relayInt.toInt();
    }

    

    //startTimes[i]=readFile(SPIFFS, ("/"+startFile+".txt").c_str());
    //timeLengths[i]=readFile(SPIFFS, ("/"+lenFile+".txt").c_str());
    //timer[i]=readFile(SPIFFS, ("/"+timerFile+".txt").c_str()).toInt();
    //relay[i]=readFile(SPIFFS, ("/"+relayFile+".txt").c_str()).toInt();    

  }

}

void writeRelays(){

  for(int i = 0;i<16;i++){
    //delay(10);
    if(relay[i]==HIGH){
        String tmp;
        tmp+="{\"gpio\":";
        tmp+=i+22;
        tmp+=",\"state\":1}";
        Wire.beginTransmission(8); 
        Wire.write(tmp.c_str());  
        Wire.endTransmission();     
    }
    else{
        String tmp;
        tmp+="{\"gpio\":";
        tmp+=i+22;
        tmp+=",\"state\":0}";
        Wire.beginTransmission(8); 
        Wire.write(tmp.c_str());  
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
