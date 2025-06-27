# filename: collectDataFromSerial.py
# author: Anna Semeraro
# data: 17/05/2025
#
# Description: script that collects data from a serial port, saves it to a file,
# and (optionally) publishes it over MQTT.

import serial, time, os
import paho.mqtt.client as mqtt

# === Serial Port Configuration ===
ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)  # allow the connection to establish

# === Ask if MQTT should be used ===
use_mqtt = input("Do you want to enable MQTT publishing? [y/N]: ").strip().lower() == 'y'
if use_mqtt:
    # === MQTT Configuration ===
    MQTT_BROKER = 'test.mosquitto.org'
    MQTT_PORT = 1883
    MQTT_TOPIC = 'latencyMonitor/'

    client = mqtt.Client()
    client.connect(MQTT_BROKER, MQTT_PORT, 60)

# === Device and Mode Input ===
device = input("Insert device name (es: iPhone 13, Samsung Galaxy S15, ...): ").strip().replace(" ", "")
mode = input("Insert mode (es: photomode, videomode): ").strip().replace(" ", "").lower()

# === Compose Filename ===
if not device or not mode:
    filename = "noDevice_noMode_latency.csv"
else:
    filename = f"{device}_{mode}_latency.csv"

if use_mqtt:
    MQTT_TOPIC += device
    print(f"Publishing to MQTT topic: {MQTT_TOPIC}")

# === File Path Setup ===
files_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "files")
os.makedirs(files_dir, exist_ok=True)
file_path = os.path.join(files_dir, filename)

# === Data Collection Loop ===
MAX_MEASUREMENTS = 1005
measurements = 0

with open(file_path, "a") as file:
    try:
        while measurements < MAX_MEASUREMENTS:
            line = ser.readline().decode('utf-8').strip()
            if line:
                print(line + "," + str(measurements))
                file.write(line + "\n")  # Save to file
                if use_mqtt:
                    client.publish(MQTT_TOPIC, line)  # Publish to MQTT
                measurements += 1
    except KeyboardInterrupt:
        print(f"\nMeasurements captured: {measurements}")
        print("Reading interrupted by user")

# === Cleanup ===
ser.close()
if use_mqtt:
    client.disconnect()
