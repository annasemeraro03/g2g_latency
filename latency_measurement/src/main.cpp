#include <WiFi.h>
#include <PubSubClient.h>

#define PHOTO_SENSOR_PIN 34   // Pin a cui è collegato il fototransistor
#define RED_LED 14            // Pin del LED rosso

// Configurazione della rete WiFi
const char* ssid = "TIM-55279956";  // Sostituisci con il tuo SSID
const char* password = "TUf3huU6bYkskCk9C3xHEAAY";  // Sostituisci con la tua password WiFi

// Configurazione del Broker MQTT
const char* mqtt_server = "test.mosquitto.org";  // Puoi usare un broker pubblico come hivemq o mosquitto
const char* mqtt_topic = "arduino/latency";  // Topic dove inviare i dati

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long ledOnTimestamp = 0;

// Soglia per il fototransistor (valore da calibrare in base al circuito) --> che soglia tengo?
const int PHOTO_SENSOR_THRESHOLD_VALUE = 900;

void setup() {
  Serial.begin(9600);
  
  pinMode(RED_LED, OUTPUT);

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione WiFi...");
  }
  Serial.println("WiFi connesso");

  client.setServer(mqtt_server, 1883);  // Impostiamo il broker MQTT
  Serial.println("Broker MQTT impostato");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Client")) {
      Serial.println("Connesso al broker MQTT");
    }else{
      Serial.print("Connessione fallita, rc=");
      Serial.println(client.state());
    }
  }
}

void loop() {
  digitalWrite(RED_LED, HIGH);
  ledOnTimestamp = micros();

  int photoSensorValue = analogRead(PHOTO_SENSOR_PIN);  // Leggi il valore del fototransistor

  if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
    unsigned long latency = micros() - ledOnTimestamp;  // Calcola la latenza

    // Converte la latenza in formato stringa
    String latencyStr = String(latency);

    // Stampa delle latenza su seriale
    Serial.println("Latency: " + latencyStr + " µs");
    Serial.println("Valore del fototransistor: " + String(photoSensorValue));

    
    if (!client.connected()) {
      reconnect();
    }
    // Pubblica il dato MQTT
    client.publish(mqtt_topic, latencyStr.c_str());
    Serial.println("Dati pubblicati su MQTT: " + latencyStr);

    digitalWrite(RED_LED, LOW);  // Spegne il LED rosso
    delay(1000);                 // Aspetta 1 secondo prima della prossima lettura
  }
}
