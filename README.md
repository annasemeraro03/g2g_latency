# G2G LATENCY - ESP32 Phototransistor Latency Measurement

## Description
This project uses an **ESP32**, a **red LED**, and a **phototransistor** to measure the **latency between the LED activation and its detection** by the phototransistor. The system automatically calibrates the light threshold to distinguish ambient light from LED light, then measures the time elapsed between turning the LED on and detecting the change via the analog phototransistor.

## Required Components
- ESP32 development board  
- Red LED  
- Phototransistor (e.g., TEPT4400, TEPT5600, BPW96C, etc.)  
- 1MΩ pull-up resistor  
- Breadboard  
- Jumper wires  

## Wiring
- **Red LED**:
  - Anode → GPIO 14 on ESP32  
  - Cathode → GND  
- **Phototransistor**:
  - Collector → GPIO 34 (analog input)  
  - Emitter → GND  
  - Pull-up resistor between collector and 3.3V

## How It Works
1. On startup, the system calibrates ambient light and LED-on light to determine a threshold.
2. In the loop, the red LED turns on and a timestamp is recorded.
3. The phototransistor is monitored until it detects light above the threshold.
4. A second timestamp is taken, and the latency in microseconds is calculated.
5. Results are printed to the Serial Monitor.

## Sample Output
<pre>
=========Starting calibration========= 
Ambient light average: 85 
LED light average: 1536 
=========Calibration complete========= 
Phototransistor threshold: 1245 
Phototransistor value: 1562 
Latency: 623 µs
</pre>

## Troubleshooting
- Ensure LED and phototransistor are properly aligned.
- Avoid strong ambient light during tests.
- If you get 0 values, check:
  - Power connections
  - Phototransistor orientation
  - Pull-up resistor is connected

## Author
Anna Semeraro, Final-year Bachelor's student in Computer Science at UniMoRe (University of Modena and Reggio Emilia), 2025