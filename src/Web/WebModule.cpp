#include "WebModule.h"
#include "../Relay/RelayModule.h"
#include "../Servo/ServoModule.h"
#include "../Motor/MotorModule.h"
#include <WiFi.h>

WebServer server(80);

void handleRoot() {
  String ip = WiFi.localIP().toString();
  String gasColor = (currentGasLevelPercent > 40.0) ? "#FF4500" : (currentGasLevelPercent > 20.0) ? "#FFA500" : "#32CD32";
  String gasStatus = (currentGasLevelPercent > 40.0) ? "DANGER" : (currentGasLevelPercent > 20.0) ? "Warning" : "Safe";
  
  String html = R"====(
  <html>
  <head>
  <title>Smart Hydra Home</title>
  <meta http-equiv="refresh" content="5"> 
  <style>
    body {background:linear-gradient(135deg,#1f1c2c,#928DAB);color:white;text-align:center;font-family:'Poppins',sans-serif;}
    h1{color:#FFD700;margin-top:15px;}
    .section{margin:20px auto;padding:20px;width:85%;background:rgba(255,255,255,0.1);border-radius:16px;box-shadow:0 4px 20px rgba(0,0,0,0.3);}
    button{background:rgba(255,255,255,0.1);border:2px solid #FFD700;color:white;padding:10px 25px;margin:8px;border-radius:10px;font-size:16px;cursor:pointer;transition:0.3s;}
    button:hover{background:#FFD700;color:#000;}
    #map{width:90%;height:400px;margin:auto;border-radius:12px;}
    .gas-level, .access-log {font-size: 20px; font-weight: bold; padding: 10px; border-radius: 8px; margin: 10px 0;}
    .access-log p {font-size: 16px; font-weight: normal; margin: 5px 0;}
  </style>
  <script src="https://maps.googleapis.com/maps/api/js?key==GOOGLEKEY=="></script>
  <script>
    function initMap(){
      var loc={lat:==LAT==,lng:==LNG==};
      var map=new google.maps.Map(document.getElementById('map'),{zoom:15,center:loc});
      new google.maps.Marker({position:loc,map:map,title:'Hydra Home'});
    }
  </script>
  </head>
  <body onload="initMap()">
    <h1> Smart Hydra Home + Alexa + RFID + GPS + MQ2 + Face Unlock</h1>
    <p>IP Address: <b>==IP==</b></p>

    <div class="section">
      <h2>Main Door Access Event</h2>
      <div class="access-log" style="background-color: ==ACCESS_COLOR==;">
        **==ACCESS_MESSAGE==**
        <p>Source ID: ==ACCESS_UID==</p>
      </div>
    </div>
    <div class="section">
      <h2>Gas Sensor Status (MQ2)</h2>
      <div class="gas-level" style="background-color: ==GAS_COLOR==;">
        Gas Level: **==GAS_LEVEL==%** (**==GAS_STATUS==**)
      </div>
      <p>Raw Analog: **==SENSOR_VALUE==** | Voltage: **==VOLTAGE== V**</p>
    </div>

    <div class="section">
      <h2>Lights & Fan (Manual Control)</h2>
      <button onclick="location.href='/jai'">Jai Shree Ram Light</button>
      <button onclick="location.href='/fan'">Lower Light</button>
      <button onclick="location.href='/lower'">Fan</button>
      <button onclick="location.href='/upper'">Upper Light</button>
    </div>

    <div class="section">
      <h2>Doors & Curtain</h2>
      <button onclick="location.href='/door1'">Main Door</button>
      <button onclick="location.href='/door2'">Balcony Door</button>
      <button onclick="location.href='/curtain'">Curtain</button>
    </div>

    <div class="section">
      <h2>Vehicle Control</h2>
      <button onclick="location.href='/forward'">Forward</button><br>
      <button onclick="location.href='/left'">Left</button>
      <button onclick="location.href='/stop'">Stop</button>
      <button onclick="location.href='/right'">Right</button><br>
      <button onclick="location.href='/backward'">Backward</button>
    </div>

    <div class="section">
      <h2>Live GPS Location</h2>
      <div id="map"></div>
    </div>
  </body></html>
  )====";

  html.replace("==GAS_LEVEL==", String(currentGasLevelPercent, 1));
  html.replace("==SENSOR_VALUE==", String(currentSensorValue));
  html.replace("==VOLTAGE==", String(currentVoltage, 2));
  html.replace("==GAS_COLOR==", gasColor);
  html.replace("==GAS_STATUS==", gasStatus);
 
  html.replace("==ACCESS_MESSAGE==", lastDoorAccessMessage);
  html.replace("==ACCESS_UID==", lastAccessUID);
  html.replace("==ACCESS_COLOR==", lastAccessColor);
 
  html.replace("==LAT==", String(latitude, 6));
  html.replace("==LNG==", String(longitude, 6));
  html.replace("==IP==", ip);
  html.replace("==GOOGLEKEY==", googleApiKey);

  server.send(200, "text/html", html);
}

void doRelay(int index, int pin) {
  toggleRelay(index, pin);
  server.sendHeader("Location", "/");
  server.send(303);
}

void doDoor1() {
  toggleDoor1();
  server.sendHeader("Location", "/");
  server.send(303);
}

void doDoor2() {
  toggleDoor2();
  server.sendHeader("Location", "/");
  server.send(303);
}

void doCurtain() {
  toggleCurtain();
  server.sendHeader("Location", "/");
  server.send(303);
}

void doMoveForward() { moveForward(); server.sendHeader("Location", "/"); server.send(303); }
void doMoveBackward() { moveBackward(); server.sendHeader("Location", "/"); server.send(303); }
void doTurnLeft() { turnLeft(); server.sendHeader("Location", "/"); server.send(303); }
void doTurnRight() { turnRight(); server.sendHeader("Location", "/"); server.send(303); }
void doStopMotors() { stopMotors(); server.sendHeader("Location", "/"); server.send(303); }

void initWebServer() {
  server.on("/", handleRoot);
  server.on("/jai", [](){ doRelay(0, RELAY_JAI_SHREE_RAM); });
  server.on("/fan", [](){ doRelay(1, RELAY_FAN); });
  server.on("/lower", [](){ doRelay(2, RELAY_LOWER_LIGHT); });
  server.on("/upper", [](){ doRelay(3, RELAY_UPPER_LIGHT); });
  
  server.on("/door1", doDoor1);
  server.on("/door2", doDoor2);
  server.on("/curtain", doCurtain);
  
  server.on("/forward", doMoveForward);
  server.on("/backward", doMoveBackward);
  server.on("/left", doTurnLeft);
  server.on("/right", doTurnRight);
  server.on("/stop", doStopMotors);
  
  server.begin();
}

void handleClient() {
  server.handleClient();
}
