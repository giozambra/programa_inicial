// Implementación de la clase Commands para manejar comandos JSON y controlar un LED.

// Encabezado.
#include "commands.hpp"

// Declaración de variables estáticas.
const int Commands::COMMANDS[CANTIDAD_COMMANDS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const int Commands::PIN_MOTOR_A[3] = {PIN_PWMA, PIN_AIN2, PIN_AIN1};
const int Commands::PIN_MOTOR_B[3] = {PIN_PWMB, PIN_BIN2, PIN_BIN1};
const int Commands::PIN_MOTOR_C[3] = {PIN_PWMC, PIN_CIN2, PIN_CIN1};

uint8_t Commands::isServoAngularOpen = 1;
uint8_t Commands::isServoLinearOpen = 1;
uint8_t Commands::isServoLinearCathOpen = 1;

Servo Commands::servo1;
Servo Commands::servo2;
Servo Commands::servo3; // Pequeño.

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
    servo3.attach(SERVO_PIN_3);

    servo1.write(POSICION_1);
    servo2.write(POSICION_GRADO_INICIAL);
    servo3.write(POSICION_1);

    pinMode(PIN_AIN2, OUTPUT);
    pinMode(PIN_AIN1, OUTPUT);
    pinMode(PIN_PWMA, OUTPUT);
    pinMode(PIN_STBY, OUTPUT);
    pinMode(PIN_BIN2, OUTPUT);
    pinMode(PIN_BIN1, OUTPUT);
    pinMode(PIN_PWMB, OUTPUT);
} // initPins().

// Funciones que controlan los motores.
void Commands::enableMotors() {
    digitalWrite(PIN_STBY, HIGH);
} // enableMotors().

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

    if (direction == forward && motor == 3) {
        moveMotorForward(PIN_MOTOR_C, speed);
    }

    if (direction == backward && motor == 1) {
        moveMotorBackward(PIN_MOTOR_A, speed);
    }

    if (direction == backward && motor == 2) {
        moveMotorBackward(PIN_MOTOR_B, speed);
    }

    if (direction == backward && motor == 3) {
        moveMotorBackward(PIN_MOTOR_C, speed);
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
    stopMotor(PIN_MOTOR_C);
} // fullStop().

// Funciones que controlan los servomotores angular y lineal.
String Commands::pinServoAngular(uint8_t flag) {
    if (flag == 0) {
        servo2.write(POSICION_GRADO_MAX);
        isServoAngularOpen = 0;
        return "Pinza angular cerrada.";
    }

    if (flag == 1) {
        servo2.write(POSICION_GRADO_INICIAL);
        isServoAngularOpen = 1;
        return "Pinza angular abierta.";
    }
} // pinServoAngular().

String Commands::pinServoLinear(uint8_t flag) {
    if (flag == 0) {
        servo1.write(POSICION_2);
        isServoLinearOpen = 0;
        return "Pinza lineal cerrada.";
    }

    if (flag == 1) {
        servo1.write(POSICION_1);
        isServoLinearOpen = 1;
        return "Pinza lineal abierta.";
    }
} // pinServoLinear().

String Commands::pinServoLinearCath(uint8_t flag) {
    if (flag == 0) {
        servo3.write(POSICION_2);
        isServoLinearCathOpen = 0;
        return "Pinza lineal catéter cerrada.";
    }

    if (flag == 1) {
        servo3.write(POSICION_1);
        isServoLinearCathOpen = 1;
        return "Pinza lineal catéter abierta.";
    }
} // pinServoLinearCath().

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
    Serial.println(payload);
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
            autoConnection = false;
            return "Close connection.";

        case 1:
            servo1.write(POSICION_1);
            servo2.write(POSICION_GRADO_INICIAL);
            servo3.write(POSICION_1);

            isServoLinearOpen = 1;
            isServoAngularOpen = 1;
            isServoLinearCathOpen = 1;

            fullStop();
            return "Inicio.";
        
        case 2:
            servo1.write(POSICION_1);
            servo2.write(POSICION_GRADO_INICIAL);
            servo3.write(POSICION_1);

            isServoLinearOpen = 1;
            isServoAngularOpen = 1;
            isServoLinearCathOpen = 1;

            fullStop();
            return "Paro de emergencia.";
        
        // Control de motoreductores.
        // Control del motorreductor lineal.
        case 3:
            // Avance guía.
            if (!isServoAngularOpen) {
                pinServoAngular(1); // Abre servomotor angular.
                delay(100);
            }

            if (isServoLinearOpen) {
                pinServoLinear(0); // Cierra servomotor lineal.
                delay(100);
            }
            
            enableMotors();
            move(forward, SPEED_2, 1);
            delay(500);
            fullStop();
            return "Avance de guía.";
        
        case 4:
            // Retroceso guía.
            if (!isServoAngularOpen) {
                pinServoAngular(1); // Abre servomotor angular.
                delay(100);
            }

            if (isServoLinearOpen) {
                pinServoLinear(0); // Cierra servomotor lineal.
                delay(100);
            }
            
            enableMotors();
            move(backward, SPEED_2, 1);
            delay(500);
            fullStop();
            return "Retroceso de guía.";
        
        // Control del motorreductor angular.
        case 5:
            // Giro de guía a la derecha.
            if (isServoAngularOpen) {
                pinServoAngular(0); // Cierra servomotor angular.
                delay(100);
            }

            if (!isServoLinearOpen) {
                pinServoLinear(1); // Abre servomotor lineal.
                delay(100);
            }
            
            enableMotors();
            move(forward, SPEED_2, 2);
            delay(100);
            fullStop();
            return "Giro de guía a la derecha.";
        
        case 6:
            // Giro de guía a la izquierda.
            if (isServoAngularOpen) {
                pinServoAngular(0); // Cierra servomotor angular.
                delay(100);
            }

            if (!isServoLinearOpen) {
                pinServoLinear(1); // Abre servomotor lineal.
                delay(100);
            }

            enableMotors();
            move(backward, SPEED_2, 2);
            delay(100);
            fullStop();
            return "Giro de guía a la izquierda.";
        
        // Control del motor lineal catéter.
        case 7:
            // Avance catéter.
            if (isServoLinearOpen) {
                pinServoLinear(0); // Cierra servomotor lineal.
                delay(100);
            }

            if (isServoLinearCathOpen) {
                pinServoLinearCath(0); // Cierra servomotor lineal catéter.
                delay(100);
            }

            if (!isServoAngularOpen) {
                pinServoAngular(1); // Abre servomotor angular.
                delay(100);
            }

            enableMotors();
            move(forward, SPEED_2, 3);
            delay(300);
            fullStop();
            return "Avance de catéter.";
        
        case 8:
            // Retroceso catéter.
            if (isServoLinearOpen) {
                pinServoLinear(0); // Cierra servomotor lineal.
                delay(100);
            }

            if (isServoLinearCathOpen) {
                pinServoLinearCath(0); // Cierra servomotor lineal catéter.
                delay(100);
            }

            if (!isServoAngularOpen) {
                pinServoAngular(1); // Abre servomotor angular.
                delay(100);
            }

            enableMotors();
            move(backward, SPEED_2, 3);
            delay(300);
            fullStop();
            return "Retroceso de catéter.";
        
        default:
            fullStop();
            return "Detenido.";
    } // switch().
} // procesarCommand().