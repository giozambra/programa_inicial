#ifndef COMMANDS_HPP
#define COMMANDS_HPP

// Librerías necesarias.
#include <Arduino.h>
#include <ESP32Servo.h>
#include "utils.hpp"
#include "statusCode.hpp"

/**
 * @class Commands
 * @brief Clase para manejar comandos JSON y controlar un LED.
 */
class Commands {
    private:
        static const uint8_t CANTIDAD_COMMANDS = 5;
        static const int COMMANDS[CANTIDAD_COMMANDS];

        static const uint8_t SERVO_PIN_1 = 17;
        static const uint8_t SERVO_PIN_2 = 16;

        static const uint8_t PIN_PWMA = 32;
        static const uint8_t PIN_AIN2 = 25;
        static const uint8_t PIN_AIN1 = 26;
        static const uint8_t PIN_STBY = 33;
        static const uint8_t PIN_PWMB = 12;
        static const uint8_t PIN_BIN1 = 27;
        static const uint8_t PIN_BIN2 = 14;

        static const uint8_t POSICION_GRADO_MAX = 110;
        static const uint8_t POSICION_GRADO_INICIAL = 50;
        static const uint8_t POSICION_GRADOS_1 = 130;
        static const uint8_t POSICION_GRADOS_2 = 150;

        // Posición de los servomotores.
        static const uint8_t POSICION_1 = 110; // Abierta.
        static const uint8_t POSICION_2 = 75; // Cerrada máxima.

        static const uint16_t WAIT_TIME = 2000; // Espera entre fases.
        static const uint8_t SPEED_1 = 150; // Velocidad de giro.
        static const uint8_t SPEED_2 = 50;
        static const int PIN_MOTOR_A[3];
        static const int PIN_MOTOR_B[3];

        static Servo servo1;
        static Servo servo2;

        enum moveDirection {
            forward,  // Giro a la derecha.
            backward // Giro a la izquierda.
        }; // moveDirection.

        enum turnDirection {
            clockwise,
            counterClockwise
        }; // turnDirection.

        // Funciones que controlan los motores.
        static void moveMotorForward(const int pinMotor[3], uint8_t speed);

        static void moveMotorBackward(const int pinMotor[3], uint8_t speed);

        static void move(int direction, uint8_t speed, uint8_t motor);

        static void disableMotors();

        static void stopMotor(const int pinMotor[3]);

        static void fullStop();

        static void enableMotors();

        // Funciones que controlan los servomotores angular y lineal.
        static String pinServoAngular(uint8_t flag);

        static String pinServoLineal(uint8_t flag);

    public:
        /**
         * @brief Constructor de la clase Utils.
         * 
         * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
         */
        Commands();

        /**
         * @brief Inicializa el pin del LED.
         * 
         * Configura el pin 2 para el LED.
         */
        static void initPins();

        /**
         * @brief Valida si el payload JSON contiene un comando válido.
         *
         * Esta función deserializa un string JSON y verifica si contiene un campo llamado "command"
         * que es un número entero válido y está presente en un array predefinido de comandos.
         *
         * @param payload El string JSON que se desea validar.
         * @return true si el JSON contiene un comando válido, false en caso contrario.
         */
        static bool validarCommand(String payload);

        /**
         * @brief Obtiene el valor del campo "command" del objeto JSON.
         *
         * Esta función deserializa un String JSON y devuelve el valor del campo "command"
         * como un entero de 8 bits sin signo (uint8_t).
         *
         * @param payload El String JSON del cual se desea obtener el valor del campo "command".
         * @return El valor del campo "command" como un entero de 8 bits sin signo (uint8_t).
         */
        static uint8_t obtenerCommand(String payload);

        /**
         * @brief Procesa un comando para controlar un LED y devuelve un mensaje descriptivo.
         *
         * Esta función recibe un comando en forma de un entero de 8 bits sin signo (uint8_t)
         * y ejecuta una acción correspondiente para controlar el estado de un LED.
         * Si el comando es 0, apaga el LED y devuelve el mensaje "LED apagado.".
         * Si el comando es 1, enciende el LED y devuelve el mensaje "LED encendido.".
         * Para cualquier otro comando, devuelve una cadena vacía.
         *
         * @param command El comando que se va a procesar.
         * @return Un mensaje descriptivo del estado del LED después de procesar el comando.
         */
        static String procesarCommand(uint8_t command);
}; // Commands.

#endif // COMMANDS_HPP.