// Implementación de la clase Connection para manejar la conexión WiFi.

// Encabezado.
#include "connection.hpp"

// Declaración de variables estáticas.
std::pair<
    std::string, std::string
> Connection::credentials = Connection::getCredentials("ciina");
const char* Connection::ssid = credentials.first.c_str();
const char* Connection::password = credentials.second.c_str();
// const char* Connection::mqttBroker = "test.mosquitto.org";
const char* Connection::mqttBroker = "broker.emqx.io";

WiFiClient Connection::espClient;
PubSubClient Connection::client(espClient);

/**
 * @brief Constructor de la clase Connection.
 * 
 * No realiza acciones específicas, ya que no se necesitan inicializaciones especiales.
 */
Connection::Connection(){
    // No se requiere inicialización especial en este caso.
}

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
std::pair<std::string, std::string> Connection::getCredentials(
    const std::string &key
) {
    if (key == "docentes") {
        return std::make_pair(
            "Docentes",
            "Pr0Un!2023*"
        );
    }
    
    else if (key == "uninavarra") {
        return std::make_pair(
            "uninavarra",
            "N@varraDeConquist@2017*#"
        );
    }
    
    else if (key == "administrativos") {
        return std::make_pair(
            "Administrativos",
            "Uni@dmin2022*9876*"
        );
    }

    else if (key == "ciina") {
        return std::make_pair(
            "CLARO-6793",
            "Cl4r0@346793"
        );
    }

    else if (key == "giovanni") {
        return std::make_pair(
            "OPPO A57",
            "mamita12"
        );
    }

    else if (key == "yo") {
        return std::make_pair(
            "YS18",
            "Pirulocoptero"
        );
    }

    else {
        return std::make_pair("", "");
    }
} // getCredentials().

/**
 * @brief Método para establecer la conexión WiFi.
 * Configura la conexión WiFi utilizando las credenciales proporcionadas (ssid y password).
 * 
 * @return void.
 */
void Connection::conectarWiFi() {
    // Se limpia el objeto JSON anterior.
    StatusCode::jsonDoc.clear();
    // Limpiar String para almacenar el JSON serializado.
    StatusCode::jsonString = String();

    StatusCode::jsonDoc["mac"] = WiFi.macAddress();
    serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
    Serial.println(StatusCode::jsonString);
    Serial.print("Conectando WiFi...");

    // Conexión WiFi.
    WiFi.mode(WIFI_STA); // Modo estación.
    WiFi.begin(ssid, password); // Ingreso a la red.

    // Tiempo de inicio de la conexión a la red.
    unsigned long startTime = millis();

    while ( // Se espera máximo 20s para la conexión a la red.
        WiFi.status() != WL_CONNECTED &&
        millis() - startTime < 20000
    ) {
        delay(1000);
        Serial.print(".");
    } // while().

    Serial.println("");

    // Si la conexión a la red falla después de 20s.
    if (WiFi.status() != WL_CONNECTED) {
        // Se limpia el objeto JSON anterior.
        StatusCode::jsonDoc.clear();

        StatusCode::jsonDoc["error"] = "No se pudo conectar a la red WiFi.";
        serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
        Serial.println(StatusCode::jsonString);
        return; // Sale del método conectar().
    } // if().

    StatusCode::jsonDoc["response"] = "Conexión WiFi exitosa";
    StatusCode::jsonDoc["red"] = ssid;
    StatusCode::jsonDoc["ip"] = WiFi.localIP();
    StatusCode::jsonDoc["dnsIP"] = WiFi.dnsIP();
    StatusCode::jsonDoc["broadcastIP"] = WiFi.broadcastIP();

    serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
    Serial.println(StatusCode::jsonString);
} // conectarWiFi().

/**
 * @brief Ejecuta la configuración y conexión al broker MQTT.
 * 
 * Esta función obtiene la intensidad de la señal WiFi (RSSI), la muestra en el monitor serie,
 * e intenta conectar al broker MQTT configurando el servidor y el callback correspondiente.
 * 
 * @return void.
 */
void Connection::ejecutarMQTT(){
    long rssi = WiFi.RSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);

    Serial.println("Conectando al broker MQTT.");
    client.setServer(mqttBroker, mqttPort);
    client.setCallback(callback);
} // ejecutarMQTT().

String Connection::validaciones(String payload) {
    // Verifica si el payload es un JSON válido.
    if (!Utils::validarJSON(payload)) {
        return StatusCode::statusError(
            "Tipo de contenido no admitido.",
            "Not format JSON."
        );
    }

    // Verifica si el payload contiene la clave command.
    if (!Utils::validarClaveCommand(payload)) {
        return StatusCode::statusError(
            "Bad Request.",
            "Not command."
        );
    }

    // Verifica que el comando esté dentro de los permitidos.
    if (!Commands::validarCommand(payload)) {
        return StatusCode::statusError(
            "Bad Request.",
            "Comando no válido."
        );
    }
    
    return StatusCode::statusOK(
        Commands::procesarCommand(
            Commands::obtenerCommand(payload)
        )
    );
} // validaciones().

void Connection::callback(
    char* topic,
    byte* payload,
    unsigned int length
) {
    String lePayload;
    // Se limpia el objeto JSON anterior.
    // StatusCode::jsonDoc.clear();
    // Limpiar String para almacenar el JSON serializado.
    // StatusCode::jsonString = String();

    // StatusCode::jsonDoc["response"] = "Mensaje recibido.";
    // StatusCode::jsonDoc["topic"] = topic;

    // serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
    // Serial.println(StatusCode::jsonString);

    // Convertir el payload en un String
    for (int i = 0; i < length; i++) {
        lePayload += (char)payload[i];
    }

    // Serial.println(lePayload);
    // Serial.println(validaciones(lePayload));

    client.publish("testTopic/ESP32/response",
        validaciones(lePayload).c_str()
    );
} // callback().

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
void Connection::reconnect() {
    // Limpiar String para almacenar el JSON serializado.
    StatusCode::jsonString = String();
    String clientID = "ESP32_" + String(WiFi.macAddress());

    while (!client.connected()) {
        // Se limpia el objeto JSON anterior.
        StatusCode::jsonDoc.clear();
        Serial.print("Intentando conectar al broker MQTT...");
        
        if (client.connect(clientID.c_str())) {
            StatusCode::jsonDoc["response"] = "Conectado al broker MQTT.";
            serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
            Serial.println(StatusCode::jsonString);
            client.subscribe("testTopic/ESP32/commands");

            StatusCode::jsonDoc["client"] = clientID;
            serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);
            client.publish("testTopic/ESP32/response",
                StatusCode::statusOK(StatusCode::jsonString).c_str()
            );
        }
        
        else {
            StatusCode::jsonDoc["response"] = "Falló.";
            StatusCode::jsonDoc["rc"] = client.state();
            serializeJson(StatusCode::jsonDoc, StatusCode::jsonString);

            Serial.println(StatusCode::jsonString);
            Serial.println("Intentando de nuevo en 5 segundos.");
            delay(5000);
        }
    }
} // reconnect().