#include "BleKeyboard.h"

#include <NimBLEDevice.h>
#include "HIDTypes.h"

#include "sdkconfig.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)
  #include "esp32-hal-log.h"
  #define LOG_TAG ""
#else
  #include "esp_log.h"
  static const char* LOG_TAG = "NimBLEDevice";
#endif

// Standard HID Keyboard USAGE IDs
#define KEY_A 0x14
#define KEY_B 0x05
#define KEY_C 0x06
#define KEY_D 0x07
#define KEY_E 0x08
#define KEY_F 0x09
#define KEY_G 0x0A
#define KEY_H 0x0B
#define KEY_I 0x0C
#define KEY_J 0x0D
#define KEY_K 0x0E
#define KEY_L 0x0F
#define KEY_M 0x33
#define KEY_N 0x11
#define KEY_O 0x12
#define KEY_P 0x13
#define KEY_Q 0x04
#define KEY_R 0x15
#define KEY_S 0x16
#define KEY_T 0x17
#define KEY_U 0x18
#define KEY_V 0x19
#define KEY_W 0x1d
#define KEY_X 0x1b
#define KEY_Y 0x1C
#define KEY_Z 0x1a

#define KEY_1 0x1e
#define KEY_2 0x1F
#define KEY_3 0x20
#define KEY_4 0x21
#define KEY_5 0x22
#define KEY_6 0x23
#define KEY_7 0x24
#define KEY_8 0x25
#define KEY_9 0x26
#define KEY_0 0x27

#define KEY_RETURN 0x28
#define KEY_ESCAPE 0x29
#define KEY_BACKSPACE 0x2A
#define KEY_TAB 0x2B
#define KEY_SPACE 0x2C
#define KEY_DEGREE 0x2D
#define KEY_EQUAL 0x2E
#define KEY_SQUARE 0x35
#define KEY_USD 0x30
#define KEY_CIRCUMFLEX 0x2f
#define KEY_ASTERISK 0x31
#define KEY_PERCENT 0x34

#define KEY_COMMA 0x10
#define KEY_SEMICOLON 0x36
#define KEY_COLON 0x37
#define KEY_EXCLAMATION 0x38

#define KEY_COMPARE 0x64

typedef struct {
    uint8_t modifiers1;
    uint8_t key1;
    uint8_t modifiers2;
    uint8_t key2;
} KeyPressSequence;

typedef struct {
    uint32_t unicode;
    KeyPressSequence sequence;
} KeymapEntry;

#define LSHIFT (1 << (uint8_t)ModifierKey::LeftShift)
#define ALT_GR  (1 << (uint8_t)ModifierKey::RightAlt)

