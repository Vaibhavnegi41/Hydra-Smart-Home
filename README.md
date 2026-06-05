<div align="center">

<br/>

```
██╗  ██╗██╗   ██╗██████╗ ██████╗  █████╗ 
██║  ██║╚██╗ ██╔╝██╔══██╗██╔══██╗██╔══██╗
███████║ ╚████╔╝ ██║  ██║██████╔╝███████║
██╔══██║  ╚██╔╝  ██║  ██║██╔══██╗██╔══██║
██║  ██║   ██║   ██████╔╝██║  ██║██║  ██║
╚═╝  ╚═╝   ╚═╝   ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝
```

# 🏠 HYDRA SMART HOME

### *Anchored on land. Floating on water. Powered by intelligence.*

<br/>


> **H**ybrid **D**ynamic **R**obotic **A**utomation &nbsp;|&nbsp; **S**elf-operating **M**onitoring **A**I-driven **R**emote-controlled **T**echnologically integrated

<br/>

</div>

---

## 📌 What is Hydra?

**Hydra Smart Home** is a next-generation **IoT + AI powered amphibious smart home system** capable of operating on both **land and water**. It combines smart automation, AI-based security, real-time safety monitoring, and disaster-resilient mobility — all controlled via a web-based dashboard and voice commands.

> 💡 **The world's first disaster-adaptive smart home** — built to float, sense, and survive.

---

## 🚨 The Problem

| ❌ Current Reality | ✅ Hydra's Solution |
|---|---|
| Homes fail completely during floods | Floats & moves autonomously during disasters |
| No mobile smart living systems exist | Full amphibious mobility with GPS tracking |
| Gas leaks detected too late | Real-time MQ2 sensor with instant alerts |
| Traditional locks are easily bypassed | Face recognition + RFID dual-layer security |
| Manual management for elderly & disabled | Full voice + remote automation via Alexa |

---

## 🧩 Core Modules

<br/>

### 🔒 Smart Door Security
> **Face Recognition + RFID Access Control**

Uses **OpenCV + DeepFace** for real-time facial recognition combined with RFID for dual-layer authentication. Every access event is logged with timestamp to the web dashboard.

```
Visitor Detected → Face Scan → Match? → Grant/Deny Access → Log Event
```

---

### 🏡 Smart Home Automation
> **Voice + Remote Control of All Appliances**

Full control of lights, fans, doors, and curtains via the web dashboard or **Alexa voice commands** using Sinric Pro integration.

```
Voice Command / Dashboard → SinricPro → ESP32 → Relay Module → Appliance
```

---

### 🌊 Flood Prediction & Autonomous Relocation
> **Multi-Sensor Early Warning System**

Combines **Rain Sensor (FC-37)**, **Ultrasonic Water Level**, and **BMP280 Pressure** data to detect flood conditions and autonomously trigger relocation sequence.

```
Sensors → Threshold Breach? → Alert → Autonomous Motor Activation → Safe Zone
```

---

### 📍 GPS Tracking & Mobility
> **Real-Time Location + Motorized Control**

Live GPS coordinates via **NEO-6M module** displayed on web dashboard map. Manual override available for directional control.

```
GPS Module → ESP32 → Web Interface → Live Map Display
```

---

### ⛽ Gas Detection
> **LPG, Methane & Flammable Gas Sensing**

**MQ2 sensor** continuously monitors for dangerous gas concentrations with instant web dashboard alerts and configurable threshold levels.

---

### 🚤 Amphibious Design
> **Land-to-Water Transition System**

Inflatable buoyant tubes provide floatation while **dual paddle propulsion** enables water navigation. 4× 12V gear motors handle land mobility.

---

## 🛠️ Tech Stack

### 💻 Software

| Layer | Technology | Role |
|-------|-----------|------|
| **AI / Vision** | Python, OpenCV, DeepFace | Face recognition & access control |
| **Firmware** | Arduino IDE, ESP32 | Hardware control & sensor reading |
| **Voice AI** | Sinric Pro + Amazon Alexa | Voice command processing |
| **Dashboard** | HTML / CSS / JavaScript | Web control interface |

### ⚙️ Hardware

| Component | Spec | Purpose |
|-----------|------|---------|
| **NodeMCU ESP32** | Dual-core 240MHz | Main controller |
| **Relay Module** | 12V 4-Channel | Appliance switching |
| **L298N Motor Driver** | Dual H-Bridge | Motor control |
| **MQ2 Gas Sensor** | LPG/Methane/Flammable | Gas leak detection |
| **FC-37 Rain Sensor** | Analog output | Rain intensity detection |
| **BMP280** | ±1 hPa accuracy | Pressure monitoring |
| **HC-SR04 Ultrasonic** | 2cm–400cm range | Water level measurement |
| **NEO-6M GPS Module** | ±2.5m accuracy | Real-time location |
| **MG90S + SG90 Servos** | Metal gear | Door & curtain control |
| **12V Gear Motors ×4** | 100 RPM | Ground mobility |
| **18650 Li-Ion ×8** | 3.7V / 2600mAh each | Power supply |
| **RFID Module** | 13.56 MHz | Alternate access control |

