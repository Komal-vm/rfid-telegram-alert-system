# 🔐 RFID Telegram Alert System

An IoT-powered smart access system using **ESP8266 + MFRC522 RFID module** that detects unauthorized tags and sends real-time alerts to **Telegram** while logging data on **ThingSpeak**. Also supports mobile NFC cards 🪪📱.

---

## 🚀 Features

- ✅ Detects authorized & unauthorized RFID/NFC cards
- 📩 Sends instant alerts to Telegram (via Bot API)
- ☁️ Logs access events to ThingSpeak (Cloud IoT Dashboard)
- 📶 WiFi-based, works with ESP8266 (NodeMCU)
- 🔒 Ideal for smart door locks, attendance, intrusion detection

---

## 🔧 Tech Stack

| Layer         | Tools Used                           |
|---------------|---------------------------------------|
| Microcontroller | ESP8266 (NodeMCU)                 |
| Sensors       | MFRC522 RFID / NFC Reader            |
| Connectivity  | WiFi (ESP8266), HTTPS                |
| Cloud         | ThingSpeak (IoT data), Telegram Bot API |
| Language      | C++ (Arduino IDE)                    |

---

## 📷 Preview

> *
>  ![IMG20250506115950](https://github.com/user-attachments/assets/48651ca7-cd92-4030-ba36-8eac3a19c8ed)

> ![image](https://github.com/user-attachments/assets/9f93e8af-1651-478c-8af2-b5dd0fbbd09b)


*

---

## 🛠️ Setup Instructions

1. **Hardware Required**
   - ESP8266 NodeMCU board
   - MFRC522 RFID module
   - Breadboard + jumper wires
   - Optional: Phone with NFC

2. **Clone this repo**
   ```bash
   git clone https://github.com/your-username/rfid-telegram-alert-system.git
3.🤖 How It Works
  RFID/NFC tag is scanned

  UID is compared to allowed list
  
  If unauthorized:
  
  Telegram alert is sent 🚨
  
  UID + timestamp are logged to ThingSpeak 📊

📡 Live Dashboard (ThingSpeak)
[Add link to your public ThingSpeak channel]



Made with ❤️ by Komal Chakradhar V M

