# filename: collectDataFromSerial.py
# author: Anna Semeraro
# data: 17/05/2025
#
# Description: script that collects data from a serial port, saves it to a file,
# and publishes it over MQTT.

import serial, time, os
import paho.mqtt.client as mqtt

# === Serial Port Configuration ===
ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)  # allow the connection to establish

# === MQTT Configuration ===
MQTT_BROKER = 'test.mosquitto.org'
MQTT_PORT = 1883
MQTT_TOPIC = 'latencyMonitor/latency'

client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# === Output File Setup ===
filename = input("Insert filename (es. NokiaLumia_photomode_latency.csv): ").strip()
if not filename.endswith(".csv"):
    filename += ".csv"

files_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "files")
os.makedirs(files_dir, exist_ok=True)
file_path = os.path.join(files_dir, filename)

# === Data Collection Loop ===
MAX_MEASUREMENTS = 1000
measurements = 0

with open(file_path, "a") as file:
    try:
        while measurements < MAX_MEASUREMENTS:
            line = ser.readline().decode('utf-8').strip()
            if line:
                print(line)
                file.write(line + "\n")              # Save to file
                client.publish(MQTT_TOPIC, line)     # Publish to MQTT
                measurements += 1
    except KeyboardInterrupt:
        print(f"\nMeasurements captured: {measurements}")
        print("Reading interrupted by user")

# === Cleanup ===
ser.close()
client.disconnect()
