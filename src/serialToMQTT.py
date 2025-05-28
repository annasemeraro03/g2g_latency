# 
# filename: collectDataFromSerial.py
# author: Anna Semeraro
# data: 17/05/2025
# 
# Description: script that collects data from a serial port and saves it to a file.
#  

# libraries
import serial, time, paho.mqtt.client as mqtt

# configuration of the serial port
ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)  # delay for the serial connection to establish

# MQTT configuration
MQTT_BROKER = 'test.mosquitto.org'       # or IP address of your MQTT broker
MQTT_PORT = 1883
MQTT_TOPIC = 'latencyMonitor/latency'

# Create MQTT client and connect
client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT, 60)

while True:
    try:
        # read a line from the serial port
        line = ser.readline().decode('utf-8').strip()
        # check if the line is not empty
        if line:
            print(line)
            client.publish(MQTT_TOPIC, line)
    # check if the user wants to interrupt the reading
    except KeyboardInterrupt:
        print("Reading interrupted by user")
        break

ser.close()
client.disconnect()