# 
# filename: collectDataFromSerial.py
# author: Anna Semeraro
# data: 17/05/2025
# 
# Description: script that collects data from a serial port and saves it to a file.
#  

# libraries
import serial
import time
import os

# configuration of the serial port
ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)  # delay for the serial connection to establish

# check if the device is between the phototransistor and the led
device = True

# set the file name based on the device presence
if device:
    file = "datiWithMiddleDevice.txt"
else:
    file = "datiWithoutMiddleDevice.txt"

# create the folder "files" if it doesn't exist
folder = "files"
os.makedirs(folder, exist_ok=True)

# retrieve the folder path of "files"
file_path = os.path.join(folder, file)

# save the data retrieved from serial port to the file
with open(file_path, "a") as file_obj:
    print("Inizio lettura... Press CTRL+C to interrupt")
    try:
        while True:
            # read a line from the serial port
            line = ser.readline().decode('utf-8').strip()
            # check if the line is not empty
            if line:
                print(line)
                file_obj.write(line + "\n")
    # check if the user wants to interrupt the reading
    except KeyboardInterrupt:
        print("Reading interrupted by user")

ser.close()
