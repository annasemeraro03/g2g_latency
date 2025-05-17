/*
  Project: Measurement of the latency with ESP32 and phototransistor 
  Author: Anna Semeraro
  Description: Arduino code for ESP32 to measure latency using a phototransistor
*/

// TODO:
// 1. Modify the function to calibrate the phototransistor threshold value
// 2. Add a function to measure the latency 
// 3. Add a function to measures the value of the phototransistor and print the value

// libraries
#include <Arduino.h>

// configuration of the pins
#define PHOTO_SENSOR_PIN 34   // pin to which the phototransistor is connected
#define RED_LED 14            // pin to which the red LED is connected

#define LOW_MEASUREMENT true
#define HIGH_MEASUREMENT false

// variables
unsigned long ledOnTimestamp = 0;                             // variable to store the timestamp when the LED is on
unsigned long PHOTO_SENSOR_THRESHOLD_VALUE = 0;               // threshold for the phototransistor (value to be calibrated based on the circuit)

const int LEN_VECTOR = 10;                                    // length of the vector for calibration

/**
 * @brief Function to calibrate the phototransistor threshold value
 * @param None
 * @return unsigned long - threshold value for the phototransistor
 * 
 */
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

/**
 * @brief Function to setup the ESP32
 * @param None
 * @return None
 * 
 */
void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  PHOTO_SENSOR_THRESHOLD_VALUE = calibration(); // calibration of the phototransistor
  Serial.println(PHOTO_SENSOR_THRESHOLD_VALUE);
  if(PHOTO_SENSOR_THRESHOLD_VALUE == 0) {
    Serial.println("calibration failed");
    exit(1);                                    // exit if calibration fails
  } else {
    Serial.println("phototransistor threshold: " + String(PHOTO_SENSOR_THRESHOLD_VALUE));
  }

}

/**
 * @brief Function to loop the ESP32
 * @param None
 * @return None
 * 
 */
void loop() { 
  int photoSensorValue;
  #ifdef HIGH_MEASUREMENT
  delay(500);
  digitalWrite(RED_LED, HIGH);  // turn on red LED 
  ledOnTimestamp = micros();    // timestamp when the LED is on 
   
  photoSensorValue = analogRead(34);      // reading the value of the phototransistor
  Serial.println("phototransistor value where led = on: " + String(photoSensorValue));
  delay(500);
  #endif

  // compare phototransistor value with threshold to check if the LED is on
  // if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
  //   unsigned long latency = micros() - ledOnTimestamp;      // latency calculation

  //   Serial.println("Latency: " + String(latency) + " µs");
  // }

  #ifdef LOW_MEASUREMENT
  delay(500);
  digitalWrite(RED_LED, LOW);  // Spegnimento LED rosso

  photoSensorValue = analogRead(34);      // reading the value of the phototransistor
  Serial.println("phototransistor value where led = off: " + String(photoSensorValue));
  delay(500);
  #endif
}
