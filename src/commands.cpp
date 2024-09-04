// Implementación de la clase Commands para manejar comandos JSON y controlar un LED.

// Encabezado.
#include "commands.hpp"

// Declaración de variables estáticas.
const int Commands::COMMANDS[CANTIDAD_COMMANDS] = {0, 1, 2, 3, 4};
const int Commands::PIN_MOTOR_A[3] = {PIN_PWMA, PIN_AIN2, PIN_AIN1};
const int Commands::PIN_MOTOR_B[3] = {PIN_PWMB, PIN_BIN2, PIN_BIN1};

Servo Commands::servo1;
Servo Commands::servo2;

/**
 * @brief Constructor de la clase Commands.
 * 
 * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
 */
Commands::Commands() {
    // No se requiere inicialización especial en este caso.
} // Commands().

/**
 * @brief Inicializa el pin del LED.
 * 
 * Configura el pin 2 para el LED.
 */
void Commands::initPins() {
    servo1.attach(SERVO_PIN_1);
    servo2.attach(SERVO_PIN_2);

    pinMode(PIN_AIN2, OUTPUT);
    pinMode(PIN_AIN1, OUTPUT);
    pinMode(PIN_PWMA, OUTPUT);
    pinMode(PIN_STBY, OUTPUT);
    pinMode(PIN_BIN2, OUTPUT);
    pinMode(PIN_BIN1, OUTPUT);
    pinMode(PIN_PWMB, OUTPUT);
} // initPins().

// Funciones que controlan los motores.
void Commands::moveMotorForward(const int PIN_MOTOR[3], uint8_t speed) {
    digitalWrite(PIN_MOTOR[1], HIGH);
    digitalWrite(PIN_MOTOR[2], LOW);
    analogWrite(PIN_MOTOR[0], speed);
} // moveMotorForward().

void Commands::moveMotorBackward(const int PIN_MOTOR[3], uint8_t speed) {
    digitalWrite(PIN_MOTOR[1], LOW);
    digitalWrite(PIN_MOTOR[2], HIGH);
    analogWrite(PIN_MOTOR[0], speed);
} // moveMotorBackward().

void Commands::move(int direction, uint8_t speed, uint8_t motor) {
    if (direction == forward && motor == 1) {
        moveMotorForward(PIN_MOTOR_A, speed);
    }

    if (direction == forward && motor == 2) {
        moveMotorForward(PIN_MOTOR_B, speed);
    }

    if (direction == backward && motor == 1) {
        moveMotorBackward(PIN_MOTOR_A, speed);
    }

    if (direction == backward && motor == 2) {
        moveMotorBackward(PIN_MOTOR_B, speed);
    }
} // move().

void Commands::disableMotors() {
    digitalWrite(PIN_STBY, LOW);
} // disableMotors().

void Commands::stopMotor(const int PIN_MOTOR[3]) {
    digitalWrite(PIN_MOTOR[1], LOW);
    digitalWrite(PIN_MOTOR[2], LOW);
    analogWrite(PIN_MOTOR[0], 0);
} // stopMotor().

void Commands::fullStop() {
    disableMotors();
    stopMotor(PIN_MOTOR_A);
    stopMotor(PIN_MOTOR_B);
} // fullStop().

void Commands::enableMotors() {
    digitalWrite(PIN_STBY, HIGH);
} // enableMotors().

// Funciones que controlan los servomotores angular y lineal.
String Commands::pinServoAngular(uint8_t flag) {
    if (flag == 2) {
        servo2.write(POSICION_GRADO_INICIAL);
        delay(WAIT_TIME);
        return "Pinza angular abierta.";
    }

    if (flag == 1) {
        servo2.write(POSICION_GRADO_MAX);
        delay(WAIT_TIME);
        return "Pinza angular cerrada.";
    }
} // pinSerAng().

String Commands::pinServoLineal(uint8_t flag) {
    if (flag == 2) {
        servo1.write(POSICION_2);
        delay(WAIT_TIME);
        return "Pinza lineal abierta.";
    }

    if (flag == 1) {
        servo1.write(POSICION_1);
        delay(WAIT_TIME);
        return "Pinza lineal cerrada.";
    }
} // pinServoLineal().

/**
 * @brief Valida si el payload JSON contiene un comando válido.
 *
 * Esta función deserializa un string JSON y verifica si contiene un campo llamado "command"
 * que es un número entero válido y está presente en un array predefinido de comandos.
 *
 * @param payload El string JSON que se desea validar.
 * @return true si el JSON contiene un comando válido, false en caso contrario.
 */
bool Commands::validarCommand(String payload) {
    StatusCode::jsonDoc.clear(); // Se limpia el objeto JSON anterior.
    deserializeJson(StatusCode::jsonDoc, payload);

    return StatusCode::jsonDoc["command"].is<int>() &&
        StatusCode::jsonDoc["command"] >= 0 &&
        Utils::containsInArray(
            COMMANDS,
            CANTIDAD_COMMANDS,
            StatusCode::jsonDoc["command"]
        );
} // validarCommand().

/**
 * @brief Obtiene el valor del campo "command" del objeto JSON.
 *
 * Esta función deserializa un String JSON y devuelve el valor del campo "command"
 * como un entero de 8 bits sin signo (uint8_t).
 *
 * @param payload El String JSON del cual se desea obtener el valor del campo "command".
 * @return El valor del campo "command" como un entero de 8 bits sin signo (uint8_t).
 */
uint8_t Commands::obtenerCommand(String payload) {
    StatusCode::jsonDoc.clear(); // Se limpia el objeto JSON anterior.

    deserializeJson(StatusCode::jsonDoc, payload);
    return StatusCode::jsonDoc["command"];
} // obtenerCommand().

/**
 * @brief Procesa un comando para controlar un LED y devuelve un mensaje descriptivo.
 *
 * Esta función recibe un comando en forma de un entero de 8 bits sin signo (uint8_t)
 * y ejecuta una acción correspondiente para controlar el estado de un pin.
 * Para cualquier otro comando, devuelve una cadena vacía.
 *
 * @param command El comando que se va a procesar.
 * @return Un mensaje descriptivo del estado del LED después de procesar el comando.
 */
String Commands::procesarCommand(uint8_t command) {
    switch (command){
        case 0:
            return "Close connection.";

        case 1:
            return pinServoAngular(1);
        
        case 2:
            return pinServoAngular(2);
        
        case 3:
            return pinServoLineal(1);
        
        case 4:
            return pinServoLineal(2);
        
        case 5:
            enableMotors();
            move(forward, SPEED_1, 1);
            delay(200);
            disableMotors();
            delay(WAIT_TIME);
            return "Avance.";
        
        case 6:
            enableMotors();
            move(backward, SPEED_1, 1);
            delay(200);
            disableMotors();
            delay(WAIT_TIME);
            return "Retroceso.";
        
        case 7:
            enableMotors();
            move(forward, SPEED_2, 2);
            delay(10);
            disableMotors();
            return "Giro a la derecha.";
        
        case 8:
            enableMotors();
            move(backward, SPEED_2, 2);
            delay(10);
            disableMotors();
            return "Giro a la izquierda.";
        
        default:
            fullStop();
            return "Detenido.";
    } // switch().
} // procesarCommand().