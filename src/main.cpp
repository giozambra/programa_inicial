#include <Arduino.h>
#include <ESP32Servo.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial BT; // Objeto Bluetooth

static const int servoPin = 17;   //  Servo lineal
static const int servoPin2 = 16;  //  Servo de angular
static const int servoPin3 = 4;
// POSICIONES DEL SERVOMECANISMO
int posGradoMax = 100;
int posGradoIni = 40;
int posDegrees1 = 130;
int posDegrees2 = 150;
int a = 2;
// posiciones de los servomotores 
int pos = 100;    // posicion abierta
int pos2 = 90;    // posicion cerrada maxima 
// posiciones del servomotor pequenos
int posIni = 0;

const int ini = 0;
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

const int waitTime = 2000;  //espera entre fases
const int speed = 50;    //velocidad de giro con este podria, controlar las distancias con relacion del tiempo min 50 max 256
const int speed2 = 250;
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN2, pinBIN1 };

enum moveDirection {
  forward,  // giro a la derecha
  backward // giro a la izquierda
};

enum turnDirection {
  clockwise,
  counterClockwise
};

void move(int direction, int speed, int motor);
void turn(int direction, int speed);
void fullStop();
void enableMotors();
void disableMotors();
void moveMotorForward(const int pinMotor[3], int speed);
void moveMotorBackward(const int pinMotor[3], int speed);
void stopMotor(const int pinMotor[3]);
void pinSerAng(int flag);
void pinSerLineal(int flag);




void controSer(int flag) {

  if (flag == 1) {

    for (int posDegrees = posDegrees2; posDegrees >= posDegrees1; posDegrees--) {
      servo1.write(posDegrees);
      Serial.println(posDegrees);
      delay(300);
    }
    for (int posDegrees = posDegrees1; posDegrees <= posDegrees2; posDegrees++) {
      servo1.write(posDegrees);
      Serial.println(posDegrees);
      delay(300);
    }
  }
  if (flag == 2) {

    for (int posDegrees = posDegrees2; posDegrees >= posDegrees1; posDegrees--) {
      servo1.write(posDegrees);
      Serial.println(posDegrees);
      delay(300);
    }
  }
};
void setup() {

  Serial.begin(9600);
  BT.begin("ESP32_LED_Control 001 "); // Nombre de su dispositivo Bluetooth y en modo esclavo
  Serial.println("\n");
  Serial.println("El dispositivo Bluetooth está listo para emparejarse \n");
  servo1.attach(servoPin);
  servo2.attach(servoPin2);
  servoPeq3.attach(servoPin3);
  servo2.write(posGradoIni);
  servo1.write(pos);
  servoPeq3.write(posIni);
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinSTBY, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinPWMB, OUTPUT);   
}

void loop() {

  servoPeq3.write(0);
  Serial.println("0");
  delay(2000);
  servoPeq3.write(90);
  Serial.println("90");
  delay(2000);
  servoPeq3.write(180);
  Serial.println("180");
  delay(2000); 
/// CONTROL DE SERVOMOTORES ///
  
  if (BT.available()){ // Compruebe si recibimos algo de Bluetooth   
    int incoming = BT.read(); // Lee lo que recibimos
    Serial.print(" -> ");
    Serial.print(incoming);
    if (incoming == 49){ //1
      pinSerAng(1);
    }
    if (incoming == 50){ //2
      pinSerAng(2);
    }
    if (incoming == 51){ //3
      pinSerLineal(1);
    }
    if (incoming == 52){ //4
      pinSerLineal(2);
    }
    if (incoming == 53){ //5
      enableMotors();
      move(forward,speed,1);
      delay(100);
      fullStop();
    }
    if (incoming == 54){ //6
      enableMotors();
      move(backward,speed,1);
      delay(100);
      fullStop();
    }
    if (incoming == 55){ //7
      enableMotors();
      digitalWrite(pinMotorB[1], HIGH);
      digitalWrite(pinMotorB[2], LOW);
      analogWrite(pinMotorB[0], speed2);
      delay(30);
      fullStop();
      //delay(waitTime);
    }
    if (incoming == 56){ //8
      enableMotors();
      digitalWrite(pinMotorB[1],LOW );
      digitalWrite(pinMotorB[2], HIGH);
      analogWrite(pinMotorB[0], speed2);
      delay(30);
      fullStop();
      //delay(waitTime);
    }
    else{
       fullStop();
    }
  } 
}
void move(int direction, int speed, int motor){
  
  if (direction == forward && motor == 1)
  {
    moveMotorForward(pinMotorA, speed);
  }
  if (direction == forward && motor == 2)     // analizar
  {
    moveMotorForward(pinMotorB, speed);
  }
  if (direction == backward && motor == 1)
  {
    moveMotorBackward(pinMotorA, speed);
  }
  if (direction == backward && motor == 2)
  {
    moveMotorBackward(pinMotorB, speed);
  }
}
void turn(int direction, int speed, int motor)
{
  if (direction == forward && motor == 1)
  {
    moveMotorForward(pinMotorA, speed);
  }
  if (direction == forward && motor == 2)
  {
    moveMotorBackward(pinMotorB, speed);
  }
  if (direction == backward && motor == 1)
  {
    moveMotorBackward(pinMotorA, speed);
  }
  if (direction == backward && motor == 2)
  {
    moveMotorForward(pinMotorB, speed);
  }
}
void fullStop()
{
  disableMotors();
  stopMotor(pinMotorA);
  stopMotor(pinMotorB);
}
//Funciones que controlan los motores
void moveMotorForward(const int pinMotor[3], int speed)
{
  digitalWrite(pinMotor[1], HIGH);
  digitalWrite(pinMotor[2], LOW);

  analogWrite(pinMotor[0], speed);
}
void moveMotorBackward(const int pinMotor[3], int speed)
{
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], HIGH);

  analogWrite(pinMotor[0], speed);
}
void stopMotor(const int pinMotor[3])
{
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], LOW);

  analogWrite(pinMotor[0], 0);
}
void enableMotors()
{
  digitalWrite(pinSTBY, HIGH);
}
void disableMotors()
{
  digitalWrite(pinSTBY, LOW);
}
// Funciones que controlan los servomotores angular y lineal
void pinSerAng(int flag){
  if (flag == 2){
    servo2.write(posGradoIni);
    Serial.println("Pinza abierta Angular");
    delay(2000);
  }
  if (flag == 1){
    servo2.write(posGradoMax);
    Serial.println("Pinza cerrada Angular");
    delay(2000);
  }
}
void pinSerLineal(int flag){
  if (flag == 2){
    /*for (int posDegrees = pos; posDegrees <= pos2; posDegrees++) {
      servo1.write(posDegrees);
      Serial.println(posDegrees);
      delay(300);
    }*/
    servo1.write(pos2);
    Serial.println("Pinza cerrada lineal");
    delay(2000);
  }
  if (flag == 1){
    servo1.write(pos);
    Serial.println("Pinza abierta Lineal");

    delay(2000);
  }
}
