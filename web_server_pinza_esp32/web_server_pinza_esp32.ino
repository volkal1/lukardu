#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <WebServer.h>
#include <WiFi.h>

#define LED 2

// Datos para conectar al wifi la placa
const char *SSID = "nombre_wifi";
const char *PWD = "pass_wifi";

WebServer server(80);

StaticJsonDocument<250> jsonDocument;
char buffer[250];

Servo servoPinza;
int anguloPinza = 0;
const int pinPinza = 13;

void setup_routing() {  // esta funcion se llama al principio, y declara las
                        // direcciones http para controlar los motores
  server.on("/pinza", HTTP_POST, handlePinza);

  server.begin();
}

void handlePinza() {  // estos metodos se encargan de mover cada parte, se
                      // tienen que mapear con las direcciones del routing
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  togglePinza();

  char message[50];

  sprintf(message, "{ \"servo1\" : %d }", anguloPinza);
  server.send(200, "application/json", message);
}

void setupPinza() {
  Serial.print("setup");
  servoPinza.attach(pinPinza);
  servoPinza.write(anguloPinza);
}

void togglePinza() {
  if (anguloPinza == 0) {
    Serial.println("open pinza");
    anguloPinza = 180;
  } else {
    Serial.println("close pinza");
    anguloPinza = 0;
  }
  servoPinza.write(anguloPinza);
}

void setup() {  // este metodo es el mismo que el del arduino
  pinMode(LED, OUTPUT);
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

void loop() {  // este metodo es el mismo que el arduino
  server.handleClient();
}

// void create_json(char *tag, float value, char *unit) {
//   jsonDocument.clear();
//   jsonDocument["type"] = tag;
//   jsonDocument["value"] = value;
//   jsonDocument["unit"] = unit;
//   serializeJson(jsonDocument, buffer);
// }

// void add_json_object(char *tag, float value, char *unit) {
//   JsonObject obj = jsonDocument.createNestedObject();
//   obj["type"] = tag;
//   obj["value"] = value;
//   obj["unit"] = unit;
// }