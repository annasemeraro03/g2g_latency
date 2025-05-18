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
 * @return None
 */
void calibratePhototransistorThreshold() {
  unsigned long sumAmbient = 0;
  int sample;

  Serial.println("=========Starting calibration=========");

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
    return;
  }

  // Set the threshold between ambient and LED levels (80% toward LED reading)
  PHOTO_SENSOR_THRESHOLD_VALUE = avgAmbient + ((avgWithLed - avgAmbient) * 0.8);
}


/**
 * @brief Function to setup the ESP32
 * @param None
 * @return None
 */
void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  calibratePhototransistorThreshold(); // calibration of the phototransistor
  Serial.println(PHOTO_SENSOR_THRESHOLD_VALUE);
  if(PHOTO_SENSOR_THRESHOLD_VALUE == 0) {
    Serial.println("Calibration failed");
    exit(1);                                    // exit if calibration fails
  } else {
    Serial.println("Phototransistor threshold: " + String(PHOTO_SENSOR_THRESHOLD_VALUE));
  }
}

/**
 * @brief Function to loop the ESP32
 * @param None
 * @return None
 */
void loop() { 
  int photoSensorValue;                   // value of the phototransistor
  unsigned long detectionTimestamp = 0;   // timestamp when the phototransistor detects the light of the LED 
  
  digitalWrite(RED_LED, HIGH); // Turn on the LED
  unsigned long ledOnTimestamp = micros();   // Store the timestamp when the LED is turned on

  while(true){
    photoSensorValue = analogRead(PHOTO_SENSOR_PIN);      // Read the value of the phototransistor
    if(photoSensorValue > PHOTO_SENSOR_THRESHOLD_VALUE) { // Check if the phototransistor detects the light of the LED
      detectionTimestamp = micros();  // Store the timestamp when the phototransistor detects the light of the LED
      break;                          // Exit the loop when the light is detected
    }
  }

  unsigned long latency = detectionTimestamp - ledOnTimestamp;   // calculation of the latency

  // Print the results
  Serial.println("Phototransistor value: " + String(photoSensorValue));
  Serial.println("Latency: " + String(latency) + " µs\n");

  digitalWrite(RED_LED, LOW);   // Turn off the LED
  delay(1000);                  // Wait for 1 second before the next iteration
}