// FR Keymap
static const KeymapEntry keymap[] = {
    {U'a', {0, KEY_A, 0, 0}},
    {U'b', {0, KEY_B, 0, 0}},
    {U'c', {0, KEY_C, 0, 0}},
    {U'd', {0, KEY_D, 0, 0}},
    {U'e', {0, KEY_E, 0, 0}},
    {U'f', {0, KEY_F, 0, 0}},
    {U'g', {0, KEY_G, 0, 0}},
    {U'h', {0, KEY_H, 0, 0}},
    {U'i', {0, KEY_I, 0, 0}},
    {U'j', {0, KEY_J, 0, 0}},
    {U'k', {0, KEY_K, 0, 0}},
    {U'l', {0, KEY_L, 0, 0}},
    {U'm', {0, KEY_M, 0, 0}},
    {U'n', {0, KEY_N, 0, 0}},
    {U'o', {0, KEY_O, 0, 0}},
    {U'p', {0, KEY_P, 0, 0}},
    {U'q', {0, KEY_Q, 0, 0}},
    {U'r', {0, KEY_R, 0, 0}},
    {U's', {0, KEY_S, 0, 0}},
    {U't', {0, KEY_T, 0, 0}},
    {U'u', {0, KEY_U, 0, 0}},
    {U'v', {0, KEY_V, 0, 0}},
    {U'w', {0, KEY_W, 0, 0}},
    {U'x', {0, KEY_X, 0, 0}},
    {U'y', {0, KEY_Y, 0, 0}},
    {U'z', {0, KEY_Z, 0, 0}},
    {U'A', {LSHIFT, KEY_A, 0, 0}},
    {U'B', {LSHIFT, KEY_B, 0, 0}},
    {U'C', {LSHIFT, KEY_C, 0, 0}},
    {U'D', {LSHIFT, KEY_D, 0, 0}},
    {U'E', {LSHIFT, KEY_E, 0, 0}},
    {U'F', {LSHIFT, KEY_F, 0, 0}},
    {U'G', {LSHIFT, KEY_G, 0, 0}},
    {U'H', {LSHIFT, KEY_H, 0, 0}},
    {U'I', {LSHIFT, KEY_I, 0, 0}},
    {U'J', {LSHIFT, KEY_J, 0, 0}},
    {U'K', {LSHIFT, KEY_K, 0, 0}},
    {U'L', {LSHIFT, KEY_L, 0, 0}},
    {U'M', {LSHIFT, KEY_M, 0, 0}},
    {U'N', {LSHIFT, KEY_N, 0, 0}},
    {U'O', {LSHIFT, KEY_O, 0, 0}},
    {U'P', {LSHIFT, KEY_P, 0, 0}},
    {U'Q', {LSHIFT, KEY_Q, 0, 0}},
    {U'R', {LSHIFT, KEY_R, 0, 0}},
    {U'S', {LSHIFT, KEY_S, 0, 0}},
    {U'T', {LSHIFT, KEY_T, 0, 0}},
    {U'U', {LSHIFT, KEY_U, 0, 0}},
    {U'V', {LSHIFT, KEY_V, 0, 0}},
    {U'W', {LSHIFT, KEY_W, 0, 0}},
    {U'X', {LSHIFT, KEY_X, 0, 0}},
    {U'Y', {LSHIFT, KEY_Y, 0, 0}},
    {U'Z', {LSHIFT, KEY_Z, 0, 0}},
    {U'1', {LSHIFT, KEY_1, 0, 0}},
    {U'2', {LSHIFT, KEY_2, 0, 0}},
    {U'3', {LSHIFT, KEY_3, 0, 0}},
    {U'4', {LSHIFT, KEY_4, 0, 0}},
    {U'5', {LSHIFT, KEY_5, 0, 0}},
    {U'6', {LSHIFT, KEY_6, 0, 0}},
    {U'7', {LSHIFT, KEY_7, 0, 0}},
    {U'8', {LSHIFT, KEY_8, 0, 0}},
    {U'9', {LSHIFT, KEY_9, 0, 0}},
    {U'0', {LSHIFT, KEY_0, 0, 0}},
    {U' ', {0, KEY_SPACE, 0, 0}},
    {U'!', {0, KEY_EXCLAMATION, 0, 0}},
    {U'"', {0, KEY_3, 0, 0}},
    {U'#', {ALT_GR, KEY_3, 0, 0}},
    {U'$', {0, KEY_USD, 0, 0}},
    {U'%', {LSHIFT, KEY_PERCENT, 0, 0}},
    {U'&', {0, KEY_1, 0, 0}},
    {U'\'', {0, KEY_4, 0, 0}},
    {U'(', {0, KEY_5, 0, 0}},
    {U')', {0, KEY_DEGREE, 0, 0}},
    {U'*', {0, KEY_ASTERISK, 0, 0}},
    {U'+', {LSHIFT, KEY_EQUAL, 0, 0}},
    {U',', {0, KEY_COMMA, 0, 0}},
    {U'-', {0, KEY_6, 0, 0}},
    {U'.', {LSHIFT, KEY_SEMICOLON, 0, 0}},
    {U'/', {LSHIFT, KEY_COLON, 0, 0}},
    {U':', {0, KEY_COLON, 0, 0}},
    {U';', {0, KEY_SEMICOLON, 0, 0}},
    {U'<', {0, KEY_COMPARE, 0, 0}},
    {U'=', {0, KEY_EQUAL, 0, 0}},
    {U'>', {LSHIFT, KEY_COMPARE, 0, 0}},
    {U'?', {LSHIFT, KEY_COMMA, 0, 0}},
    {U'@', {ALT_GR, KEY_0, 0, 0}},
    {U'[', {ALT_GR, KEY_5, 0, 0}},
    {U'\\', {ALT_GR, KEY_8, 0, 0}},
    {U']', {ALT_GR, KEY_DEGREE, 0, 0}},
    {U'^', {ALT_GR, KEY_9, 0, 0}},
    {U'_', {0, KEY_8, 0, 0}},
    {U'`', {ALT_GR, KEY_7, 0, KEY_SPACE}},
    {U'{', {ALT_GR, KEY_4, 0, 0}},
    {U'|', {ALT_GR, KEY_6, 0, 0}},
    {U'}', {ALT_GR, KEY_EQUAL, 0, 0}},
    {U'~', {ALT_GR, KEY_2, 0, KEY_SPACE}},
    {U'\n', {0, KEY_RETURN, 0, 0}},
    {U'\t', {0, KEY_TAB, 0, 0}},
    {U'é', {0, KEY_2, 0, 0}},
    {U'€', {ALT_GR, KEY_E, 0, 0}},
    {U'£', {LSHIFT, KEY_USD, 0, 0}},
    {U'è', {0, KEY_7, 0, 0}},
    {U'à', {0, KEY_0, 0, 0}},
    {U'ù', {0, KEY_PERCENT, 0, 0}},
    {U'ç', {0, KEY_9, 0, 0}},

    // Accented characters (dead keys)
    {U'â', {0, KEY_CIRCUMFLEX, 0, KEY_A}},
    {U'ê', {0, KEY_CIRCUMFLEX, 0, KEY_E}},
    {U'î', {0, KEY_CIRCUMFLEX, 0, KEY_I}},
    {U'ô', {0, KEY_CIRCUMFLEX, 0, KEY_O}},
    {U'û', {0, KEY_CIRCUMFLEX, 0, KEY_U}},
    
    {U'ä', {LSHIFT, KEY_CIRCUMFLEX, 0, KEY_A}},
    {U'ë', {LSHIFT, KEY_CIRCUMFLEX, 0, KEY_E}},
    {U'ï', {LSHIFT, KEY_CIRCUMFLEX, 0, KEY_I}},
    {U'ö', {LSHIFT, KEY_CIRCUMFLEX, 0, KEY_O}},
    {U'ü', {LSHIFT, KEY_CIRCUMFLEX, 0, KEY_U}},
	
	{U'à', {ALT_GR, KEY_7, 0, KEY_A}},
    {U'è', {ALT_GR, KEY_7, 0, KEY_E}},
    {U'ì', {ALT_GR, KEY_7, 0, KEY_I}},
    {U'ò', {ALT_GR, KEY_7, 0, KEY_O}},
    {U'ù', {ALT_GR, KEY_7, 0, KEY_U}},

	{U'ã', {ALT_GR, KEY_2, 0, KEY_A}},
    {U'õ', {ALT_GR, KEY_2, 0, KEY_O}},
	{U'ñ', {ALT_GR, KEY_2, 0, KEY_N}},

    {U'Â', {0, KEY_CIRCUMFLEX, LSHIFT, KEY_A}},
    {U'Ê', {0, KEY_CIRCUMFLEX, LSHIFT, KEY_E}},
    {U'Î', {0, KEY_CIRCUMFLEX, LSHIFT, KEY_I}},
    {U'Ô', {0, KEY_CIRCUMFLEX, LSHIFT, KEY_O}},
    {U'Û', {0, KEY_CIRCUMFLEX, LSHIFT, KEY_U}},

    {U'Ä', {LSHIFT, KEY_CIRCUMFLEX, LSHIFT, KEY_A}},
    {U'Ë', {LSHIFT, KEY_CIRCUMFLEX, LSHIFT, KEY_E}},
    {U'Ï', {LSHIFT, KEY_CIRCUMFLEX, LSHIFT, KEY_I}},
    {U'Ö', {LSHIFT, KEY_CIRCUMFLEX, LSHIFT, KEY_O}},
    {U'Ü', {LSHIFT, KEY_CIRCUMFLEX, LSHIFT, KEY_U}},

	{U'À', {ALT_GR, KEY_7, LSHIFT, KEY_A}},
    {U'È', {ALT_GR, KEY_7, LSHIFT, KEY_E}},
    {U'Ì', {ALT_GR, KEY_7, LSHIFT, KEY_I}},
    {U'Ò', {ALT_GR, KEY_7, LSHIFT, KEY_O}},
    {U'Ù', {ALT_GR, KEY_7, LSHIFT, KEY_U}},

	{U'Ã', {ALT_GR, KEY_2, LSHIFT, KEY_A}},
    {U'Õ', {ALT_GR, KEY_2, LSHIFT, KEY_O}},
	{U'Ñ', {ALT_GR, KEY_2, LSHIFT, KEY_N}},
    
    // Other special characters
    {U'¤', {ALT_GR, KEY_USD, 0, 0}},
    {U'µ', {LSHIFT, KEY_ASTERISK, 0, 0}},
    {U'²', {0, KEY_SQUARE, 0, 0}},
    {U'§', {LSHIFT, KEY_EXCLAMATION, 0, 0}},
    {U'°', {LSHIFT, KEY_DEGREE, 0, 0}},
	
	// Substitued characters
	{U'“', {0, KEY_3, 0, 0}},
	{U'”', {0, KEY_3, 0, 0}},
	{U'«', {0, KEY_3, 0, 0}},
	{U'»', {0, KEY_3, 0, 0}},
	{U'¨', {0, KEY_3, 0, 0}},
	
	{U'’', {0, KEY_4, 0, 0}},
    {U'‘', {0, KEY_4, 0, 0}},
    {U'`', {0, KEY_4, 0, 0}},
	{U'´', {0, KEY_4, 0, 0}},
	
	{U'‚', {0, KEY_COMMA, 0, 0}},
	{U'¸', {0, KEY_COMMA, 0, 0}},
	{U'„', {0, KEY_COMMA, 0, KEY_COMMA}}, 
	
	{U'›', {LSHIFT, KEY_COMPARE, 0, 0}},
	{U'‹', {0, KEY_COMPARE, 0, 0}}, 
	
	{U'•', {0, KEY_ASTERISK, 0, 0}},
	{U'–', {0, KEY_6, 0, 0}}, 
	{U'—', {0, KEY_6, 0, 0}}, 
	
	{U'œ', {0, KEY_O, 0, KEY_E}}, 
	{U'Œ', {LSHIFT, KEY_O, LSHIFT, KEY_E}}, 
	{U'Æ', {LSHIFT, KEY_A, LSHIFT, KEY_E}},
	{U'æ', {0, KEY_A, 0, KEY_E}},
	
	{U'×', {0, KEY_X, 0, 0}},
	
	{U'Á', {LSHIFT, KEY_A, 0, 0}},
	{U'Å', {LSHIFT, KEY_A, 0, 0}},
	{U'É', {LSHIFT, KEY_E, 0, 0}},
	{U'Í', {LSHIFT, KEY_I, 0, 0}},
	{U'Ð', {LSHIFT, KEY_D, 0, 0}},
	{U'Ó', {LSHIFT, KEY_O, 0, 0}},
	{U'Ø', {LSHIFT, KEY_0, 0, 0}},
	{U'Þ', {LSHIFT, KEY_T, LSHIFT, KEY_H}},
	{U'ß', {0, KEY_S, 0, KEY_S}},
	{U'á', {0, KEY_A, 0, 0}},
	{U'í', {0, KEY_I, 0, 0}},
	{U'ð', {0, KEY_D, 0, 0}},
	{U'ó', {0, KEY_O, 0, 0}},
	{U'ø', {0, KEY_0, 0, 0}},
	{U'ý', {0, KEY_Y, 0, 0}},
	{U'ÿ', {0, KEY_Y, 0, 0}},
	{U'þ', {0, KEY_T, 0, KEY_H}},
	{U'Ý', {0, KEY_Y, 0, 0}},
	{U'å', {0, KEY_A, 0, 0}},
	{U'÷', {LSHIFT, KEY_COLON, 0, 0}},
	{U'³', {0, KEY_CIRCUMFLEX, 0, KEY_3}},
	{U'ª', {0, KEY_CIRCUMFLEX, 0, KEY_A}},
	{U'¦', {ALT_GR, KEY_6, 0, 0}},
	{U'¥', {LSHIFT, KEY_Y, 0, 0}},
	{U'¢', {0, KEY_C, 0, KEY_T}},
	{U'¡', {0, KEY_EXCLAMATION, 0, 0}},
	{U'Ÿ', {LSHIFT, KEY_Y, 0, 0}},
	{U'ž', {0, KEY_Z, 0, 0}},
	{U'š', {0, KEY_S, 0, 0}},
	{U'™', {LSHIFT, KEY_T, LSHIFT, KEY_M}},
	{U'00A0', {0, KEY_SPACE, 0, 0}}, // NBSP U+00A0 160
};

