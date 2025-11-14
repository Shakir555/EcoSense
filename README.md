# EcoSense – Enviro-IoT Dashboard  
*A modern environmental monitoring system powered by ESP32 + Vue 3*

EcoSense is a premium, real-time **IoT dashboard** that displays **temperature** and **humidity** readings collected from a **DHT11 sensor**, processed by an **ESP32 microcontroller**, and visualized through a **modern glass-style web interface** built with **Vue 3 + TailwindCSS**.

This project demonstrates:
- Embedded development (ESP32 firmware)
- REST API communication
- Real-time data visualization
- Beautiful modern UI/UX dashboard design

---

## System Architecture

EcoSense uses a **client–server architecture** where the ESP32 provides sensor data and the Vue dashboard consumes and visualizes it.

### Communication Flow  
![ESP32 Communication Diagram](diagram/esp_web.png)

**ESP32 REST API Endpoints**

| Endpoint | Description | Example Output |
|---------|-------------|----------------|
| `/hello` | Basic connectivity test | `"Hello World from ESP32!"` |
| `/data` | Sensor data (JSON) | `{ "temperature": 28.0, "humidity": 32.0 }` |

---

## Web Dashboard UI (Preview)

EcoSense features a **clean**, **modern**, glass-style IoT dashboard with smooth gradients and real-time charts.

![EcoSense Dashboard UI](diagram/eco_sense_ui.png)

### Dashboard Features
- Live **temperature** display (°C)  
- Live **humidity** display (%)  
- Real-time line charts with auto-updating  
- API connection indicator (Connected / Disconnected)  
- Timestamp of last sensor update  
- Modern glass-UI with gradients and soft shadows  
- Fully responsive (desktop → mobile)

---

## ⚙️ Tech Stack

### **Embedded / Hardware**
- ESP32 (WiFi Station Mode)
- DHT11 sensor
- ESP-IDF (recommended) or Arduino framework

### **Frontend**
- Vue 3 (Composition API)
- TailwindCSS
- Chart.js
- Vite Dev Server

### **Communication**
- REST API over HTTP  
- JSON responses  
- Local WiFi network  

## Author  
**Shakir Salam**
