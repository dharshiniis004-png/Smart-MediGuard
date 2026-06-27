#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>            // FIX Bug 3: was missing entirely
#include <SoftwareSerial.h>

// ─────────────────────────────────────────────────────────────
//  OLED  (SPI mode: DC=D6, RST=D7, CS=A1)
// ─────────────────────────────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64

// FIX Bug 1: SPI constructor takes (w, h, &SPI, dc, rst, cs)
//            The original code passed 0x3C which is the I2C address
//            and is meaningless (and wrong) in SPI mode.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, 6, 7, A1);

// ─────────────────────────────────────────────────────────────
//  RTC  (DS3231 via I2C: SDA=A4, SCL=A5)
// ─────────────────────────────────────────────────────────────
RTC_DS3231 rtc;   // FIX Bug 3: added RTC object

// ─────────────────────────────────────────────────────────────
//  GSM  (SIM800L: RX=D12, TX=A3)
// ─────────────────────────────────────────────────────────────
SoftwareSerial sim800l(12, A3);   // RX, TX

// ─────────────────────────────────────────────────────────────
//  PIN DEFINITIONS
// ─────────────────────────────────────────────────────────────
#define BUZZER      8
#define CONFIRM_BTN 2
#define SNOOZE_BTN  3

// Red LEDs  : Morning=D9, Afternoon=D10, Night=D4
// Green LEDs: Morning=A2, Afternoon=D5,  Night=A0
int redLEDs[3]   = {9, 10, 4};
int greenLEDs[3] = {A2, 5, A0};

// ─────────────────────────────────────────────────────────────
//  DOSE CONFIGURATION
// ─────────────────────────────────────────────────────────────
const char* doseNames[3] = {"Morning", "Afternoon", "Night"};

// Scheduled times  (24-hour format)
const int doseHour[3]   = {10, 13, 20};   // 10:00, 13:00, 20:00
const int doseMinute[3] = { 0,  0,  0};

// ─────────────────────────────────────────────────────────────
//  PHONE NUMBER  – replace with caregiver's number
// ─────────────────────────────────────────────────────────────
String caregiverNumber = "+919491168117";

// ─────────────────────────────────────────────────────────────
//  STATE VARIABLES
// ─────────────────────────────────────────────────────────────
bool doseFiredToday[3]  = {false, false, false};  // prevent re-trigger
bool gsmReady           = false;                  // module health flag

// FIX Bug 10: snooze timing uses millis() not delay()
bool     inSnooze       = false;
unsigned long snoozeStart = 0;
const unsigned long SNOOZE_MS = 300000UL;  // 5 minutes

int  activeSnoozeCount  = 0;
int  activeDose         = -1;  // which dose is currently ringing
bool waitingForButton   = false;

// ─────────────────────────────────────────────────────────────
//  SETUP
// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  // Output pins
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);  // Active-LOW: HIGH = OFF

  for (int i = 0; i < 3; i++) {
    pinMode(redLEDs[i],   OUTPUT);
    pinMode(greenLEDs[i], OUTPUT);
    digitalWrite(redLEDs[i],   LOW);
    digitalWrite(greenLEDs[i], LOW);
  }

  // Input pins
  pinMode(CONFIRM_BTN, INPUT_PULLUP);
  pinMode(SNOOZE_BTN,  INPUT_PULLUP);

  // ── OLED ─────────────────────────────────────────────────
  // FIX Bug 1: no I2C address argument in SPI mode
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("OLED failed"));
    for (;;);   // halt – display is critical
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  showOLED("Smart MediGuard", "Initializing...", "", "");
  delay(2000);

  // ── RTC ──────────────────────────────────────────────────
  // FIX Bug 3: initialise DS3231
  if (!rtc.begin()) {
    Serial.println(F("RTC not found!"));
    showOLED("RTC ERROR", "Check wiring", "", "");
    for (;;);   // halt – RTC is critical
  }
  if (rtc.lostPower()) {
    // Set RTC to compile time if battery died
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // ── GSM ──────────────────────────────────────────────────
  sim800l.begin(9600);
  delay(2000);  // allow module to boot

  // FIX Bug 7: check module is alive before trusting it
  sim800l.println(F("AT"));
  unsigned long t = millis();
  while (millis() - t < 3000) {
    if (sim800l.available()) {
      String r = sim800l.readString();
      if (r.indexOf("OK") != -1) { gsmReady = true; break; }
    }
  }

  if (gsmReady) {
    sim800l.println(F("ATE0"));      // disable echo
    delay(300);
    sim800l.println(F("AT+CMGF=1")); // SMS text mode
    delay(300);
    Serial.println(F("GSM ready"));
  } else {
    Serial.println(F("GSM not responding – SMS disabled"));
  }

  showOLED("System Ready", "RTC Connected", "", "");
  delay(1500);
}

