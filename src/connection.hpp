#ifndef CONNECTION_HPP
#define CONNECTION_HPP

// Librerías necesarias.
#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <PubSubClient.h>
#include "env.hpp"
#include "statusCode.hpp"
#include "commands.hpp"

/**
 * @class Connection
 * @brief Clase para manejar la conexión WiFi, un servidor HTTP, y un websocket.
 */
class Connection {
    private:
        // Credenciales de acceso a la red.
        static std::pair<std::string, std::string> credentials;
        static const char* ssid;
        static const char* password;
        
        static const char* mqttBroker;
        static const int mqttPort = MQTT_PORT;

        static WiFiClient espClient;

        /**
         * @brief Obtiene un par de credenciales basado en una clave específica.
         * 
         * Esta función retorna un par que contiene un nombre de usuario y una contraseña
         * correspondiente a la clave proporcionada. Si la clave no coincide con ninguna de las
         * opciones conocidas, se devuelve un par vacío.
         * 
         * @param key La clave que identifica el par de credenciales deseado.
         * 
         * @return std::pair<std::string, std::string>
         * Un par que contiene el nombre de usuario y la contraseña asociados con la clave.
         * Si la clave no es reconocida, se devuelve un par vacío.
         * 
         * @note Las credenciales están hardcodeadas en la función y no deberían 
         * ser utilizadas en un entorno de producción por motivos de seguridad.
         */
        static std::pair<std::string, std::string> getCredentials(
            const std::string &key
        ); // getCredentials().

        static String validaciones(String payload);

        static void callback(
            char* topic,
            byte* payload,
            unsigned int length
        ); // callback().

    public:
        /**
         * @brief Constructor de la clase Connection.
         * 
         * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
         */
        Connection();

        static PubSubClient client;

        /**
         * @brief Método para establecer la conexión WiFi.
         * 
         * Configura la conexión WiFi utilizando las credenciales proporcionadas (ssid y password).
         * 
         * @return void.
         */
        static void conectarWiFi();

        /**
         * @brief Ejecuta la configuración y conexión al broker MQTT.
         * 
         * Esta función obtiene la intensidad de la señal WiFi (RSSI), la muestra en el monitor serie,
         * e intenta conectar al broker MQTT configurando el servidor y el callback correspondiente.
         * 
         * @return void.
         */
        static void ejecutarMQTT();

        /**
         * @brief Intenta reconectar al broker MQTT y maneja la suscripción y publicación de mensajes.
         * 
         * Esta función se encarga de intentar la reconexión al broker MQTT en caso de que la conexión
         * se haya perdido. Se crea un nuevo client ID basado en la dirección MAC del ESP32.
         * 
         * En caso de una conexión exitosa, se publica un mensaje de confirmación en un tópico específico
         * y se suscribe a un tópico para recibir comandos. Si la conexión falla, se muestra un mensaje
         * de error con el código de retorno y se reintenta la conexión después de 5 segundos.
         * 
         * @return void.
         */
        static void reconnect();
};

#endif // CONNECTION_HPP