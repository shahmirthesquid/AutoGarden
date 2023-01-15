/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#include "growboxFunctions.h"



// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>GrowBox</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  html {font-family: Arial; display: inline-block; text-align: center;}
  h2 {font-size: 3.0rem;}
  p {font-size: 3.0rem;}
  body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
  .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
  .switch input {display: none}
  .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
  .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
  .inline-block-child {display: inline-block;}
  p.double {border-style: double;}
  div.double {border-style: double;}
  .center {
  display: block;
  margin-left: auto;
  margin-right: auto;
  width: 99%;}
  input:checked+.slider {background-color: #2196F3}
  input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
  <script>
  
    function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(true); }, 500);   
    }

    function toggleCheckbox(element) {
      var xhr = new XMLHttpRequest();
      if(element.checked){ xhr.open("GET", "/update?state=0_"+element.name, true); }
      else { xhr.open("GET", "/update?state=1_"+element.name, true); }
      xhr.send();
    }

    function showMe (chkbox, div) {
      var vis = "none";
      var xhr = new XMLHttpRequest();

      if(chkbox.checked){
        vis = "block";
        xhr.open("GET", "/update?timer=1_"+div, true);

      }
      else { xhr.open("GET", "/update?timer=0_"+div, true); }

      document.getElementById(div).style.display = vis;

      xhr.send();
    }

    function buttonRefresh(i) {
      let output = "output";
      output+= i;
      let outputState = "outputState";
      outputState+= i;
      let send = "/state?state="
      send+=i;
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var inputChecked;
          var outputStateM;
          if( this.responseText == 1){ 
            inputChecked = true;
            outputStateM = "On";
          }
          else { 
            inputChecked = false;
            outputStateM = "Off";
          }
          document.getElementById(output).checked = inputChecked;
          document.getElementById(outputState).innerHTML = outputStateM;
        }
      };
      xhttp.open("GET", send, true);
      xhttp.send();
    }

    function timerRefresh(i) {
      let output = "c";
      output+= i;
      let outputState = "timer";
      outputState+= i;
      let send = "/timer?state="
      send+=i;
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var inputChecked;
          var outputStateM;
          if( this.responseText == 1){ 
            inputChecked = true;
            outputStateM = "block";
          }
          else { 
            inputChecked = false;
            outputStateM = "none";
          }
          document.getElementById(output).checked = inputChecked;
          document.getElementById(outputState).style.display = outputStateM;
        }
      };
      xhttp.open("GET", send, true);
      xhttp.send();
    }

    setInterval(function ( ) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("internalTime").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/time", true);
      xhttp.send();
    }, 1000 ) ;

    setInterval(function ( ) {
      for(let i = 1; i<=16;i++){
        buttonRefresh(i);
        timerRefresh(i);
      }
    }, 1000);

    //setInterval(function() {
    //  imgObj = document.getElementById('img1');
    //  imgObj.src ="http://192.168.0.104/auto.jpg?" + (new Date()).getTime();
    //  
    //}, 1000);

