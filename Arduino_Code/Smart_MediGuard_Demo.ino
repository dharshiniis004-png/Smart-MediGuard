#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

// ========================== OLED Configuration ==========================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, 6, 7, A1);

// ========================== GSM Configuration ===========================

SoftwareSerial sim800l(12, A3);

// Replace with a valid phone number before using GSM
String caregiverNumber = "YOUR_PHONE_NUMBER";

// =========================== Pin Definitions ============================

#define BUZZER_PIN       8
#define CONFIRM_BUTTON   2
#define SNOOZE_BUTTON    3

// ============================= LED Pins ================================

// Morning, Afternoon, Night
const int redLEDs[3] = {9, 10, 4};
const int greenLEDs[3] = {A2, 5, A0};

// ========================== Medicine Labels ============================

const char* medicineNames[3] = {
  "Morning",
  "Afternoon",
  "Night"
};

// ======================== Function Prototypes ==========================

void runDose(int doseIndex);
void sendSMS(String number, String message);

// =============================== Setup ================================

void setup() {

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);   // Active LOW buzzer

  pinMode(CONFIRM_BUTTON, INPUT_PULLUP);
  pinMode(SNOOZE_BUTTON, INPUT_PULLUP);

  for (int i = 0; i < 3; i++) {
    pinMode(redLEDs[i], OUTPUT);
    pinMode(greenLEDs[i], OUTPUT);

    digitalWrite(redLEDs[i], LOW);
    digitalWrite(greenLEDs[i], LOW);
  }

  // OLED Initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Smart MediGuard");
  display.println();
  display.println("Medicine Reminder");
  display.println();
  display.println("System Starting...");
  display.display();

  delay(3000);

  // GSM Initialization
  sim800l.begin(9600);

  delay(1000);
  sim800l.println("AT");

  delay(1000);
  sim800l.println("AT+CMGF=1");

  delay(1000);
}
// =============================== Loop ================================

void loop() {

  // Demo mode: Process all three medicine reminders sequentially
  for (int dose = 0; dose < 3; dose++) {
    runDose(dose);
    delay(5000);
  }

  // Display completion message
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("All doses");
  display.println("processed.");
  display.println();
  display.println("Demo Complete!");
  display.display();

  while (true);
}

// =========================== Medicine Logic ===========================

void runDose(int doseIndex) {

  bool medicineTaken = false;
  int snoozeCount = 0;

  while (!medicineTaken) {

    // Activate reminder
    digitalWrite(redLEDs[doseIndex], HIGH);
    digitalWrite(BUZZER_PIN, LOW);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Time to take");
    display.setCursor(0, 18);
    display.print(medicineNames[doseIndex]);
    display.setCursor(0, 45);
    display.print("Sending SMS...");
    display.display();

    sendSMS(
      caregiverNumber,
      String(medicineNames[doseIndex]) + " reminder"
    );

    bool snoozed = false;

    while (true) {

      // Confirm Button
      if (digitalRead(CONFIRM_BUTTON) == LOW) {

        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(redLEDs[doseIndex], LOW);
        digitalWrite(greenLEDs[doseIndex], HIGH);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(medicineNames[doseIndex]);
        display.println(" dose");
        display.println("taken");
        display.setCursor(0, 45);
        display.print("Sending SMS...");
        display.display();

        sendSMS(
          caregiverNumber,
          String(medicineNames[doseIndex]) + " dose taken"
        );

        delay(3000);

        digitalWrite(greenLEDs[doseIndex], LOW);

        medicineTaken = true;
        break;
      }

      // Snooze Button
      if (digitalRead(SNOOZE_BUTTON) == LOW) {

        digitalWrite(BUZZER_PIN, HIGH);

        snoozed = true;
        snoozeCount++;

        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Reminder");
        display.println("Snoozed");
        display.setCursor(0, 45);
        display.print("Sending SMS...");
        display.display();

        sendSMS(
          caregiverNumber,
          String(medicineNames[doseIndex]) + " reminder snoozed"
        );

        delay(5000);
        break;
      }
    }

    if (snoozed && !medicineTaken) {

      if (snoozeCount >= 2) {

        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(redLEDs[doseIndex], LOW);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(medicineNames[doseIndex]);
        display.println(" dose");
        display.println("missed!");
        display.setCursor(0, 45);
        display.print("Sending SMS...");
        display.display();

        sendSMS(
          caregiverNumber,
          String(medicineNames[doseIndex]) + " dose missed"
        );

        break;
      }

      continue;
    }

    if (medicineTaken) {
      break;
    }
  }
}
// =========================== GSM Function ============================

void sendSMS(String number, String message) {

  sim800l.print("AT+CMGS=\"");
  sim800l.print(number);
  sim800l.println("\"");

  delay(1000);

  sim800l.print(message);

  delay(1000);

  sim800l.write(26);   // CTRL + Z

  delay(3000);
}