// ÁÅÇÉÍÐÑÓØÚÞßáíðñóøýÿþ  Ý å÷ ³ª¦¥¢¡Ÿžš™
static const size_t keymapSize = sizeof(keymap) / sizeof(KeymapEntry);


// Report IDs:
#define KEYBOARD_ID 0x01

static const uint8_t _hidReportDescriptor[] = {
  USAGE_PAGE(1),      0x01,          // USAGE_PAGE (Generic Desktop Ctrls)
  USAGE(1),           0x06,          // USAGE (Keyboard)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  // ------------------------------------------------- Keyboard
  REPORT_ID(1),       KEYBOARD_ID,   //   REPORT_ID (1)
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0xE0,          //   USAGE_MINIMUM (0xE0)
  USAGE_MAXIMUM(1),   0xE7,          //   USAGE_MAXIMUM (0xE7)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 1 byte (Reserved)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE (8)
  HIDINPUT(1),        0x01,          //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x05,          //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  USAGE_PAGE(1),      0x08,          //   USAGE_PAGE (LEDs)
  USAGE_MINIMUM(1),   0x01,          //   USAGE_MINIMUM (0x01) ; Num Lock
  USAGE_MAXIMUM(1),   0x05,          //   USAGE_MAXIMUM (0x05) ; Kana
  HIDOUTPUT(1),       0x02,          //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 3 bits (Padding)
  REPORT_SIZE(1),     0x03,          //   REPORT_SIZE (3)
  HIDOUTPUT(1),       0x01,          //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x06,          //   REPORT_COUNT (6) ; 6 bytes (Keys)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE(8)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
  LOGICAL_MAXIMUM(1), 0x65,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
  USAGE_MAXIMUM(1),   0x65,          //   USAGE_MAXIMUM (0x65)
  HIDINPUT(1),        0x00,          //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
};

