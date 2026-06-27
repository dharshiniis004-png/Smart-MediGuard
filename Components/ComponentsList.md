# Components List

This document lists all the hardware, software, and libraries used in the **Smart MediGuard – IoT Medicine Reminder System**.

---

# Hardware Components

### Arduino UNO R3

* Quantity: 1
* Purpose: Main microcontroller that controls the complete medicine reminder system.

### DS3231 RTC Module

* Quantity: 1
* Purpose: Maintains accurate real-time information for medicine scheduling.

### SSD1306 SPI OLED Display (128×64)

* Quantity: 1
* Purpose: Displays reminder messages, system status, and user notifications.

### SIM800L GSM Module

* Quantity: 1
* Purpose: Sends SMS notifications to the caregiver.

### Active LOW Buzzer

* Quantity: 1
* Purpose: Generates an audible alert whenever a medicine reminder is triggered.

### Red LEDs

* Quantity: 3
* Purpose: Indicates active reminders for Morning, Afternoon, and Night medicines.

### Green LEDs

* Quantity: 3
* Purpose: Indicates successful confirmation of medicine intake.

### Confirm Push Button

* Quantity: 1
* Purpose: Confirms that the medicine has been taken.

### Snooze Push Button

* Quantity: 1
* Purpose: Delays the reminder by five minutes.

### 220Ω Resistors

* Quantity: 6
* Purpose: Limits current flowing through the LEDs.

### Breadboard

* Quantity: 1
* Purpose: Used for circuit prototyping and hardware assembly.

### Jumper Wires

* Quantity: As Required
* Purpose: Provides electrical connections between all hardware components.

### External 3.7V–4.2V Regulated Power Supply

* Quantity: 1
* Purpose: Provides stable power to the SIM800L GSM module.

### USB Cable

* Quantity: 1
* Purpose: Used to program and power the Arduino UNO during development.

---

# Software Requirements

* Arduino IDE
* Embedded C/C++

---

# Arduino Libraries

* Wire.h
* SPI.h
* Adafruit_GFX.h
* Adafruit_SSD1306.h
* RTClib.h
* SoftwareSerial.h

---

# Power Requirements

* Arduino UNO – 5V
* DS3231 RTC – 5V
* SSD1306 SPI OLED – 5V
* SIM800L GSM – External Regulated 3.7V–4.2V Supply
* LEDs – 5V through 220Ω resistors
* Active LOW Buzzer – 5V

---

# Notes

* The DS3231 RTC provides accurate real-time scheduling.
* Three Red LEDs indicate active reminders.
* Three Green LEDs indicate successful medicine confirmation.
* The SIM800L GSM module is integrated in both hardware and software.
* During hardware testing, SMS transmission could not be demonstrated because a compatible 2G SIM/network was unavailable.
* The project was developed as a breadboard prototype for embedded systems implementation and testing.
