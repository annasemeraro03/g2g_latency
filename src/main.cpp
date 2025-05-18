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
unsigned long PHOTO_SENSOR_THRESHOLD_VALUE = 0;               // threshold for the phototransistor (value to be calibrated based on the circuit)

unsigned long calcolateThreshold(bool ledState) {
  const int SAMPLE_COUNT = 50; // Number of samples to collect
  unsigned long sum = 0;       // Variable to store the sum of samples

  // Turn on the led ON and collect samples
  if (ledState) {
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(RED_LED, LOW);
  }
  delay(500);  // Allow time for light to reach sensor

  // Collect samples with the LED on
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    int sample = analogRead(PHOTO_SENSOR_PIN);
    sum += sample;
    delay(10);
  }

  unsigned long threshold = sum / SAMPLE_COUNT; // Calculate the average

  // return the average light level with the LED on
  return threshold;
}

/**
 * @brief Calibrates the phototransistor to determine a reliable threshold 
 *        for detecting when the red LED is turned on.
 * 
 * @return unsigned long - the calculated threshold value
 */
unsigned long calibratePhototransistorThreshold() {
  unsigned long sumAmbient = 0;
  int sample;

  Serial.println("=========Starting calibration...=========");

  // Calculate the average ambient light level
  unsigned long avgAmbient = calcolateThreshold(false); // Call the function to get the average with LED off
  Serial.println("Ambient light average: " + String(avgAmbient));

  unsigned long avgWithLed = calcolateThreshold(true); // Call the function to get the average with LED on
  Serial.println("LED light average: " + String(avgWithLed));

  // Turn off the LED after calibration
  digitalWrite(RED_LED, LOW);

  // Debug info
  Serial.println("=========Calibration complete=========");

  // Safety check: make sure the LED was detected
  if (avgWithLed <= avgAmbient) {
    Serial.println("Error: LED light not detected clearly. Check your wiring or circuit.");
    return 0;
  }

  // Set the threshold between ambient and LED levels (80% toward LED reading)
  unsigned long threshold = avgAmbient + ((avgWithLed - avgAmbient) * 0.8);

  return threshold;
}


/**
 * @brief Function to setup the ESP32
 * @param None
 * @return None
 */
void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  PHOTO_SENSOR_THRESHOLD_VALUE = calibratePhototransistorThreshold(); // calibration of the phototransistor
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
 */
void loop() { 
  int photoSensorValue;                   // variable to store the value of the phototransistor
  unsigned long detectionTimestamp = 0;   // variable to store the timestamp when the phototransistor detects the light of the LED 
  unsigned long ledOnTimestamp = 0;       // variable to store the timestamp when the LED is on
  
  #ifdef HIGH_MEASUREMENT
  digitalWrite(RED_LED, HIGH);  // turn on red LED 
  ledOnTimestamp = micros();    // timestamp when the LED is on 
  while (true) {
    photoSensorValue = analogRead(PHOTO_SENSOR_PIN);
    if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
      detectionTimestamp = micros();  // timestamp when the phototransistor detects the light of the LED
      break;                         
    }
    delay(100);  // delay to allow the phototransistor to stabilize
  }
  Serial.println("phototransistor value where led = on: " + String(photoSensorValue));
  delay(500);
  #endif

  // compare phototransistor value with threshold to check if the LED is on
  if (photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) {
    unsigned long latency = detectionTimestamp - ledOnTimestamp;      // latency calculation
    Serial.println("Latency: " + String(latency) + " µs");
  }

  #ifdef LOW_MEASUREMENT
  delay(500);
  digitalWrite(RED_LED, LOW);  // Spegnimento LED rosso

  photoSensorValue = analogRead(34);      // reading the value of the phototransistor
  Serial.println("phototransistor value where led = off: " + String(photoSensorValue));
  delay(500);
  #endif
}