BleKeyboard::BleKeyboard(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) : hid(0)
{
  this->deviceName = deviceName;
  this->deviceManufacturer = deviceManufacturer;
  this->batteryLevel = batteryLevel;
}

void BleKeyboard::begin(void)
{
  NimBLEDevice::init(deviceName);
  BLEDevice::setSecurityAuth(true, true, false);

  NimBLEServer *pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(this);

  hid        = new NimBLEHIDDevice(pServer);
  inputKeyboard = hid->getInputReport(KEYBOARD_ID); // <-- input REPORTID from report map
  outputKeyboard = hid->getOutputReport(KEYBOARD_ID);
  outputKeyboard->setCallbacks(this);
  
  hid->setManufacturer(deviceManufacturer);
  hid->setPnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->setHidInfo(0x08, 0x01); // hid->setHidInfo(0x00, 0x01); 0x08 for FR, not sure it has any impact
  hid->setReportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  hid->startServices();

  NimBLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->getHidService()->getUUID());
  pAdvertising->start();
  hid->setBatteryLevel(batteryLevel);
}

void BleKeyboard::end(void)
{
}

bool BleKeyboard::isConnected(void) const {
  return connected;
}

void BleKeyboard::setBatteryLevel(uint8_t level) {
  this->batteryLevel = level;
  if (hid != 0)
    this->hid->setBatteryLevel(this->batteryLevel);
}

