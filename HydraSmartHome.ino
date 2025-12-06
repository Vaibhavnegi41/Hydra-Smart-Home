/*************************************************************
 🏠 Smart Hydra Home + RFID + GPS + Alexa (Sinric Pro) + MQ2 Gas Alert + Face Recognition Door
*************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>


const char* ssid = "HiFi";
const char* password = "12345";


#define APP_KEY       "e5f45f6e-df54-464f-b552-814068b243bc"
#define APP_SECRET    "beef06a8-bfc4-404f-bbcf-93f0190c8835-b19c6cd1-8806-41c6-ae76-439b79751b67"
#define SWITCH_ID_1   "690f18aefb7ad926788957e6"
#define SWITCH_ID_2   "690f198bfb7ad92678895829"
#define SWITCH_ID_3   "690f19176cf03d7232c5cb18"


#define RELAY_ACTIVE_LOW true
#if RELAY_ACTIVE_LOW
  #define RELAY_ON  LOW
  #define RELAY_OFF HIGH
#else
  #define RELAY_ON  HIGH
  #define RELAY_OFF LOW
#endif


#define RELAY_JAI_SHREE_RAM 32
#define RELAY_FAN           22
#define RELAY_LOWER_LIGHT   33 //upper light hain yeh bhai
#define RELAY_UPPER_LIGHT   21


#define DOOR1_SERVO 15    // Face Recognition / RFID Controlled Door 
#define DOOR2_SERVO 2
#define CURTAIN_SERVO 4


#define LEFT_MOTOR_IN1 12
#define LEFT_MOTOR_IN2 13
#define RIGHT_MOTOR_IN1 14
#define RIGHT_MOTOR_IN2 27


#define SS_PIN  5
#define RST_PIN 25
MFRC522 mfrc522(SS_PIN, RST_PIN);
byte allowedUID[4] = {0xC7, 0x31, 0x7B, 0x05};


TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
double latitude = 0.0, longitude = 0.0;


WebServer server(80);
const char* googleApiKey = "AIzaSyC6cCoDPuWb2G84Yy2I7f4xaN5e4xkNA98"; //google maps api key


Servo door1, door2, curtain;
bool relayState[4] = { false, false, false, false }; // false = OFF, true = ON (logical state)
bool doorOpen[2] = {false, false};
bool curtainOpen = false;
bool door1OpenByRFID = false;
unsigned long door1OpenUntil = 0;
const unsigned long DOOR1_OPEN_MS = 5000;


const int mq2Pin = 34;
const int buzzerPin = 26;
const int gasThreshold = 1500;
const unsigned long MQ2_READ_INTERVAL = 1000; 
unsigned long lastMq2ReadMillis = 0;


bool buzzerActive = false;
unsigned long buzzerStartMillis = 0;
const unsigned long BUZZER_ON_DURATION = 300; 
const unsigned long BUZZER_OFF_DURATION = 200; 
unsigned long lastBuzzerToggle = 0;
bool buzzerOnState = false;


const unsigned long MOD_WARN_BUZZ_DURATION = 100;
const unsigned long MOD_WARN_PAUSE = 200;
bool modWarnActive = false;
unsigned long modWarnStart = 0;
unsigned long lastModWarnToggle = 0;
bool modWarnState = false;


float currentGasLevelPercent = 0.0;
int currentSensorValue = 0;
float currentVoltage = 0.0;


String lastDoorAccessMessage = "No recent access attempt.";
String lastAccessUID = "N/A";
String lastAccessColor = "#928DAB"; 



bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == SWITCH_ID_1) {
    digitalWrite(RELAY_JAI_SHREE_RAM, state ? RELAY_ON : RELAY_OFF);
    relayState[0] = state;
  } else if (deviceId == SWITCH_ID_2) {
    digitalWrite(RELAY_FAN, state ? RELAY_ON : RELAY_OFF);
    relayState[1] = state;
  } else if (deviceId == SWITCH_ID_3) {
    digitalWrite(RELAY_UPPER_LIGHT, state ? RELAY_ON : RELAY_OFF);
    relayState[3] = state;
  }
  Serial.printf("Device %s turned %s\n", deviceId.c_str(), state ? "ON" : "OFF");
  return true;
}


void handleRoot() {
  String ip = WiFi.localIP().toString();
  Serial.print("Webpage loaded. Current IP: "); 
  Serial.println(ip);
  

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


void toggleRelay(int index, int relayPin) {
  relayState[index] = !relayState[index];
  digitalWrite(relayPin, relayState[index] ? RELAY_ON : RELAY_OFF);
  server.sendHeader("Location", "/");
  server.send(303);
}

void toggleDoor1() {
  doorOpen[0] = !doorOpen[0];
  door1.write(doorOpen[0] ? 90 : 0);
  server.sendHeader("Location", "/");
  server.send(303);
}

void toggleDoor2() {
  doorOpen[1] = !doorOpen[1];
  door2.write(doorOpen[1] ? 90 : 0);
  server.sendHeader("Location", "/");
  server.send(303);
}

void toggleCurtain() {
  curtainOpen = !curtainOpen;
  curtain.write(curtainOpen ? 90 : 10);
  server.sendHeader("Location", "/");
  server.send(303);
}


void moveForward(){digitalWrite(LEFT_MOTOR_IN1,HIGH);digitalWrite(LEFT_MOTOR_IN2,LOW);digitalWrite(RIGHT_MOTOR_IN1,HIGH);digitalWrite(RIGHT_MOTOR_IN2,LOW);server.sendHeader("Location","/");server.send(303);}
void moveBackward(){digitalWrite(LEFT_MOTOR_IN1,LOW);digitalWrite(LEFT_MOTOR_IN2,HIGH);digitalWrite(RIGHT_MOTOR_IN1,LOW);digitalWrite(RIGHT_MOTOR_IN2,HIGH);server.sendHeader("Location","/");server.send(303);}
void turnLeft(){digitalWrite(LEFT_MOTOR_IN1,LOW);digitalWrite(LEFT_MOTOR_IN2,HIGH);digitalWrite(RIGHT_MOTOR_IN1,HIGH);digitalWrite(RIGHT_MOTOR_IN2,LOW);server.sendHeader("Location","/");server.send(303);}
void turnRight(){digitalWrite(LEFT_MOTOR_IN1,HIGH);digitalWrite(LEFT_MOTOR_IN2,LOW);digitalWrite(RIGHT_MOTOR_IN1,LOW);digitalWrite(RIGHT_MOTOR_IN2,HIGH);server.sendHeader("Location","/");server.send(303);}
void stopMotors(){digitalWrite(LEFT_MOTOR_IN1,LOW);digitalWrite(LEFT_MOTOR_IN2,LOW);digitalWrite(RIGHT_MOTOR_IN1,LOW);digitalWrite(RIGHT_MOTOR_IN2,LOW);server.sendHeader("Location","/");server.send(303);}


void setup() {
  Serial.begin(115200);
  Serial.println("\nSmart Hydra Home Booting...");

  
  pinMode(RELAY_JAI_SHREE_RAM, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_LOWER_LIGHT, OUTPUT);
  pinMode(RELAY_UPPER_LIGHT, OUTPUT);

  
  digitalWrite(RELAY_JAI_SHREE_RAM, RELAY_OFF);
  digitalWrite(RELAY_FAN, RELAY_OFF);
  digitalWrite(RELAY_LOWER_LIGHT, RELAY_OFF);
  digitalWrite(RELAY_UPPER_LIGHT, RELAY_OFF);

  
  relayState[0] = false;
  relayState[1] = false;
  relayState[2] = false;
  relayState[3] = false;


  door1.attach(DOOR1_SERVO);
  door2.attach(DOOR2_SERVO);
  curtain.attach(CURTAIN_SERVO);
  door1.write(0); door2.write(0); curtain.write(10);

  pinMode(LEFT_MOTOR_IN1, OUTPUT);
  pinMode(LEFT_MOTOR_IN2, OUTPUT);
  pinMode(RIGHT_MOTOR_IN1, OUTPUT);
  pinMode(RIGHT_MOTOR_IN2, OUTPUT);
  stopMotors();

 
  SPI.begin(18, 19, 23, SS_PIN);
  mfrc522.PCD_Init();
  Serial.println("RFID Ready...");

 
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("📡 GPS Initialized...");

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  Serial.println("MQ2 Gas Sensor Ready...");

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.print("Connecting to WiFi (non-blocking) ...");
  


  SinricProSwitch &sw1 = SinricPro[SWITCH_ID_1];
  SinricProSwitch &sw2 = SinricPro[SWITCH_ID_2];
  SinricProSwitch &sw3 = SinricPro[SWITCH_ID_3];
  sw1.onPowerState(onPowerState);
  sw2.onPowerState(onPowerState);
  sw3.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.onConnected([](){ Serial.println("Connected to Sinric Pro!"); });
  SinricPro.onDisconnected([](){ Serial.println("Disconnected from Sinric Pro!"); });

 
  server.on("/", handleRoot);
  server.on("/jai", [](){ toggleRelay(0, RELAY_JAI_SHREE_RAM); });
  server.on("/fan", [](){ toggleRelay(1, RELAY_FAN); });
  server.on("/lower", [](){ toggleRelay(2, RELAY_LOWER_LIGHT); });
  server.on("/upper", [](){ toggleRelay(3, RELAY_UPPER_LIGHT); });
  server.on("/door1", toggleDoor1);
  server.on("/door2", toggleDoor2);
  server.on("/curtain", toggleCurtain);
  server.on("/forward", moveForward);
  server.on("/backward", moveBackward);
  server.on("/left", turnLeft);
  server.on("/right", turnRight);
  server.on("/stop", stopMotors);
  server.begin();
  Serial.println("Web Server Running...");
}



const unsigned long WIFI_RECONNECT_INTERVAL = 5000;
unsigned long lastWiFiReconnectAttempt = 0;


const unsigned long IP_PRINT_INTERVAL = 10000; 
unsigned long lastIPPrintMillis = 0;

void printIPContinuously() {
  unsigned long now = millis();
  

  if (WiFi.status() == WL_CONNECTED && (now - lastIPPrintMillis >= IP_PRINT_INTERVAL)) {
    lastIPPrintMillis = now;
    Serial.print("🌐 Current IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void updateBuzzerState() {
  unsigned long now = millis();

  if (buzzerActive) {
   
    if (now - lastBuzzerToggle >= (buzzerOnState ? BUZZER_ON_DURATION : BUZZER_OFF_DURATION)) {
      buzzerOnState = !buzzerOnState;
      lastBuzzerToggle = now;
      digitalWrite(buzzerPin, buzzerOnState ? HIGH : LOW);
    }
  } else if (modWarnActive) {
    
    if (now - lastModWarnToggle >= (modWarnState ? MOD_WARN_BUZZ_DURATION : MOD_WARN_PAUSE)) {
      modWarnState = !modWarnState;
      lastModWarnToggle = now;
      digitalWrite(buzzerPin, modWarnState ? HIGH : LOW);
    }
  } else {
  
    digitalWrite(buzzerPin, LOW);
    buzzerOnState = false;
    modWarnState = false;
  }
}

void loop() {
  
  server.handleClient();
  SinricPro.handle();

  unsigned long now = millis();

  
  if (WiFi.status() != WL_CONNECTED) {
    if (now - lastWiFiReconnectAttempt >= WIFI_RECONNECT_INTERVAL) {
      lastWiFiReconnectAttempt = now;
      Serial.println("WiFi disconnected! Attempting reconnect...");
      WiFi.reconnect();
    }
  } else {
    
  }
  
  
  printIPContinuously(); 

  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("🔹 Card UID: ");
    String scannedUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      if (i < mfrc522.uid.size - 1) Serial.print(":");
      
      scannedUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    bool auth = true;
    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
        auth = false;
        break;
      }
    }

    if (auth) {
      
      lastDoorAccessMessage = "Authorized Card Access: Main Door Open & Lower Floor Light ON";
      lastAccessUID = scannedUID;
      lastAccessColor = "#28A745"; 
 
      
      Serial.println(" Authorized Card Detected → Opening Main Door & Turning ON Lower Floor Light");
      door1.write(90);
      digitalWrite(RELAY_FAN, RELAY_ON);
      relayState[2] = true;
      door1OpenByRFID = true;
      door1OpenUntil = millis() + DOOR1_OPEN_MS;
    } else {
      
      lastDoorAccessMessage = " Unauthorized Card Access: Denied!";
      lastAccessUID = scannedUID;
      lastAccessColor = "#DC3545"; 
      
      
      Serial.println(" Unauthorized Card! Access Denied.");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  
  if (door1OpenByRFID && millis() > door1OpenUntil) {
    door1.write(0);
    door1OpenByRFID = false;
   
    digitalWrite(RELAY_LOWER_LIGHT, RELAY_OFF);
    relayState[2] = false;
    
   
    lastDoorAccessMessage = "🔒 Auto-Close: Door 1 Closed & Light OFF";
    lastAccessUID = "System Timer";
    lastAccessColor = "#6C757D"; 
    
    
    Serial.println("🔒 Door auto-closing & Lower Light OFF");
  }

  
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      Serial.printf("📍 Lat: %.6f | Lng: %.6f\n", latitude, longitude);
    }
  }

 
  if (now - lastMq2ReadMillis >= MQ2_READ_INTERVAL) {
    lastMq2ReadMillis = now;

    int sensorValue = analogRead(mq2Pin);
    float voltage = sensorValue * (3.3 / 4095.0);
    
    float gasLevel = map(sensorValue, 0, 4095, 0, 100);

 
    currentSensorValue = sensorValue;
    currentVoltage = voltage;
    currentGasLevelPercent = gasLevel;

 
    Serial.println("------ MQ2 Gas Sensor Reading ------");
    Serial.printf("Analog Value : %d\n", sensorValue);
    Serial.printf("Voltage      : %.2f V\n", voltage);
    Serial.printf("Gas Level    : %.1f %%\n", gasLevel);

    if (sensorValue > gasThreshold) {
      Serial.println("ALERT: Dangerous Gas Level Detected! Activating Buzzer ");
 
      buzzerActive = true;
      
    } else if (sensorValue > 1450) {
      Serial.println(" Warning: Moderate Gas Level, Please Check Ventilation.");
     
      modWarnActive = true;
      
    } else {
      Serial.println("Air Quality: Safe & Normal.");
      
      buzzerActive = false;
      modWarnActive = false;
      digitalWrite(buzzerPin, LOW);
    }

    Serial.println("------------------------------------\n");
  }

  
  updateBuzzerState();

  
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "OPEN") {
      
      lastDoorAccessMessage = "Face Recognized: Opening Main Door";
      lastAccessUID = "Face Recognition";
      lastAccessColor = "#17A2B8"; 
      
      
      Serial.println("Face recognized — Opening Main Door ...");
      door1.write(90);
    } else if (command == "CLOSE") {
      
      lastDoorAccessMessage = "Face Session Ended: Closing Main Door";
      lastAccessUID = "Face Recognition";
      lastAccessColor = "#6C757D"; 
    
      
      Serial.println("Face session ended — Closing Main Door ...");
      door1.write(0);
    } else {
      Serial.println("Unknown command received");
    }
  }

  
  yield();
}