# 💊 Smart MediGuard – IoT Medicine Reminder System

An **Arduino UNO-based Smart Medicine Reminder System** that helps users take medicines on time using a **DS3231 Real-Time Clock (RTC)** for accurate scheduling. The system provides **audio-visual alerts**, allows users to **confirm or snooze reminders**, and integrates a **SIM800L GSM module** to support caregiver SMS notifications.

---

## 📖 Overview

Medication adherence is a significant challenge, particularly for elderly individuals and patients with chronic illnesses. Missing or delaying medication can lead to serious health complications.

**Smart MediGuard** is a low-cost embedded system designed to provide reliable medicine reminders without requiring a smartphone or internet connection. The system continuously monitors the current time using a **DS3231 RTC** and triggers reminders at predefined medication schedules. Users can confirm that they have taken their medicine or snooze the reminder. The design also includes GSM support for caregiver notifications.

---

## ✨ Features

* ⏰ Accurate real-time medicine reminders using **DS3231 RTC**
* 🖥️ **SSD1306 SPI OLED** display for reminder messages and system status
* 🔊 Active LOW buzzer for audible alerts
* 🔴 Three Red LEDs indicating active reminders (Morning, Afternoon, Night)
* 🟢 Three Green LEDs indicating successful medicine confirmation
* ✅ Confirm button to acknowledge medicine intake
* 😴 Snooze button to postpone reminders
* 📩 SIM800L GSM integration for caregiver SMS notifications
* 🕒 Three scheduled medicine reminders:

  * Morning – **10:00 AM**
  * Afternoon – **1:00 PM**
  * Night – **8:00 PM**
* 🔌 Standalone embedded system requiring no internet connection
* ⚙️ Modular Arduino code with RTC-based scheduling

---

# 🛠 Hardware Components

| Component                    |    Quantity |
| ---------------------------- | ----------: |
| Arduino UNO R3               |           1 |
| DS3231 RTC Module            |           1 |
| SSD1306 SPI OLED Display     |           1 |
| SIM800L GSM Module           |           1 |
| Active LOW Buzzer            |           1 |
| Confirm Push Button          |           1 |
| Snooze Push Button           |           1 |
| Red LEDs                     |           3 |
| Green LEDs                   |           3 |
| 220Ω Resistors               |           6 |
| Breadboard                   |           1 |
| Jumper Wires                 | As Required |
| External 4V Supply (SIM800L) |           1 |

---

# 💻 Software Used

* Arduino IDE
* Embedded C/C++
* RTClib Library
* Adafruit SSD1306 Library
* Adafruit GFX Library
* SoftwareSerial Library

---

# ⚙️ Working Principle

1. The DS3231 RTC continuously maintains accurate real-time information.
2. Arduino compares the current RTC time with the stored medicine schedules.
3. At the scheduled time:

   * OLED displays the medicine reminder.
   * Corresponding Red LED turns ON.
   * Buzzer starts sounding.
   * GSM module attempts to send a reminder SMS.
4. The user can:

   * **Press Confirm** to stop the alert, turn ON the corresponding Green LED, and send a confirmation SMS.
   * **Press Snooze** to postpone the reminder for 5 minutes.
5. If the reminder is snoozed more than two times, the system marks the dose as missed and attempts to send a missed-dose SMS.
6. The system continuously repeats this process throughout the day.

---

# 🧠 System Architecture

* **Arduino UNO** acts as the central controller.
* **DS3231 RTC** provides accurate timekeeping.
* **OLED Display** shows reminder and status messages.
* **SIM800L GSM Module** handles SMS notifications.
* **Buzzer** provides audible alerts.
* **Three Red LEDs** indicate active reminders.
* **Three Green LEDs** indicate successful medicine confirmation.
* **Confirm and Snooze Buttons** allow user interaction.

---

# 📂 Repository Structure

```text
Smart-MediGuard/
│
├── Arduino_Code/
│   └── Smart_MediGuard.ino
│
├── Circuit_Diagram/
│   └── CircuitDiagram.png
│
├── Block_Diagram/
│   └── BlockDiagram.png
│
├── Flowchart/
│   └── Flowchart.png
│
├── Components/
│   └── ComponentsList.md
│
├── Documentation/
│   └── ProjectDocumentation.pdf
│
├── Images/
│   ├── Cover.png
│   └── Working.png
│
├── LICENSE
├── .gitignore
└── README.md
```

---

# 🚀 Future Enhancements

* Mobile application integration
* Automated pill dispensing mechanism
* Cloud-based medication monitoring
* Voice-assisted reminders
* Battery backup system
* Mobile notifications over Wi-Fi or Bluetooth
* Multiple patient profile support

---

# 📊 Project Information

| Parameter               | Details          |
| ----------------------- | ---------------- |
| Project Type            | Embedded Systems |
| Platform                | Arduino UNO      |
| Programming Language    | Embedded C/C++   |
| Development Environment | Arduino IDE      |
| Communication           | SIM800L GSM      |
| Time Management         | DS3231 RTC       |
| Team Size               | 3 Members        |
| Project Status          | Completed        |

---

# 👨‍💻 My Contribution

This project was developed as a **team project consisting of three members**.

My primary contributions included:

* Arduino firmware development
* RTC scheduling implementation
* OLED display interfacing
* GSM module integration
* Hardware integration and circuit assembly
* System testing and debugging
* GitHub repository organization
* Technical documentation

---

# ⚠️ GSM Implementation Note

The **SIM800L GSM module** has been fully integrated into both the hardware design and Arduino firmware.

During hardware testing, SMS transmission could not be demonstrated because a compatible **2G SIM/network** was unavailable. However, the GSM communication logic and AT command implementation are fully included in the project.

---

# 📌 Applications

* Elderly healthcare monitoring
* Home medicine reminder systems
* Hospital patient assistance
* Chronic disease medication management
* Assisted living environments

---

# 📜 License

This project is licensed under the **MIT License**.

---

## ⭐ If you found this project useful, consider giving it a Star on GitHub.
