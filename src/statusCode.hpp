#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

// Librerías necesarias.
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @class StatusCode
 * @brief Clase para manejar los códigos de estado de cierre de un websocket.
 */
class StatusCode {
    public:
        /**
         * @brief Constructor de la clase StatusCode.
         * 
         * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
         */
        StatusCode();

        static StaticJsonDocument<64> jsonDoc; // Objeto JSON a utilizar.
        static String jsonString; // String para almacenar el JSON serializado.

        static String statusOK(String response);

        static String statusError(String response, String e);
};

#endif // STATUSCODE_HPP