void BleKeyboard::sendReport(KeyReport* keys)
{
  if (this->isConnected())
  {
    this->inputKeyboard->setValue((uint8_t*)keys, sizeof(KeyReport));
    this->inputKeyboard->notify();
  }
}

void BleKeyboard::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
    connected = true;
    if (connectCallback) connectCallback();
}

void BleKeyboard::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
    connected = false;
    if (disconnectCallback) disconnectCallback();
}

void BleKeyboard::onConnect(Callback cb) {
    connectCallback = cb;
}

void BleKeyboard::onDisconnect(Callback cb) {
    disconnectCallback = cb;
}

void BleKeyboard::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
  uint8_t* value = (uint8_t*)(pCharacteristic->getValue().c_str());
  ESP_LOGI(LOG_TAG, "special keys: %d", *value);
}



// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.

// private method to find and place a usage ID in the report
bool BleKeyboard::addKeyToReport(uint8_t usage_id)
{
    for (int i = 0; i < 6; i++) {
        if (_keyReport.keys[i] == usage_id) { // Key already in report
            return true;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (_keyReport.keys[i] == 0x00) { // Found empty slot
            _keyReport.keys[i] = usage_id;
			vTaskDelay(10);
            return true;
        }
    }
    return false; // No empty slot
}

// private method to remove a usage ID from the report
void BleKeyboard::removeKeyFromReport(uint8_t usage_id)
{
    for (int i = 0; i < 6; i++) {
        if (_keyReport.keys[i] == usage_id) {
            _keyReport.keys[i] = 0x00;
        }
    }
}

// press() for UNICODE characters.
size_t BleKeyboard::press(uint8_t k)
{
    for (size_t i = 0; i < keymapSize; i++) {
        if (keymap[i].unicode == k) {
            const KeyPressSequence* seq = &keymap[i].sequence;
            
            _keyReport.modifiers |= seq->modifiers1;
            
            if (seq->key1 != 0 && !addKeyToReport(seq->key1)) {
                setWriteError();
                return 0; // Report is full
            }
            
            sendReport(&_keyReport);
            return 1;
        }
        vTaskDelay(10);
    }
    
    setWriteError();
    return 0; // Character not in map
}

// press() for Modifier Keys
size_t BleKeyboard::press(ModifierKey k)
{
    _keyReport.modifiers |= (1 << static_cast<uint8_t>(k));
    sendReport(&_keyReport);
	vTaskDelay(10);
    return 1;
}

// press() for Special Keys
size_t BleKeyboard::press(SpecialKey k)
{
    if (!addKeyToReport(static_cast<uint8_t>(k))) {
        setWriteError();
        return 0; // Report is full
    }
    sendReport(&_keyReport);
	vTaskDelay(10);
    return 1;
}

// release() for UNICODE characters
size_t BleKeyboard::release(uint8_t k)
{
    for (size_t i = 0; i < keymapSize; i++) {
        if (keymap[i].unicode == k) {
            const KeyPressSequence* seq = &keymap[i].sequence;
            
            _keyReport.modifiers &= ~seq->modifiers1;
            
            if (seq->key1 != 0) {
                removeKeyFromReport(seq->key1);
            }
            
            sendReport(&_keyReport);
            return 1;
        }
    }
    vTaskDelay(10);
    return 0; // Character not in map
}

// release() for Modifier Keys
size_t BleKeyboard::release(ModifierKey k)
{
    _keyReport.modifiers &= ~(1 << static_cast<uint8_t>(k));
    sendReport(&_keyReport);
    return 1;
}

// release() for Special Keys
size_t BleKeyboard::release(SpecialKey k)
{
    removeKeyFromReport(static_cast<uint8_t>(k));
    sendReport(&_keyReport);
    return 1;
}

size_t BleKeyboard::tap(SpecialKey k)
{
    press(k);
    delay(10); 
    return release(k);
}

void BleKeyboard::releaseAll(void)
{
	_keyReport.keys[0] = 0;
	_keyReport.keys[1] = 0;
	_keyReport.keys[2] = 0;
	_keyReport.keys[3] = 0;
	_keyReport.keys[4] = 0;
	_keyReport.keys[5] = 0;
	_keyReport.modifiers = 0;
	sendReport(&_keyReport);
	vTaskDelay(10);
}

size_t BleKeyboard::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    for (size_t i = 0; i < size; ) {
        uint32_t unicode_char = 0;
        int len = 0;
        unsigned char c = buffer[i];

        if (c < 0x80) { // 1-byte
            unicode_char = c;
            len = 1;
        } else if ((c & 0xE0) == 0xC0 && i + 1 < size) { // 2-byte
            unicode_char = ((buffer[i] & 0x1F) << 6) | (buffer[i+1] & 0x3F);
            len = 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < size) { // 3-byte
            unicode_char = ((buffer[i] & 0x0F) << 12) | ((buffer[i+1] & 0x3F) << 6) | (buffer[i+2] & 0x3F);
            len = 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < size) { // 4-byte
             unicode_char = ((buffer[i] & 0x07) << 18) | ((buffer[i+1] & 0x3F) << 12) | ((buffer[i+2] & 0x3F) << 6) | (buffer[i+3] & 0x3F);
            len = 4;
        } else { // Invalid sequence, skip
            i++;
            continue;
        }

        typeUnicodeCharacter(unicode_char);
        i += len;
        n++;
        delay(_delay);
    }
    return n;
}

