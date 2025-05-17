# 
# Nome file: dati.py
# Autore: Anna Semeraro
# Data: 17/05/2025
# 
# Descrizione: Questo script legge i dati da un dispositivo seriale (ad esempio, un Arduino) e li salva in un file di testo.
#  

import serial
import time
import os

ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)  # attesa per inizializzazione

device = True

if device:
    file = "datiWithMiddleDevice.txt"
else:
    file = "datiWithoutMiddleDevice.txt"

folder = "files"
os.makedirs(folder, exist_ok=True)

file_path = os.path.join(folder, file)

with open(file_path, "a") as file_obj:
    print("Inizio lettura... Premi CTRL+C per interrompere.")
    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                print(line)
                file_obj.write(line + "\n")
    except KeyboardInterrupt:
        print("Lettura terminata.")

ser.close()