// ─────────────────────────────────────────────────────────────
//  MAIN LOOP  – runs forever, checks RTC every cycle
// ─────────────────────────────────────────────────────────────
void loop() {

  // FIX Bug 2 & Bug 3: read RTC and compare against schedule
  DateTime now = rtc.now();
  int h = now.hour();
  int m = now.minute();

  // Reset daily flags at midnight so doses fire again next day
  if (h == 0 && m == 0) {
    for (int i = 0; i < 3; i++) doseFiredToday[i] = false;
  }

  // Check each dose schedule (only when not already in a reminder)
  if (!waitingForButton && !inSnooze) {
    for (int i = 0; i < 3; i++) {
      if (h == doseHour[i] && m == doseMinute[i] && !doseFiredToday[i]) {
        doseFiredToday[i]  = true;
        activeDose         = i;
        activeSnoozeCount  = 0;
        waitingForButton   = true;
        startReminder(i);
        break;
      }
    }
  }

  // FIX Bug 10: snooze countdown via millis()
  if (inSnooze) {
    if (millis() - snoozeStart >= SNOOZE_MS) {
      inSnooze = false;
      waitingForButton = true;
      startReminder(activeDose);   // re-ring after snooze
    }
    return;  // skip button check while counting down
  }

  // Poll buttons only while a reminder is active
  if (waitingForButton) {
    checkButtons();
  }
}

// ─────────────────────────────────────────────────────────────
//  startReminder(dose)
//  Turns on buzzer + red LED and shows reminder on OLED.
// ─────────────────────────────────────────────────────────────
void startReminder(int dose) {
  digitalWrite(redLEDs[dose], HIGH);
  digitalWrite(BUZZER, LOW);       // Active-LOW: LOW = ON

  showOLED("Time to take", doseNames[dose], "Medicine!", "Confirm/Snooze");
  sendSMS(String(doseNames[dose]) + " medicine reminder.");
  Serial.print(F("Reminder started: "));
  Serial.println(doseNames[dose]);
}

// ─────────────────────────────────────────────────────────────
//  checkButtons()
//  Reads confirm and snooze with edge detection + debounce.
// ─────────────────────────────────────────────────────────────
void checkButtons() {
  static bool prevConfirm = HIGH;
  static bool prevSnooze  = HIGH;

  bool curConfirm = digitalRead(CONFIRM_BTN);
  bool curSnooze  = digitalRead(SNOOZE_BTN);

  // Confirm button pressed (falling edge)
  if (prevConfirm == HIGH && curConfirm == LOW) {
    delay(50);
    if (digitalRead(CONFIRM_BTN) == LOW) {
      handleConfirm();
    }
  }

  // Snooze button pressed (falling edge)
  if (prevSnooze == HIGH && curSnooze == LOW) {
    delay(50);
    if (digitalRead(SNOOZE_BTN) == LOW) {
      handleSnooze();
    }
  }

  prevConfirm = curConfirm;
  prevSnooze  = curSnooze;
}

// ─────────────────────────────────────────────────────────────
//  handleConfirm()
//  Dose taken: buzzer off, red off, green on briefly.
// ─────────────────────────────────────────────────────────────
void handleConfirm() {
  int dose = activeDose;

  digitalWrite(BUZZER,         HIGH);  // buzzer OFF
  digitalWrite(redLEDs[dose],  LOW);   // red LED OFF
  digitalWrite(greenLEDs[dose],HIGH);  // green LED ON

  showOLED(doseNames[dose], "Taken Successfully", "", "");
  sendSMS(String(doseNames[dose]) + " medicine taken successfully.");
  Serial.print(F("Confirmed: "));
  Serial.println(doseNames[dose]);

  waitingForButton = false;
  activeDose = -1;

  // Green LED stays ON for 3 seconds (non-blocking via millis)
  unsigned long greenStart = millis();
  while (millis() - greenStart < 3000) { /* brief hold */ }
  digitalWrite(greenLEDs[dose], LOW);

  showOLED("System Ready", "Waiting for", "next dose...", "");
}