</script>

  </head><body>
  <h1>GrowBox Control Panel!</h1>
  <p>Time: <span id="internalTime"></span></p>

  <label id="biError" style="display: block"><a href="http://99.255.0.148:8080" target="_blank">BlueIris</a> Video Stream:</label><br>
  <!-- <img id="img1" border="0" class="center" src="http://192.168.0.104/auto.jpg?"><br> -->

  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name1" value="%NAME1%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c1" onclick="showMe(this,'timer1')" %TIMER1% ">Is this a timer?

    <div id="timer1" style="display: %ISHIDDEN1%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START1%): <input type="time" name="start1" value="%START1%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN1%): <input type="number" min="0" step="0.1" name="len1"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME1% - State <span id="outputState1"></span></h4><label class="switch"><input type="checkbox" name="relay1" onchange="toggleCheckbox(this)" id="output1" %RELAY1% ><span class="slider"></span></label>
  
  </div>







  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name2" value="%NAME2%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c2" onclick="showMe(this,'timer2')" %TIMER2% ">Is this a timer?

    <div id="timer2" style="display: %ISHIDDEN2%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START2%): <input type="time" name="start2" value="%START2%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN2%): <input type="number" min="0" step="0.1" name="len2"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME2% - State <span id="outputState2"></span></h4><label class="switch"><input type="checkbox" name="relay2" onchange="toggleCheckbox(this)" id="output2" %RELAY2% ><span class="slider"></span></label>
  
  </div>





  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name3" value="%NAME3%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c3" onclick="showMe(this,'timer3')" %TIMER3% ">Is this a timer?

    <div id="timer3" style="display: %ISHIDDEN3%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START3%): <input type="time" name="start3" value="%START3%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN3%): <input type="number" min="0" step="0.1" name="len3"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME3% - State <span id="outputState3"></span></h4><label class="switch"><input type="checkbox" name="relay3" onchange="toggleCheckbox(this)" id="output3" %RELAY3% ><span class="slider"></span></label>
  
  </div>








  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name4" value="%NAME4%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c4" onclick="showMe(this,'timer4')" %TIMER4% ">Is this a timer?

    <div id="timer4" style="display: %ISHIDDEN4%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START4%): <input type="time" name="start4" value="%START4%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN4%): <input type="number" min="0" step="0.4" name="len4"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME4% - State <span id="outputState4"></span></h4><label class="switch"><input type="checkbox" name="relay4" onchange="toggleCheckbox(this)" id="output4" %RELAY4% ><span class="slider"></span></label>
  
  </div>




  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name5" value="%NAME5%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c5" onclick="showMe(this,'timer5')" %TIMER5% ">Is this a timer?

    <div id="timer5" style="display: %ISHIDDEN5%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START5%): <input type="time" name="start5" value="%START5%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN5%): <input type="number" min="0" step="0.1" name="len5"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME5% - State <span id="outputState5"></span></h4><label class="switch"><input type="checkbox" name="relay5" onchange="toggleCheckbox(this)" id="output5" %RELAY5% ><span class="slider"></span></label>
  
  </div>





  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name6" value="%NAME6%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c6" onclick="showMe(this,'timer6')" %TIMER6% ">Is this a timer?

    <div id="timer6" style="display: %ISHIDDEN6%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START6%): <input type="time" name="start6" value="%START6%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN6%): <input type="number" min="0" step="0.1" name="len6"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME6% - State <span id="outputState6"></span></h4><label class="switch"><input type="checkbox" name="relay6" onchange="toggleCheckbox(this)" id="output6" %RELAY6% ><span class="slider"></span></label>
  
  </div>




  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name7" value="%NAME7%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c7" onclick="showMe(this,'timer7')" %TIMER7% ">Is this a timer?

    <div id="timer7" style="display: %ISHIDDEN7%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START7%): <input type="time" name="start7" value="%START7%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN7%): <input type="number" min="0" step="0.1" name="len7"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME7% - State <span id="outputState7"></span></h4><label class="switch"><input type="checkbox" name="relay7" onchange="toggleCheckbox(this)" id="output7" %RELAY7% ><span class="slider"></span></label>
  
  </div>





  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name8" value="%NAME8%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c8" onclick="showMe(this,'timer8')" %TIMER8% ">Is this a timer?

    <div id="timer8" style="display: %ISHIDDEN8%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START8%): <input type="time" name="start8" value="%START8%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN8%): <input type="number" min="0" step="0.1" name="len8"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME8% - State <span id="outputState8"></span></h4><label class="switch"><input type="checkbox" name="relay8" onchange="toggleCheckbox(this)" id="output8" %RELAY8% ><span class="slider"></span></label>
  
  </div>





  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name9" value="%NAME9%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c9" onclick="showMe(this,'timer9')" %TIMER9% ">Is this a timer?

    <div id="timer9" style="display: %ISHIDDEN9%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START9%): <input type="time" name="start9" value="%START9%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN9%): <input type="number" min="0" step="0.1" name="len9"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME9% - State <span id="outputState9"></span></h4><label class="switch"><input type="checkbox" name="relay9" onchange="toggleCheckbox(this)" id="output9" %RELAY9% ><span class="slider"></span></label>
  
  </div>







  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name10" value="%NAME10%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c10" onclick="showMe(this,'timer10')" %TIMER10% ">Is this a timer?

    <div id="timer10" style="display: %ISHIDDEN10%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START10%): <input type="time" name="start10" value="%START10%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN10%): <input type="number" min="0" step="0.1" name="len10"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME10% - State <span id="outputState10"></span></h4><label class="switch"><input type="checkbox" name="relay10" onchange="toggleCheckbox(this)" id="output10" %RELAY10% ><span class="slider"></span></label>
  
  </div>




  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name11" value="%NAME11%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c11" onclick="showMe(this,'timer11')" %TIMER11% ">Is this a timer?

    <div id="timer11" style="display: %ISHIDDEN11%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START11%): <input type="time" name="start11" value="%START11%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN11%): <input type="number" min="0" step="0.1" name="len11"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME11% - State <span id="outputState11"></span></h4><label class="switch"><input type="checkbox" name="relay11" onchange="toggleCheckbox(this)" id="output11" %RELAY11% ><span class="slider"></span></label>
  
  </div>



  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name12" value="%NAME12%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c12" onclick="showMe(this,'timer12')" %TIMER12% ">Is this a timer?

    <div id="timer12" style="display: %ISHIDDEN12%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START12%): <input type="time" name="start12" value="%START12%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN12%): <input type="number" min="0" step="0.1" name="len12"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME12% - State <span id="outputState12"></span></h4><label class="switch"><input type="checkbox" name="relay12" onchange="toggleCheckbox(this)" id="output12" %RELAY12% ><span class="slider"></span></label>
  
  </div>




  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name13" value="%NAME13%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c13" onclick="showMe(this,'timer13')" %TIMER13% ">Is this a timer?

    <div id="timer13" style="display: %ISHIDDEN13%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START13%): <input type="time" name="start13" value="%START13%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN13%): <input type="number" min="0" step="0.1" name="len13"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME13% - State <span id="outputState13"></span></h4><label class="switch"><input type="checkbox" name="relay13" onchange="toggleCheckbox(this)" id="output13" %RELAY13% ><span class="slider"></span></label>
  
  </div>






  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name14" value="%NAME14%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c14" onclick="showMe(this,'timer14')" %TIMER14% ">Is this a timer?

    <div id="timer14" style="display: %ISHIDDEN14%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START14%): <input type="time" name="start14" value="%START14%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN14%): <input type="number" min="0" step="0.1" name="len14"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME14% - State <span id="outputState14"></span></h4><label class="switch"><input type="checkbox" name="relay14" onchange="toggleCheckbox(this)" id="output14" %RELAY14% ><span class="slider"></span></label>
  
  </div>





  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name15" value="%NAME15%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c15" onclick="showMe(this,'timer15')" %TIMER15% ">Is this a timer?

    <div id="timer15" style="display: %ISHIDDEN15%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START15%): <input type="time" name="start15" value="%START15%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN15%): <input type="number" min="0" step="0.1" name="len15"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME15% - State <span id="outputState15"></span></h4><label class="switch"><input type="checkbox" name="relay15" onchange="toggleCheckbox(this)" id="output15" %RELAY15% ><span class="slider"></span></label>
  
  </div>


  <div class="double">
    <form action="/get" target="hidden-form">
      Set Name: <input type="text" name="name16" value="%NAME16%"> <input type="submit" value="Submit" onclick="submitMessage()">
    </form><br>

    <input type="checkbox" id="c16" onclick="showMe(this,'timer16')" %TIMER16% ">Is this a timer?

    <div id="timer16" style="display: %ISHIDDEN16%;">

      <form action="/get" target="hidden-form">
        When to turn ON (current value %START16%): <input type="time" name="start16" value="%START16%"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form><br>

      <form action="/get" target="hidden-form">
        How many minutes to stay ON? (current value %LEN16%): <input type="number" min="0" step="0.1" name="len16"> <input type="submit" value="Submit" onclick="submitMessage()">
      </form>

    </div>

    <h4>%NAME16% - State <span id="outputState16"></span></h4><label class="switch"><input type="checkbox" name="relay16" onchange="toggleCheckbox(this)" id="output16" %RELAY16% ><span class="slider"></span></label>
  
  </div>





  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";





