#ifndef UTILS_H
#define UTILS_H

// Librerías necesarias.
#include <Arduino.h>
#include "statusCode.hpp"

/**
 * @class Utils
 * @brief Clase utilitaria para diversas operaciones,
 * incluyendo validaciones de JSON y búsquedas en arrays.
 */
class Utils {
    public:
        /**
         * @brief Constructor de la clase Utils.
         * 
         * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
         */
        Utils();

        /**
         * @brief Valida si el payload es un JSON.
         * 
         * @param payload El payload a validar como JSON.
         * @return true si el JSON es válido, false si hay errores en la deserialización.
         */
        static bool validarJSON(String payload);

        /**
         * @brief Valida si el payload JSON contiene la clave "command".
         *
         * Esta función recibe un String JSON, lo deserializa y verifica si contiene la clave
         * "command".
         *
         * @param payload El String JSON que se desea validar.
         * @return true si el JSON contiene la clave "command", false en caso contrario.
         */
        static bool validarClaveCommand(String payload);

        /**
         * @brief Verifica si un valor específico está presente en un array.
         *
         * Esta función recorre un array de enteros y verifica si contiene un valor específico.
         *
         * @param leArray El array de enteros en el que se buscará el valor.
         * @param size El tamaño del array.
         * @param value El valor que se desea buscar en el array.
         * 
         * @return true si el valor está presente en el array, false en caso contrario.
         */
        static bool containsInArray(
            const int leArray[],
            int size,
            int value
        );
};

#endif // UTILS_H.