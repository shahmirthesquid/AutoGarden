
#include "growboxFunctions.h"
/*

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

</script>

  </head><body>
  <h1>GrowBox Control Panel!</h1>
  <p>Time: <span id="internalTime"></span></p>
  %BUTTONPLACEHOLDER%


  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

*/


/*
 * Simple NTP client
 * https://mischianti.org/
 *
 * The MIT License (MIT)
 * written by Renzo Mischianti <www.mischianti.org>
 */
 
