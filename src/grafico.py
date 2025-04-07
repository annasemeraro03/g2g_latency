import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
from datetime import datetime

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)

# Configura i dettagli del broker MQTT
mqtt_broker = "test.mosquitto.org"  # Usa lo stesso broker MQTT di Arduino
mqtt_topic = "arduino/latency"  # Il topic su cui Arduino invia i dati
mqtt_port = 1883

# Dati per il grafico
timestamps = []
latencies = []

# Funzione di callback quando un messaggio è ricevuto
def on_message(client, userdata, msg):
    # Decodifica il messaggio ricevuto
    latency = float(msg.payload.decode())
    timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]  # Formato timestamp

    # Aggiungi i dati a liste
    timestamps.append(timestamp)
    latencies.append(latency)
    print(f"Ricevuto: {timestamp} - Latency: {latency} µs")

    # Crea il grafico in tempo reale
    plt.clf()  # Pulisce il grafico precedente
    plt.plot(timestamps, latencies, label="Latency (µs)")
    plt.xlabel("Timestamp (s)")
    plt.ylabel("Latency (µs)")
    plt.xticks(rotation=45)  # Ruota i timestamp per evitare sovrapposizioni
    plt.tight_layout()  # Ottimizza lo spazio del grafico
    plt.title("Latency graph")
    plt.pause(0.1)  # Pausa per aggiornare il grafico

# Imposta il client MQTT utilizzando la versione aggiornata dell'API
client = mqtt.Client(client_id="Python_Client", clean_session=True)
client.on_message = on_message

# Connessione al broker
client.connect(mqtt_broker, mqtt_port, 60)

# Sottoscrizione al topic
client.subscribe(mqtt_topic)

# Avvia il loop per ricevere i messaggi
plt.ion()  # Modalità interattiva per il grafico in tempo reale
plt.show()

try:
    client.loop_forever()  # Tieni il client in ascolto
except KeyboardInterrupt:
    print("Uscita...")
    plt.ioff()
    plt.show()
