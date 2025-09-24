#ifndef ESP32_BLE_KEYBOARD_H
#define ESP32_BLE_KEYBOARD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include <NimBLEServer.h>
#include <NimBLECharacteristic.h>
#include <NimBLEHIDDevice.h>
#include <Print.h>


enum class ModifierKey : uint8_t {
    LeftCtrl   = 0,
    LeftShift  = 1,
    LeftAlt    = 2,
    LeftGui    = 3,
    RightCtrl  = 4,
    RightShift = 5,
    RightAlt   = 6,
    RightGui   = 7
};

enum class SpecialKey : uint8_t {
    UpArrow    = 0x52,
    DownArrow  = 0x51,
    LeftArrow  = 0x50,
    RightArrow = 0x4F,
    Backspace  = 0x2A,
    Tab        = 0x2B,
    Return     = 0x28,
    Esc        = 0x29,
    Insert     = 0x49,
    Delete     = 0x4C,
    PageUp     = 0x4B,
    PageDown   = 0x4E,
    Home       = 0x4A,
    End        = 0x4D,
    CapsLock   = 0x39,
    F1         = 0x3A,
    F2         = 0x3B,
    F3         = 0x3C,
    F4         = 0x3D,
    F5         = 0x3E,
    F6         = 0x3F,
    F7         = 0x40,
    F8         = 0x41,
    F9         = 0x42,
    F10        = 0x43,
    F11        = 0x44,
    F12        = 0x45,
    F13        = 0x68,
    F14        = 0x69,
    F15        = 0x6A,
    F16        = 0x6B,
    F17        = 0x6C,
    F18        = 0x6D,
    F19        = 0x6E,
    F20        = 0x6F,
    F21        = 0x70,
    F22        = 0x71,
    F23        = 0x72,
    F24        = 0x73
};


//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class BleKeyboard : public Print, NimBLEServerCallbacks, NimBLECharacteristicCallbacks
{
public:
  using Callback = std::function<void(void)>;

public:
  BleKeyboard(std::string deviceName = "ESP32 Keyboard", std::string deviceManufacturer = "DIY", uint8_t batteryLevel = 100);
  void begin(void);
  void end(void);
  void sendReport(KeyReport* keys);
  
  size_t press(uint8_t k); // For UNICODE characters
  size_t press(ModifierKey k);
  size_t press(SpecialKey k);
  size_t tap(SpecialKey k);
  
  size_t release(uint8_t k); // For UNICODE characters
  size_t release(ModifierKey k);
  size_t release(SpecialKey k);
  
  void setDelay(uint32_t ms);
  void releaseAll(void);
  bool isConnected(void) const;
  void setBatteryLevel(uint8_t level);
  void onConnect(Callback cb);
  void onDisconnect(Callback cb);
  void debug(uint8_t usage_id, uint8_t modifiers = 0);
  
  virtual size_t write(uint8_t c) override;
  virtual size_t write(const uint8_t *buffer, size_t size) override;

protected:
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override;
  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override;
  virtual void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;
  // void writeSequence(uint8_t c); // NEW

protected:
  NimBLEHIDDevice*      hid;
  NimBLECharacteristic* inputKeyboard;
  NimBLECharacteristic* outputKeyboard;
  KeyReport _keyReport;

  uint8_t batteryLevel;
  std::string deviceManufacturer;
  std::string deviceName;
  bool connected = false;

  Callback connectCallback    = nullptr;
  Callback disconnectCallback = nullptr;
  
private:
  uint32_t _delay = 50;
  
  bool addKeyToReport(uint8_t usage_id);
  void removeKeyFromReport(uint8_t usage_id);
  
  void typeUnicodeCharacter(uint32_t unicode_char);
  size_t pressRaw(uint8_t usage_id, uint8_t modifiers);
  
};

#endif // CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_KEYBOARD_H