const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "timer";

const int output = 2;
const int buttonPin = 3;

int relay[16];          // = {relay1,relay2,relay3,relay4,relay5,relay6};
String names[16];       // = {name1,name2,name3,name4,name5,name6};
int timer[16];          // = {timer1,timer2,timer3,timer4,timer5,timer6};
String startTimes[16];  // = {start1,start2,start3,start4,start5,start6};
float timeLengths[16];  //={len1,len2,len3,len4,len5,len6};


unsigned long lastDrainTime;


// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

String formattedTime;
String Date;
int Day;
int Month;
int Year;
int hour = 0;
int minute = 0;
int second = 0;
int totalSec = 0;


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// A UDP instance to let us send and receive packets over UDP
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -18000;
//UTC X = X * 60 * 60
//UTC 8 = 8 * 60 * 60 = 28800
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
unsigned long epochTime = timeClient.getEpochTime();
struct tm* ptm = gmtime((time_t*)&epochTime);

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Khan - 2G";
const char* password = "fatima2005";

const char* PARAM_STRING = "inputString";
const char* PARAM_INT = "inputInt";
const char* PARAM_FLOAT = "inputFloat";


void setup() {


  Serial.begin(115200);
//pinMode(output, OUTPUT);
//digitalWrite(output, LOW);
//pinMode(buttonPin, INPUT);


// Initialize SPIFFS-------------
#ifdef ESP32
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
#else
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
#endif

  //SPIFFS.format();

  // Initialize WIFI-------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize Relays-------------
  readSetRelays();
  //ledState = readFile(SPIFFS,"/state.txt").toInt();

  // Initialize Server-------------
  serverSetup();
  server.begin();

  // Initialize Serial to Arduino MEGA using Wire.h-------------
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  Wire.begin(D1, D2);

  // Initialize  Drain Float Switch-------------------------------
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);






}

