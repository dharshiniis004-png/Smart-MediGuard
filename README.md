# 💊 Smart MediGuard – IoT Medicine Reminder System

An Arduino UNO-based Smart Medicine Reminder System designed to help users take medicines on time through scheduled reminders using a Real-Time Clock (RTC). The system provides audio-visual alerts and allows users to confirm or snooze reminders. A SIM800L GSM module is included in the design to support caregiver notifications.

---

## 📖 Overview

Medication adherence is a major challenge, especially for elderly patients and individuals with chronic illnesses. Smart MediGuard is a low-cost embedded system that provides timely medicine reminders without requiring a smartphone or internet connection.

The system uses an Arduino UNO as the main controller, a DS3231 RTC module for accurate timekeeping, an LCD display for reminder messages, a buzzer and LEDs for alerts, and push buttons for user interaction.

---

## ✨ Features

- ⏰ Real-time medicine reminders using DS3231 RTC
- 📟 LCD displays reminder messages
- 🔊 Buzzer for audio alerts
- 🔴 Red LED for active reminders
- 🟢 Green LED for confirmation
- ✅ Confirm button to acknowledge medicine intake
- 😴 Snooze button to delay reminders
- 📩 SIM800L GSM module support for caregiver SMS notifications
- 🔌 Standalone operation without internet connectivity

---

## 🛠 Hardware Used

- Arduino UNO
- DS3231 RTC Module
- 16×2 I2C LCD Display
- SIM800L GSM Module
- Buzzer
- Red LED
- Green LED
- Push Buttons (Confirm & Snooze)
- Breadboard
- Jumper Wires

---

## 💻 Software Used

- Arduino IDE
- Embedded C/C++

---

## ⚙️ Working Principle

1. The DS3231 RTC continuously keeps track of the current time.
2. Three medicine reminder timings are stored in the Arduino.
3. At the scheduled time:
   - LCD displays the reminder.
   - Buzzer sounds.
   - Red LED turns ON.
4. User can:
   - Press **Confirm** to stop the alert and mark the dose as taken.
   - Press **Snooze** to postpone the reminder.
5. SIM800L GSM integration is included to support SMS notifications to caregivers.

---

## 📂 Repository Structure

```
Smart-MediGuard/
│
├── Arduino_Code/
├── Circuit_Diagram/
├── Block_Diagram/
├── Flowchart/
├── Images/
├── Documentation/
├── Components/
└── README.md
```

---

## 🚀 Future Improvements

- Mobile application integration
- Automated pill dispensing
- Cloud-based medication tracking
- Voice reminders
- Battery backup system

---

## 👨‍💻 My Contribution

This project was developed as a **team project (3 members)**.

My contributions included:

- Arduino programming
- Hardware integration
- Circuit assembly
- RTC and LCD interfacing
- Testing and debugging
- Project documentation

---

## 📄 License

This project is licensed under the MIT License.
