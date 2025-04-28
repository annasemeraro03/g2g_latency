/*
  Progetto: Misurazione della latenza con ESP32 e fototransistor
  Autore: Anna Semeraro
  Data: 28/04/2025
  Versione: 1.1
  Descrizione: Codice Arduino per ESP32 per misurare la latenza utilizzando un fototransistor

*/

// Librerie
#include <Arduino.h>

// Configurazione dei pin
#define PHOTO_SENSOR_PIN 34   // Pin a cui è collegato il fototransistor
#define RED_LED 14            // Pin del LED rosso

unsigned long ledOnTimestamp = 0; // Variabile per memorizzare il timestamp quando il LED è acceso
const int PHOTO_SENSOR_THRESHOLD_VALUE = 900;   // Soglia per il fototransistor (valore da calibrare in base al circuito)

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
}

void loop() { 
  delay(1000);
  digitalWrite(RED_LED, HIGH);  // Accensione led rosso
  ledOnTimestamp = micros();    // Timestamp quando il LED è acceso
   
  int photoSensorValue = analogRead(34);      // Lettura del valore del fototransistor
  Serial.println("Valore del fototransistor: " + String(photoSensorValue));

  // Confronto valore phototransistor con threshold (soglia)
  if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
    unsigned long latency = micros() - ledOnTimestamp;      // Calcolo latenza

    // Stampa delle latenza su seriale
    Serial.println("Latency: " + String(latency) + " µs");
  }
  delay(1000);
  digitalWrite(RED_LED, LOW);  // Spegnimento LED rosso
}
