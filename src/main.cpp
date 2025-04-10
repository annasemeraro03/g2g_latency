/*
  Progetto: Misurazione della latenza con ESP32 e fototransistor
  Autore: Anna Semeraro
  Data: 10/04/2025
  Versione: 1.0
  Descrizione: Codice Arduino per ESP32 per misurare la latenza utilizzando un fototransistor, i cui dati vengono pubblicati su un broker MQTT

*/

// Librerie
#include <WiFi.h>
#include <PubSubClient.h>

// Configurazione dei pin
#define PHOTO_SENSOR_PIN 34   // Pin a cui è collegato il fototransistor
#define RED_LED 14            // Pin del LED rosso

// Configurazione della rete WiFi
const char* ssid = "rete_wifi";  // Sostituisci con il tuo SSID
const char* password = "password";  // Sostituisci con la tua password WiFi

// Configurazione del Broker MQTT
const char* mqtt_server = "test.mosquitto.org";  // broker mqtt pubblico
const char* mqtt_topic = "arduino/latency";  // Topic dove inviare i dati

// Inizializzazione del client WiFi e del client MQTT
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long ledOnTimestamp = 0; // Variabile per memorizzare il timestamp quando il LED è acceso
const int PHOTO_SENSOR_THRESHOLD_VALUE = 900;   // Soglia per il fototransistor (valore da calibrare in base al circuito)

// Funzione per connettersi alla rete WiFi
void connectToWiFi() {
  Serial.print("Connessione alla rete WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" connesso");
}

// Funzione per la riconnessione al broker MQTT
void reconnectToMQTT() {
  while (!client.connected()) {
    if (client.connect("ESP32_Client")) {
      Serial.println("Connesso al broker MQTT");
    }else{
      Serial.print("Connessione fallita, rc=");
      Serial.println(client.state());
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(RED_LED, OUTPUT);

  connectToWiFi();                              // Connessione alla rete WiFi

  client.setServer(mqtt_server, 1883);          // set del broker MQTT
  Serial.println("Broker MQTT impostato");
}


void loop() {
  digitalWrite(RED_LED, HIGH);  // Accensione led rosso
  ledOnTimestamp = micros();    // Timestamp quando il LED è acceso

   
  int photoSensorValue = analogRead(PHOTO_SENSOR_PIN);      // Lettura del valore del fototransistor
  Serial.println("Valore del fototransistor: " + String(photoSensorValue));

  // Confronto valore phototransistor con threshold (soglia)
  if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
    unsigned long latency = micros() - ledOnTimestamp;      // Calcolo latenza

    // Stampa delle latenza su seriale
    Serial.println("Latency: " + String(latency) + " µs");

    // Controllo della connessione al broker MQTT
    if (!client.connected()) {
      reconnectToMQTT();
    }

    // Pubblicazione del dato MQTT
    client.publish(mqtt_topic, String(latency).c_str()); // Pubblica la latenza in microsecondi
    Serial.println("Dati pubblicati su MQTT: " + String(latency) + " µs");

    digitalWrite(RED_LED, LOW);  // Spegnimento LED rosso
  }
  delay(1000);                   // Attesa di 1 secondo prima della prossima lettura
}
