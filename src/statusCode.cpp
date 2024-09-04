// Implementación de la clase CodeStatus para gestionar respuestas de WebSocket.

// Encabezado.
#include "statusCode.hpp"

// Declaración de variables estáticas.
StaticJsonDocument<64> StatusCode::jsonDoc; // Objeto JSON a utilizar.
String StatusCode::jsonString; // String para almacenar el JSON serializado.

/**
 * @brief Constructor de la clase StatusCode.
 * 
 * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
 */
StatusCode::StatusCode() {
    // No se requiere inicialización especial en este caso.
} // StatusCode().

String StatusCode::statusOK(String response) {
    jsonDoc.clear(); // Se limpia el objeto JSON anterior.
    jsonString = String(); // Limpiar String para almacenar el JSON serializado.
    DeserializationError error = deserializeJson(jsonDoc, response);

    if (error){
        // Añadir datos al nuevo objeto JSON.
        jsonDoc["response"] = response;
        serializeJson(jsonDoc, jsonString);
        return jsonString;
    }
    
    return response;
} // statusOK().

String StatusCode::statusError(String response, String e) {
    jsonDoc.clear(); // Se limpia el objeto JSON anterior.
    jsonString = String(); // Limpiar String para almacenar el JSON serializado.

    jsonDoc["response"] = response;
    jsonDoc["error"] = e;
    serializeJson(jsonDoc, jsonString);
    return jsonString;
}