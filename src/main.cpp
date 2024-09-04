#include <Arduino.h>
#include "commands.hpp"
#include "connection.hpp"

void setup() {
  Serial.begin(9600);
  Commands::initPins();
  Connection::conectarWiFi();
  Connection::ejecutarMQTT();
} // setup().

void loop() {
  if (!Connection::client.connected()) {
    Connection::reconnect();
  }
  
  Connection::client.loop();
} // loop().