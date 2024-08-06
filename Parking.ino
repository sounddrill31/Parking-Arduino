#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Constants
const int IR_SENSOR1_PIN = 2;
const int IR_SENSOR2_PIN = 3;
const int SERVO_PIN = 4;
const int SLOT_COUNT = 4;
const int SERVO_OPEN_ANGLE = 0;
const int SERVO_CLOSED_ANGLE = 100;

// Variables
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
int slotCount = SLOT_COUNT;
bool irSensor1Triggered = false;
bool irSensor2Triggered = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(IR_SENSOR1_PIN, INPUT);
  pinMode(IR_SENSOR2_PIN, INPUT);
  servo.attach(SERVO_PIN);
  servo.write(SERVO_CLOSED_ANGLE);
  lcd.setCursor(0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check IR sensors
  if (digitalRead(IR_SENSOR1_PIN) == LOW && !irSensor1Triggered) {
    irSensor1Triggered = true;
    if (slotCount > 0) {
      servo.write(SERVO_OPEN_ANGLE);
      slotCount--;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    SORRY :(    ");
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  ");
      delay(3000);
      lcd.clear();
    }
  }

  if (digitalRead(IR_SENSOR2_PIN) == LOW && !irSensor2Triggered) {
    irSensor2Triggered = true;
    if (!irSensor1Triggered) {
      servo.write(SERVO_OPEN_ANGLE);
      slotCount++;
    }
  }

  // Reset servo and flags
  if (irSensor1Triggered && irSensor2Triggered) {
    delay(1000);
    servo.write(SERVO_CLOSED_ANGLE);
    irSensor1Triggered = false;
    irSensor2Triggered = false;
  }

  // Update LCD display
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor(0, 1);
  lcd.print("Slot Left: ");
  lcd.print(slotCount);
}