void loop() {

  timeClient.update();
  formattedTime = timeClient.getFormattedTime();
  Serial.println(formattedTime);
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();


  for (int i = 0; i < 16; i++) {
    Serial.print("Relay(Inverted)");
    Serial.print((i + 1));
    Serial.print("\nStatus: ");
    Serial.print(relay[i]);
    Serial.println();
    Serial.println("name" + String(i + 1) + " IS " + names[i] + "   start" + String(i + 1) + ": " + startTimes[i] + "   Timer" + String(i + 1) + ": " + timer[i] + "    len" + String(i + 1) + ": " + String(timeLengths[i]));
  }

  delay(1000);

  if(digitalRead(D5)==LOW){
    lastDrainTime = millis();
  }

  for (int i = 0; i < 16; i++) {


    if (timer[i]) {

      String onTime = startTimes[i];

      int extractedHour = strtol(onTime.substring(0, 2).c_str(), NULL, 10);
      int extractedMin = strtol(onTime.substring(3, 5).c_str(), NULL, 10);
      int extractedSec = (extractedHour * 60 * 60) + (extractedMin * 60);

      float waterSeconds = timeLengths[i] * 60;
      totalSec = (timeClient.getHours() * 60 * 60) + (timeClient.getMinutes() * 60) + timeClient.getSeconds();

      Serial.println("totalSec = (" + String(hour) + "*60*60)+(" + String(minute) + "*60)+" + String(timeClient.getSeconds()));
      Serial.println(totalSec);
      Serial.println(extractedSec);
      Serial.println(extractedSec + waterSeconds);

      if (totalSec >= extractedSec && totalSec <= (extractedSec + waterSeconds)) {
        Serial.println("SETTING HIGH");
        //digitalWrite(output,1);
        Serial.println("Watering time!");
        if ((totalSec + 2) > (extractedSec + waterSeconds)) {
          relay[i] = HIGH;
          String tmp;
          tmp += "/relay";
          tmp += (i + 1);
          tmp += ".txt";
          //writeFile(SPIFFS, tmp.c_str(), "1");

        } else {
          relay[i] = LOW;
        }
      }
    }

    if(names[i] == "Drain Pump"){
      if(millis() > (lastDrainTime + 30000)   ){
        relay[i]= HIGH; //set relay off
      }
      else{
        relay[i] = LOW; //set relay ON
      }
    }
    else if(names[i] == "Runoff Pumps"){
      if(digitalRead(D6) == LOW){//If water level sensors activate
        relay[i]=LOW; //set relay ON
      }
      else{
        relay[i]=HIGH;//set relay off
      }
    }
  }


  writeRelays();
}
