#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuración WiFi
const char* ssid = "cuarto";
const char* password = "norueguito";

// Configuración MQTT
const char* mqtt_broker = "77.42.17.7";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp8266/datos";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }
  
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi conectado");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Error: No se pudo conectar a WiFi");
  }
}

void reconnect() {
  int intentos = 0;
  while (!client.connected() && intentos < 5) {
    Serial.print("Conectando a MQTT...");
    
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 5s");
      delay(5000);
      intentos++;
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("=== Iniciando ESP8266 MQTT ===");
  
  setup_wifi();
  
  client.setServer(mqtt_broker, mqtt_port);
  
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Generar y enviar número aleatorio
  int numero_aleatorio = random(0, 100);
  
  char payload[50];
  snprintf(payload, sizeof(payload), "Numero: %d", numero_aleatorio);
  
  Serial.print("Publicando en ");
  Serial.print(mqtt_topic);
  Serial.print(": ");
  Serial.println(payload);
  
  client.publish(mqtt_topic, payload);
  
  delay(5000); // Esperar 5 segundos antes de enviar otro
}