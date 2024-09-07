#include <Arduino.h>
#include "globals.hpp"
#include "commands.hpp"
#include "connection.hpp"

bool autoConnection = true;

void setup() {
  Serial.begin(9600);
  Commands::initPins();
  Connection::conectarWiFi();
  Connection::ejecutarMQTT();
} // setup().

void loop() {
  if (!Connection::client.connected() && autoConnection) {
    Connection::reconnect();
  }
  
  Connection::client.loop();
} // loop().