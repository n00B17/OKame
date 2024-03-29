/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Map between servos and board pins
// FrontLeft 27, 25
// FrontRight 26, 18
// BackLeft 32,12
// BackRight 19, 23
// ServoArm 21, 17, 16

#include <Arduino.h>
// #include <ESPmDNS.h>
#include "WiFiConfig.h"


//#include <WiFi.h>
//#include <WiFiMulti_Generic.h>
//#include <SoftwareSerial.h>
//#include <Arduino_FreeRTOS.h>
//#include <AsyncTCP.h> already included in AsyncWebServer.h

//#include <ESPAsyncWebServer.h> // Async Web Server likely already included in ElegantOTA.h
#include "minikame.h"
#include <ElegantOTA.h>
#include "credentials.h" //
//#include <WiFiManager.h>
// #include <ESPAsyncWiFiManager.h>
#include <ArduinoJson.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS or set in credentials.h (see example file and rename)


// const char* ssid     = SECRET_WIFI_SSID;
// const char* password = SECRET_WIFI_PASS;

MiniKame robot;
boolean walk = false;
const char* PARAM_INPUT_1 = "id";

const int output = 2;

String ComCode;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html>
<head><meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
  <title>Os Kame</title>
  <style>"body {
    background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }"
    #JD {text-align: center;}#JD {text-align: center;font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif;font-size: 24px;}
    .foot {text-align: center;font-family: "Comic Sans MS", cursive;font-size: 9px;color: #F00;} 
    .antiselect{user-select: none;}
    .button {
      border: none;color: white;padding: 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 12px;
      box-shadow: 0 6px #999;
      width: 100%;
    }
    .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
      .red {background-color: #F00;}
      .green {background-color: #090;}
      .yellow {background-color:#F90;}
      .blue {background-color:#03C;} 
      .antiselect{user-select: none;}  
    </style>
</head>
<body onload="javascript:start();">&nbsp;
  <table width="100%" border="1"><tr><td bgcolor="#FFFF33" id="JD">Quadruped Controller</td></tr></table>
  <table width="100" height="249" border="0" align="center">
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="nix" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect red">nix</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="forward" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect green">Forward</button></label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="nix"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect red antiselect">nix</button> </label></form></td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="left" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect green">Turn_Left</button></label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="stopit"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect red">Stop_all</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="right"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect green">Turn_Right</button></label></form></td>
    </tr>
    <tr><td>&nbsp;</td><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="walkback" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect green">back</button></label></form></td><td>&nbsp;</td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="hello" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect yellow">Hello </button></label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="dance"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Dance</button></label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="pushup"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect yellow">PushUp</button></label></form></td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="updown" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">UpDown</button></label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="moonwalk"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">MoonWalk</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="frontback"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">FrontBack</button> </label></form></td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="split" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Split</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="jump"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Jump</button></label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="omniwalk"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">OmniWalk</button></label></form></td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="nix" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect yellow">nix</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="run"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Run</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="nix"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect yellow">nix</button> </label></form></td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="update" type="button" onmousedown="goToUpdatePage();" ontouchstart="goToUpdatePage();" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect yellow">Update</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action="">&nbsp;</form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="wifi"  type="button" onmousedown="goToWifiPage();" ontouchstart="goToWifiPage();" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect yellow">WiFi</button> </label></form></td>
    </tr>
    <!--
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="Unterarm_hoch" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Unterarm_hoch</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="Oberarm_hoch"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Oberarm_hoch</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="Greifer_zu"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Greifer_zu</button> </label></form></td>
    </tr>
    <tr><td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="Unterarm_runter" type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Unterarm_runter</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="Oberarm_runter"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Oberarm_runter</button> </label></form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action=""><label><button id="Greifer_auf"  type="button" onmousedown="toggleOnbox(this);" ontouchstart="toggleOnbox(this);" onmouseup="toggleOffbox(this);" ontouchend="toggleOffbox(this);" class="button antiselect blue">Greifer_auf</button> </label></form></td>
    </tr>
  </table><p class="foot"><a href="/update">Update Firmware</a>.</p></body> -->
  </html>

   <script>
   function toggleOnbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/on?id="+x.id , true);
     xhr.send();
   }
   function toggleOffbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/off" , true);
     xhr.send();
   }
   function goToUpdatePage() {
    // Redirect to the "/update" page
    window.location.href = "/update";
    }
    function goToWifiPage() {
    // Redirect to the "/update" page
    window.location.href = "/wifi-setting";
    }
  </script>
  </body>
</html>)rawliteral";


//for now this the wifi-setting page is just a place holder 
// const char wifi_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html><html>
// <head> TEstpage </head>
// </html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);
WiFiConfig WifiConfig;




void setup() {
  Serial.begin(115200);
  Serial.println();

  // tries to connect to wifi in sta mode using the stored credentials, falls back to app mode if it fails
  WifiConfig.WiFiSetup();
  // handles the wifi settings page if requested
  WifiConfig.WiFiWebHandler(server);
  

  ComCode ="";
  

  
  
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);

    

  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  
  // Receive an HTTP GET request
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    int paramsNr = request->params();

    //Serial.println(paramsNr);
    //Serial.println(request->hasParam("buttonid")); 
    
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      ComCode = inputMessage;
      inputParam = PARAM_INPUT_1;
      Serial.print("buttonid=");
      Serial.println(ComCode);
            
     }

    
    digitalWrite(output, HIGH);
    request->send(200, "text/plain", "ok");
    walk=true;
    Serial.println("Walk on");
  });

  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(output, LOW);
    request->send(200, "text/plain", "ok");
    walk=false;
    Serial.println("Walk off");
    ComCode ="";
  });
  
  server.onNotFound(notFound);
  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  robot.init();
}