// ─────────────────────────────────────────────────────────────
//  handleSnooze()
//  Snooze or declare missed dose if limit exceeded.
// ─────────────────────────────────────────────────────────────
void handleSnooze() {
  int dose = activeDose;

  activeSnoozeCount++;
  digitalWrite(BUZZER,        HIGH);  // buzzer OFF
  digitalWrite(redLEDs[dose], LOW);   // red LED OFF

  // FIX Bug 8: was >= 2, now > 2, allowing up to 2 snoozes before missed
  if (activeSnoozeCount > 2) {
    // Dose missed
    showOLED(doseNames[dose], "Dose Missed!", "Sending SMS...", "");
    sendSMS(String(doseNames[dose]) + " medicine dose missed!");
    Serial.print(F("Missed: "));
    Serial.println(doseNames[dose]);

    waitingForButton = false;
    activeDose = -1;

    unsigned long holdStart = millis();
    while (millis() - holdStart < 3000) { /* show missed screen */ }

    showOLED("System Ready", "Waiting for", "next dose...", "");

  } else {
    // Valid snooze — FIX Bug 10: use millis() not delay()
    showOLED("Reminder Snoozed", "Retry in", "5 Minutes", "");
    sendSMS(String(doseNames[dose]) + " reminder snoozed.");
    Serial.print(F("Snoozed ("));
    Serial.print(activeSnoozeCount);
    Serial.println(F(")"));

    waitingForButton = false;
    inSnooze   = true;
    snoozeStart = millis();
  }
}

// ─────────────────────────────────────────────────────────────
//  sendSMS(message)
//  Sends SMS with correct AT sequence and ">" prompt wait.
// ─────────────────────────────────────────────────────────────
void sendSMS(String message) {
  if (!gsmReady) {
    Serial.println(F("GSM unavailable – SMS skipped"));
    return;  // FIX Bug 7: don't attempt if module not confirmed
  }

  Serial.print(F("Sending SMS: "));
  Serial.println(message);

  // Step 1: ensure text mode
  sim800l.println(F("AT+CMGF=1"));
  delay(300);
  sim800l.readString();   // flush

  // Step 2: set recipient number
  sim800l.print(F("AT+CMGS=\""));
  sim800l.print(caregiverNumber);
  sim800l.println(F("\""));

  // FIX Bug 4: wait for ">" prompt from SIM800L
  unsigned long waitStart = millis();
  bool gotPrompt = false;
  while (millis() - waitStart < 5000) {
    if (sim800l.available()) {
      char c = sim800l.read();
      if (c == '>') {
        gotPrompt = true;
        delay(100);   // short settle after prompt
        break;
      }
    }
  }

  if (!gotPrompt) {
    Serial.println(F("GSM: no > prompt – SMS aborted"));
    return;  // do not crash; continue program
  }

  // FIX Bug 5: use print() not println() to avoid extra \r\n
  sim800l.print(message);

  // Step 3: send Ctrl+Z (ASCII 26) to commit the SMS
  sim800l.write(26);

  // FIX Bug 6: read confirmation reply
  unsigned long sendStart = millis();
  while (millis() - sendStart < 10000) {
    if (sim800l.available()) {
      String reply = sim800l.readString();
      if (reply.indexOf("+CMGS:") != -1) {
        Serial.println(F("SMS sent OK"));
        return;
      }
      if (reply.indexOf("ERROR") != -1) {
        Serial.println(F("SMS ERROR from module"));
        return;
      }
    }
  }
  Serial.println(F("SMS timeout – no reply from module"));
}

// ─────────────────────────────────────────────────────────────
//  showOLED(line1, line2, line3, line4)
//  Generic 4-line OLED display helper.
// ─────────────────────────────────────────────────────────────
void showOLED(String l1, String l2, String l3, String l4) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (l1.length()) { display.setCursor(0,  2); display.println(l1); }
  if (l2.length()) { display.setCursor(0, 18); display.println(l2); }
  if (l3.length()) { display.setCursor(0, 34); display.println(l3); }
  if (l4.length()) { display.setCursor(0, 50); display.println(l4); }

  display.display();
}  see igot this code and i thik it is perfect 
