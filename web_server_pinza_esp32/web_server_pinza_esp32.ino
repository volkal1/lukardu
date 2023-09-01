#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#define LED 2

const char *SSID = "nombre_wifi";
const char *PWD = "pass_wifi";

// Setting PWM frequency, channels and bit resolution
const bool flag = 8;

WebServer server(80);

StaticJsonDocument<250> jsonDocument;
char buffer[250];

Servo myservo;
int servoPos = 0;
int buttonState = 0;
int lastButtonState = 0;

void setup_routing() {     
  server.on("/led", HTTP_POST, handlePost);
          
  server.begin();    
}
 
void create_json(char *tag, float value, char *unit) {  
  jsonDocument.clear();  
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}
 
void add_json_object(char *tag, float value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit; 
}

void handlePost() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  bool state = jsonDocument["ligth"];
  
  togglePinza();
  char message[50];

  sprintf(message, "{ \"servo1\" : %d }", servoPos);
  server.send(200, "application/json", message);
}

void setupPinza() {
  Serial.print("setup");
  myservo.attach(13);
  myservo.write(servoPos);
}

void togglePinza() {
  if (servoPos == 0) {
    Serial.println("open pinza");
    servoPos = 180;
  } else {
    Serial.println("close pinza");
    servoPos = 0;
  }
  myservo.write(servoPos);
}

void setup() {     
  pinMode(LED,OUTPUT);
  Serial.begin(115200); 

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());  
  setupPinza();
  setup_routing();     
   
}    
       
void loop() {    
  server.handleClient();     
}