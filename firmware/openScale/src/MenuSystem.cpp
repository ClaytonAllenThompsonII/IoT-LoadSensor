#include <LiquidCrystal_I2C.h>
#include "MenuSystem.h"
#include "ScaleManager.h"

// Constants for button pins
const int buttonTarePin = 2;
const int buttonCalibratePin = 3;
const int buttonOnOffPin = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address, columns, and rows


void MenuSystem::initialize() {
    // Initialize display and buttons here
    lcd.init();                      // Initialize the LCD
    lcd.backlight();
    pinMode(buttonTarePin, INPUT_PULLUP);
    pinMode(buttonCalibratePin, INPUT_PULLUP);
    pinMode(buttonOnOffPin, INPUT_PULLUP);
    lcd.setCursor(0, 0);
    lcd.print("OpenScale Ready");
    
}

void MenuSystem::update() {
    // Check button presses and update display accordingly
    // Example: If tare button pressed, call ScaleManager::tare();
   if (digitalRead(buttonTarePin) == LOW) {
        tareScale();
    } else if (digitalRead(buttonCalibratePin) == LOW) {
        calibrateScale();
    } else if (digitalRead(buttonOnOffPin) == LOW) {
        togglePower();
    }
}

void MenuSystem::tareScale() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Taring...");
    ScaleManager::tare();
    delay(500);  // Debounce delay
    lcd.print("Done");
}

void MenuSystem::calibrateScale() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating...");
    // Calibration logic
    delay(500);  // Debounce delay
    lcd.print("Calibrated");
}

void MenuSystem::togglePower() {
    // Power toggle logic
}