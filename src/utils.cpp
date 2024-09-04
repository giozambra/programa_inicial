/**
 * Implementación de la clase Utils para diversas operaciones,
 * incluyendo validaciones de JSON y búsquedas en arrays.
 */

// Encabezado.
#include "utils.hpp"

/**
 * @brief Constructor de la clase Utils.
 * 
 * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
 */
Utils::Utils() {
    // No se requiere inicialización especial en este caso.
} // Utils().

/**
 * @brief Valida si el payload es un JSON.
 * 
 * @param payload El payload a validar como JSON.
 * @return true si el JSON es válido, false si hay errores en la deserialización.
 */
bool Utils::validarJSON(String payload) {
    StatusCode::jsonDoc.clear(); // Se limpia el objeto JSON anterior.
    StatusCode::jsonString = String(); // Limpiar String para almacenar el JSON serializado.

    DeserializationError error = deserializeJson(
        StatusCode::jsonDoc,
        payload
    );

    if (error) {
        StatusCode::jsonDoc.clear(); // Se limpia el objeto JSON anterior.

        StatusCode::jsonDoc["response"] = "Error al analizar JSON.";
        StatusCode::jsonDoc["error"] = error.c_str();

        serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
        Serial.println(StatusCode::jsonString);
        return false;
    }

    // Pendiente validar el paso de números en vez del JSON.
    return true;
} // validarJSON().

/**
 * @brief Valida si el payload JSON contiene la clave "command".
 *
 * Esta función recibe un String JSON, lo deserializa y verifica si contiene la clave "command".
 *
 * @param payload El String JSON que se desea validar.
 * @return true si el JSON contiene la clave "command", false en caso contrario.
 */
bool Utils::validarClaveCommand(String payload) {
    StatusCode::jsonDoc.clear(); // Se limpia el objeto JSON anterior.

    deserializeJson(StatusCode::jsonDoc, payload);
    return StatusCode::jsonDoc.containsKey("command");
} // validarClaveCommand().

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
bool Utils::containsInArray(
    const int leArray[],
    int size,
    int value
) {
    for (int i = 0; i < size; i++) {
        if (leArray[i] == value) {
            // El valor se encontró en el array.
            return true;
        }
    }
    
    // El valor no se encontró en el array.
    return false;
} // containsInArray().