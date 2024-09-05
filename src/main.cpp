#include <Arduino.h>
#include <ESP32Servo.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial BT; // Objeto Bluetooth.

static const int servoPin = 17;   // Lineal.
static const int servoPin2 = 16;  // Angular.
static const int servoPin3 = 4;

// Posiciones del servomecanismo.
int posGradoMax = 100;
int posGradoIni = 40;
int posDegrees1 = 130;
int posDegrees2 = 150;

// Posiciones de los servomotores.
int pos = 100; // Abierta.
int pos2 = 90; // Cerrada maxima.

// Posiciones del servomotor pequeño.
int posIni = 0;

Servo servo1;
Servo servo2;
Servo servoPeq3;

const int pinPWMA = 32;
const int pinAIN2 = 25;
const int pinAIN1 = 26;
const int pinSTBY = 33;
const int pinPWMB = 13;
const int pinBIN1 = 27;   //14;
const int pinBIN2 = 14;  //13;

const int waitTime = 2000; // Espera entre fases.

// Control de distancias con relacion al tiempo min 50 max 256.
const int speed = 50; // Velocidad de giro.
const int speed2 = 250;

const int pinMotorA[3] = {pinPWMA, pinAIN2, pinAIN1};
const int pinMotorB[3] = {pinPWMB, pinBIN2, pinBIN1};

enum moveDirection {
  forward, // Giro a la derecha.
  backward // Giro a la izquierda.
}; // moveDirection.

enum turnDirection {
  clockwise,
  counterClockwise
}; // turnDirection.

void moveMotorForward(const int pinMotor[3], int speed);
void moveMotorBackward(const int pinMotor[3], int speed);
void move(int direction, int speed, int motor);
void fullStop();
void enableMotors();
void disableMotors();
void stopMotor(const int pinMotor[3]);
void pinSerAng(int flag);
void pinSerLineal(int flag);

void setup() {
  Serial.begin(9600);

  // Nombre del dispositivo Bluetooth y en modo esclavo.
  BT.begin("ESP32_LED_Control 001 ");

  Serial.println("\n");
  Serial.println("El dispositivo Bluetooth está listo para emparejarse \n");

  servo1.attach(servoPin);
  servo2.attach(servoPin2);
  servoPeq3.attach(servoPin3);

  servo1.write(pos);
  servo2.write(posGradoIni);
  servoPeq3.write(posIni);

  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinSTBY, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinPWMB, OUTPUT);   
} // setup().

void loop() {
  // Control de servomotores.
  if (BT.available()) { // Comprueba si recibimos algo por Bluetooth.
    int incoming = BT.read(); // Lee lo que recibimos.

    Serial.print(" -> ");
    Serial.print(incoming);

    if (incoming == 49) { // 1.
      pinSerAng(1);
    }

    if (incoming == 50) { // 2.
      pinSerAng(2);
    }

    if (incoming == 51) { // 3.
      pinSerLineal(1);
    }

    if (incoming == 52) { // 4.
      pinSerLineal(2);
    }

    if (incoming == 53) { // 5.
      enableMotors();
      move(forward, speed, 1);
      delay(100);
      fullStop();
    }

    if (incoming == 54) { // 6.
      enableMotors();
      move(backward, speed, 1);
      delay(100);
      fullStop();
    }

    if (incoming == 55) { // 7.
      enableMotors();
      digitalWrite(pinMotorB[1], HIGH);
      digitalWrite(pinMotorB[2], LOW);
      analogWrite(pinMotorB[0], speed2);
      delay(30);
      fullStop();
    }

    if (incoming == 56) { // 8.
      enableMotors();
      digitalWrite(pinMotorB[1], LOW);
      digitalWrite(pinMotorB[2], HIGH);
      analogWrite(pinMotorB[0], speed2);
      delay(30);
      fullStop();
    }

    else{
       fullStop();
    }
  } // if().
} // loop().

void enableMotors() {
  digitalWrite(pinSTBY, HIGH);
} // enableMotors().

// Funciones que controlan los motores.
void moveMotorForward(const int pinMotor[3], int speed) {
  digitalWrite(pinMotor[1], HIGH);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], speed);
} // moveMotorForward().

void moveMotorBackward(const int pinMotor[3], int speed) {
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], HIGH);
  analogWrite(pinMotor[0], speed);
} // moveMotorBackward().

void move(int direction, int speed, int motor) {
  if (direction == forward && motor == 1) {
    moveMotorForward(pinMotorA, speed);
  }

  if (direction == forward && motor == 2) { // Analizar.
    moveMotorForward(pinMotorB, speed);
  }

  if (direction == backward && motor == 1) {
    moveMotorBackward(pinMotorA, speed);
  }

  if (direction == backward && motor == 2) {
    moveMotorBackward(pinMotorB, speed);
  }
} // move().

void disableMotors() {
  digitalWrite(pinSTBY, LOW);
} // disableMotors().

void stopMotor(const int pinMotor[3]) {
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], 0);
} // stopMotor().

void fullStop() {
  disableMotors();
  stopMotor(pinMotorA);
  stopMotor(pinMotorB);
} // fullStop().

// Funciones que controlan los servomotores angular y lineal.
void pinSerAng(int flag) {
  if (flag == 2) {
    servo2.write(posGradoIni);
    Serial.println("Pinza angular abierta.");
    delay(waitTime);
  }

  if (flag == 1) {
    servo2.write(posGradoMax);
    Serial.println("Pinza angular cerrada.");
    delay(waitTime);
  }
} // pinSerAng().

void pinSerLineal(int flag) {
  if (flag == 2) {
    servo1.write(pos2);
    Serial.println("Pinza lineal cerrada.");
    delay(waitTime);
  }

  if (flag == 1) {
    servo1.write(pos);
    Serial.println("Pinza lineal abierta.");
    delay(waitTime);
  }
} // pinSerLineal().