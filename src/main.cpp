#include <Arduino.h>

#define PHOTO_SENSOR_PIN 34   // Pin GPIO a cui è collegato il fototransistor

void setup() {
  Serial.begin(9600);  
  pinMode(PHOTO_SENSOR_PIN, INPUT);  // Imposta il pin come ingresso
}

void loop() {
  int lightState = digitalRead(PHOTO_SENSOR_PIN);  // Legge lo stato del fototransistor
  if (lightState == HIGH) {
    Serial.println("Luce non rilevata");
  } else {
    Serial.println("Luce rilevata");
  }
  delay(500);  // Ritardo tra le letture
}