#include <Servo.h>

// Pin digital conectado al botón switch
const int switchPin = 0;

// Pin al que está conectado el servo
const int servoPin = 5;

Servo pinza;

// Variables para el estado del botón y el ángulo del servo
int buttonState = 0;
int lastButtonState = 0;
int servoAngle = 100; // Ángulo inicial del servo
bool movingTo53 = false; // Indica si el servo está en movimiento hacia 53

void setup() {
  pinMode(switchPin, INPUT);
  pinza.attach(servoPin);
  pinza.write(servoAngle); // Inicializar el servo en el ángulo inicial
}

void loop() {
  buttonState = digitalRead(switchPin);

  // Cambiar el estado del servo si se presiona el botón
  if (buttonState == HIGH && lastButtonState == LOW) {
    if (!movingTo53) {
      servoAngle = 53;
      movingTo53 = true;
    } else {
      servoAngle = 100;
      movingTo53 = false;
    }
    pinza.write(servoAngle);
    delay(200); // Pequeño retraso para evitar múltiples cambios rápidos
  }

  lastButtonState = buttonState;
}
