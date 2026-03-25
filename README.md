🏠 Hydra Smart Home

"Anchored on land, floating on water, powered by intelligence."


📌 About The Project
Hydra Smart Home is an IoT + AI powered amphibious smart home system capable of operating on both land and water. It combines smart automation, AI-based security, real-time safety monitoring, and disaster-resilient mobility — all controlled via a web-based dashboard and voice commands.
HYDRA → Hybrid Dynamic Robotic Automation
SMART → Self-operating, Monitoring, AI-driven, Remote-controlled, Technologically integrated

🚨 Problem Statement

Existing homes cannot function during floods or environmental disasters
No adaptive or mobile smart living systems exist for dynamic environments
Inadequate early detection for gas leaks and fire hazards
Traditional door locks lack smart AI-based verification
Elderly and disabled individuals struggle with manual home management


✅ Solution
A fully integrated smart home prototype that is:

Disaster-Resilient — floats and moves autonomously during floods
AI-Secured — face recognition + RFID for authorized access only
IoT Automated — remote control of all home appliances via web/voice
Safety Monitored — real-time gas, rain, pressure, and water-level sensing


🧩 Modules
ModuleDescription🔆 Smart AutomationRemote/voice control of lights, fans, doors & curtains via web interface + Alexa🔐 Smart Door SecurityFace recognition (OpenCV + DeepFace) and RFID access control📍 GPS Tracking & MobilityReal-time GPS tracking + motorized wheel control via web interface💨 Gas DetectionMQ2 sensor detects LPG, methane & flammable gases with early alerts🌊 Flood PredictionRain, ultrasonic & pressure sensors trigger autonomous relocation on threshold breach🚤 Amphibious DesignInflatable buoyant tubes + dual paddle propulsion for water navigation

🛠️ Tech Stack & Components
Software

Python (OpenCV, DeepFace)
Arduino IDE (ESP32 firmware)
Sinric Pro / Alexa (Voice control)
Web Interface (HTML/CSS/JS dashboard)

Hardware
ComponentPurposeNodeMCU ESP32Main controller12V 4-Channel Relay ModuleAppliance switchingL298N Motor DriverMotor controlMQ2 Gas SensorGas leak detectionFC-37 Rain SensorRain intensity detectionBMP280 Pressure SensorAtmospheric pressure monitoringUltrasonic SensorWater level measurementGPS Module (NEO-6M)Real-time location trackingMG90S + SG90 Servo MotorsDoor & curtain control12V 100 RPM Gear Motors (×4)Mobility18650 Lithium Batteries (×8)Power supplyRFID ModuleAlternate access control

📱 Web Dashboard Features

 Lights & Fan manual control
 Main door, balcony door & curtain toggle
 Vehicle control (Forward / Backward / Left / Right)
 Live GPS location map
 Gas sensor status (MQ2)
 Latest door access event log


🚀 Applications

 Flood-prone residential areas
 Disaster relief and emergency shelters
 Smart homes for elderly and disabled individuals
 Environmental monitoring stations
 Rescue and surveillance units
 Remote area smart living solutions
 Research and educational demonstration platform


🔮 Future Enhancements

 Solar panel integration for self-sustainability
 ML-based advanced flood prediction using satellite data
 Autonomous AI navigation and obstacle detection
 Air quality, humidity & water quality sensors
 Cloud integration with mobile app connectivity