void loop() {
  if (walk){
    if (ComCode=="forward") {
    robot.walk(1,550);
    }
    if (ComCode=="walkback") {
    robot.walkBack(1,550);
    }
    if (ComCode=="stopit") {
    robot.home();
    }
    if (ComCode=="dance") {
    robot.dance(2,1000);
    }
    if (ComCode=="moonwalk") {
    robot.moonwalkL(2,1000);
    }
    if (ComCode=="jump") {
    robot.jump();
    }
    if (ComCode=="run") {
    robot.walk(1,330);
    }
    if (ComCode=="left") {
    robot.turnL(1,550);
    }
    if (ComCode=="hello") {
    robot.hello();
    }
    if (ComCode=="updown") {
    robot.upDown(1,550);
    }
    if (ComCode=="split") {
    robot.zero();
    }
    if (ComCode=="right") {
    robot.turnR(1,550);
    }
    if (ComCode=="pushup") {
    robot.pushUp(2,2000);
    }
    if (ComCode=="frontback") {
    robot.frontBack(4,200);
    }
    if (ComCode=="omniwalk") {
    robot.omniWalk(2,550,0,2);
    }
//arm
    if (ComCode=="Unterarm_hoch") {
    robot.lowarmUp();
    }
    if (ComCode=="Oberarm_hoch") {
    robot.uparmUp();
    }
    if (ComCode=="Greifer_zu") {
    robot.clawClose();
    }
    if (ComCode=="Unterarm_runter") {
    robot.lowarmDown();
    }
    if (ComCode=="Oberarm_runter") {
    robot.uparmDown();
    }
    if (ComCode=="Greifer_auf") {
    robot.clawOpen();
    }
    //Serial.print("ComCode=");
    //Serial.println(ComCode);    
//update+wifi
    if (ComCode=="wifi") {
    Serial.println(ComCode);  
    }    
    if (ComCode=="update") {
    Serial.println(ComCode);      
    } 
  }
 if (!walk){
  robot.home();
 }
}