---

## 📱 Web Dashboard

```
┌─────────────────────────────────────────────────────────┐
│  🏠 HYDRA SMART HOME          ● Connected    [Settings] │
├─────────────────┬───────────────────────────────────────┤
│  CONTROLS       │  LIVE GPS LOCATION                    │
│                 │                                       │
│  💡 Lights  ON  │         [  MAP  ]                    │
│  🌀 Fan     OFF │                                       │
│  🚪 Door    --  │  Lat: 28.7041°  Lng: 77.1025°        │
│  🪟 Curtain --  │                                       │
├─────────────────┼───────────────────────────────────────┤
│  VEHICLE        │  SENSORS                              │
│                 │                                       │
│    [  ▲  ]      │  ⛽ Gas:    ✅ Safe (120 ppm)         │
│  [◄] [■] [►]   │  🌧 Rain:   ⚠️  Detected              │
│    [  ▼  ]      │  💧 Water:  ✅ Normal (12cm)          │
│                 │  🌡 Press:  ✅ 1013 hPa               │
├─────────────────┴───────────────────────────────────────┤
│  LAST ACCESS EVENT:  John Doe — Face Match — 10:42 AM  │
└─────────────────────────────────────────────────────────┘
```

---

## 🏗️ System Architecture

```
                        ┌──────────────────┐
                        │   USER INPUTS    │
                        └────────┬─────────┘
                                 │
               ┌─────────────────┼─────────────────┐
               │                 │                 │
        ┌──────▼──────┐   ┌──────▼──────┐   ┌──────▼──────┐
        │  Web Dashboard  │   Voice (Alexa)  │  Face / RFID │
        └──────┬──────┘   └──────┬──────┘   └──────┬──────┘
               │                 │                 │
               └─────────────────▼─────────────────┘
                                 │
                        ┌────────▼─────────┐
                        │   NodeMCU ESP32  │
                        └────────┬─────────┘
                                 │
        ┌────────────────────────┼────────────────────────┐
        │                        │                        │
┌───────▼────────┐    ┌──────────▼──────────┐   ┌────────▼────────┐
│  Relay Module  │    │   Sensor Array       │   │  Motor Drivers  │
│  (Appliances)  │    │  MQ2 | FC37 | BMP280 │   │  L298N (×2)     │
│  Lights / Fan  │    │  Ultrasonic | GPS    │   │  4× Gear Motors │
│  Doors/Curtain │    └─────────────────────┘   │  Servo Motors   │
└────────────────┘                              └─────────────────┘
```

---

## 🚀 Getting Started

### Prerequisites

```bash
Python 3.10+
Arduino IDE 2.x
ESP32 Board Package installed
HuggingFace / Sinric Pro account
```

### Installation

```bash
# 1. Clone the repository
git clone https://github.com/YourUsername/hydra-smart-home.git
cd hydra-smart-home

# 2. Create virtual environment
python -m venv venv
venv\Scripts\activate        # Windows
source venv/bin/activate     # Mac/Linux

# 3. Install Python dependencies
pip install -r requirements.txt

# 4. Setup environment variables
cp .env.example .env
# Add your API keys to .env

# 5. Flash ESP32 firmware
# Open /firmware/hydra_firmware.ino in Arduino IDE
# Select ESP32 Dev Module → Upload

# 6. Run the vision module
python face_recognition/main.py

# 7. Open dashboard
# Navigate to your ESP32's IP address in browser
```

### Environment Variables

```env
SINRIC_APP_KEY=your_sinric_app_key
SINRIC_APP_SECRET=your_sinric_app_secret
DEVICE_ID_LIGHT=your_device_id
DEVICE_ID_FAN=your_device_id
DEVICE_ID_DOOR=your_device_id
```

---

## 🌍 Real-World Applications

```
🏘️  Flood-prone residential areas        🧓  Elderly & disabled smart living
⛑️  Disaster relief emergency shelters   🔬  Environmental monitoring stations
🔍  Rescue & surveillance units          📚  Research & educational demonstrations
🌏  Remote area smart living solutions
```

---

## 🤝 Contributing

Contributions are welcome!

```bash
# 1. Fork the repository
# 2. Create your feature branch
git checkout -b feature/amazing-feature

# 3. Commit your changes
git commit -m "feat: add amazing feature"

# 4. Push to branch
git push origin feature/amazing-feature

# 5. Open a Pull Request
```

---

<div align="center">

<br/>

**⭐ If you found this project useful, please give it a star on GitHub! ⭐**

<br/>

*Built with ❤️ — where technology meets resilience*

<br/>

![Made with Python](https://img.shields.io/badge/Made%20with-Python-blue?style=flat-square)
![IoT](https://img.shields.io/badge/Type-IoT%20%2B%20AI-green?style=flat-square)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)

</div>