size_t BleKeyboard::write(uint8_t c) {
    return write(&c, 1);
}

/**
 * @brief Private helper to type a single Unicode character using the keymap.
 */
void BleKeyboard::typeUnicodeCharacter(uint32_t unicode_char) {
    for (size_t i = 0; i < keymapSize; i++) {
        if (keymap[i].unicode == unicode_char) {
            const KeyPressSequence* seq = &keymap[i].sequence;

            // Check if it's a sequence (e.g., dead key)
            if (seq->key1 != 0 && seq->key2 != 0) {
                // First key press of the sequence
                pressRaw(seq->key1, seq->modifiers1);
                vTaskDelay(10); // Use a short, fixed delay for reliability
                releaseAll();
                vTaskDelay(10);

                // Second key press of the sequence
                pressRaw(seq->key2, seq->modifiers2); // Use the new modifier for the second key
                vTaskDelay(10);
                releaseAll();
            } else { // Single keypress
                uint8_t key_to_press = seq->key1;
                if (key_to_press != 0) {
                    pressRaw(key_to_press, seq->modifiers1);
                    vTaskDelay(10);
                    releaseAll();
                }
            }
            return; // Found and typed the character
        }
    }
}

void BleKeyboard::setDelay(uint32_t ms) {
  this->_delay = ms;
}

void BleKeyboard::debug(uint8_t usage_id, uint8_t modifiers)
{
  if (this->isConnected())
  {
    // Prepare the key report
    _keyReport.modifiers = modifiers;
    _keyReport.keys[0] = usage_id;

    // Send the key press
    sendReport(&_keyReport);
    vTaskDelay(10); // A small delay

    // Release the key
    releaseAll();
  }
}

size_t BleKeyboard::pressRaw(uint8_t usage_id, uint8_t modifiers) {
    _keyReport.modifiers = modifiers;
    if (usage_id != 0 && !addKeyToReport(usage_id)) {
        return 0; // Report full
    }
    sendReport(&_keyReport);
    return 1;
}
