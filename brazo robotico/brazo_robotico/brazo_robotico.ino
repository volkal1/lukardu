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
#include <Servo.h>

// Pines analógicos conectados a los potenciómetros
const int potPinX = A1; // Potenciómetro para el eje X
const int potPinY = A3; // Potenciómetro para el eje Y

// Objetos de los servos
Servo servoX; // Servo para el eje X
Servo servoY; // Servo para el eje Y

// Velocidad máxima de movimiento de los servos (ajusta según tu preferencia)
const int maxSpeed = 10; // Mayor valor = movimiento más lento

void setup() {
   pinMode(switchPin, INPUT);
  pinza.attach(servoPin);
  pinza.write(servoAngle); // Inicializar el servo en el ángulo inicial
  servoX.attach(7);
  servoY.attach(9);
}
  void loop() {
  // Leer los valores de los potenciómetros (0-1023)
  int potValueX = analogRead(potPinX);
  int potValueY = analogRead(potPinY);

  // Mapear los valores leídos a un rango de ángulos (0-180)
  int angleX = map(potValueX, 0, 1023, 60, 160);
  int angleY = map(potValueY, 0, 1023, 60, 160);

  // Calcular la diferencia entre la posición actual y la deseada
  int diffX = angleX - servoX.read();
  int diffY = angleY - servoY.read();

  // Mover los servos con una velocidad limitada
  if (abs(diffX) > maxSpeed) {
    if (diffX > 0) {
      servoX.write(servoX.read() + maxSpeed);
    } else {
      servoX.write(servoX.read() - maxSpeed);
    }
  } else {
    servoX.write(angleX);
  }

  if (abs(diffY) > maxSpeed) {
    if (diffY > 0) {
      servoY.write(servoY.read() + maxSpeed);
    } else {
      servoY.write(servoY.read() - maxSpeed);
    }
  } else {
    servoY.write(angleY);
  }

  // Pequeña pausa para evitar movimientos bruscos
  delay(15);
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

