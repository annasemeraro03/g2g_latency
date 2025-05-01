/*
  Project: Measurement of the latency with ESP32 and phototransistor 
  Author: Anna Semeraro
  Description: Arduino code for ESP32 to measure latency using a phototransistor

*/

// libraries
#include <Arduino.h>

// configuration of the pins
#define PHOTO_SENSOR_PIN 34   // pin to which the phototransistor is connected
#define RED_LED 14            // pin to which the red LED is connected

// variables
unsigned long ledOnTimestamp = 0;                             // variable to store the timestamp when the LED is on
unsigned long PHOTO_SENSOR_THRESHOLD_VALUE = 0;               // threshold for the phototransistor (value to be calibrated based on the circuit)

const int LEN_VECTOR = 10;                                    // length of the vector for calibration

unsigned long calibration(){
  unsigned long photo_calibration_vec[LEN_VECTOR];
  unsigned long sum = 0;
  int valid_samples = 0;

  // reading values from the phototransistor for calibration
  for(int i=0; i<LEN_VECTOR; i++){
    photo_calibration_vec[i] = analogRead(PHOTO_SENSOR_PIN);
    delayMicroseconds(90);
  }

  // sum of all phototransistor readings to determine the average threshold
  for(int i=0; i<LEN_VECTOR; i++){
      // if reading 0 = max light, so shift all data to one
      photo_calibration_vec[i] += 1;
      if (sum + photo_calibration_vec[i] < sum) {   // overflow check
          Serial.println("Overflow detected!");
          return 0;                                 // handle overflow case
      }
      sum += photo_calibration_vec[i];
  }

  unsigned long threshold = sum / LEN_VECTOR;

  return threshold;
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  PHOTO_SENSOR_THRESHOLD_VALUE = calibration(); // calibration of the phototransistor
  if(PHOTO_SENSOR_THRESHOLD_VALUE == 0) {
    Serial.println("calibration failed");
    exit(1);                                    // exit if calibration fails
  } else {
    Serial.println("phototransistor threshold: " + String(PHOTO_SENSOR_THRESHOLD_VALUE));
  }

}

void loop() { 
  delay(1000);
  digitalWrite(RED_LED, HIGH);  // turn on red LED 
  ledOnTimestamp = micros();    // timestamp when the LED is on 
   
  int photoSensorValue = analogRead(34);      // reading the value of the phototransistor
  Serial.println("value of the phototransistor: " + String(photoSensorValue));

  // compare phototransistor value with threshold to check if the LED is on
  if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
    unsigned long latency = micros() - ledOnTimestamp;      // latency calculation

    Serial.println("Latency: " + String(latency) + " µs");
  }
  delay(1000);
  digitalWrite(RED_LED, LOW);  // Spegnimento LED rosso
}
