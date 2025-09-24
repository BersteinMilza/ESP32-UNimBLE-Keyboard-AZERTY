#include <Arduino.h>
#include "BleKeyboard.h"

// Create an instance of the BleKeyboard class
BleKeyboard bleKeyboard("ESP32 Keyboard", "ESP", 80);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Keyboard Shortcuts Example");

  // Start the BLE keyboard server
  bleKeyboard.begin();
  
  Serial.println("Waiting for a client connection...");
}

void loop() {
  if (bleKeyboard.isConnected()) {
    Serial.println("Performing Copy/Paste action in 5 seconds...");
    delay(5000);

    // Type the initial text
    bleKeyboard.print("This is a test sentence.");
    delay(500);

    // 1. Select all (Ctrl+A)
    Serial.println("Sending Ctrl+A (Select All)");
    bleKeyboard.press(ModifierKey::LeftCtrl);
    bleKeyboard.press('a');
    delay(100);
    bleKeyboard.releaseAll();
    delay(500);

    // 2. Copy (Ctrl+C)
    Serial.println("Sending Ctrl+C (Copy)");
    bleKeyboard.press(ModifierKey::LeftCtrl);
    bleKeyboard.press('c');
    delay(100);
    bleKeyboard.releaseAll();
    delay(500);
    
    // 3. Move to the end of the line and press Enter
    Serial.println("Sending Right Arrow and Enter");
    bleKeyboard.tap(SpecialKey::RightArrow);
    bleKeyboard.tap(SpecialKey::Return);
    delay(500);

    // 4. Paste (Ctrl+V)
    Serial.println("Sending Ctrl+V (Paste)");
    bleKeyboard.press(ModifierKey::LeftCtrl);
    bleKeyboard.press('v');
    delay(100);
    bleKeyboard.releaseAll();
    delay(500);

    // Add a long delay before repeating the action
    delay(10000);
  }
}

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}
