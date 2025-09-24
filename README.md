# ESP32 NimBLE Keyboard library

Changed the forked [ESP32-BLE-Keyboard](https://github.com/wakwak-koba/ESP32-NimBLE-Keyboard) to support french AZERTY.
The plan was to just changed the mapping, but due to limits in ASCII for French characters and target usage, it snowballed into UNICODE support.

Now, BleKeyboard.print handles more complexe characters, and for unsupported characters, a substitution can be made. However, it is slower than the original library.

I deleted media keys support as it is irrelevant for my use case but relevant sections can be added as is.

Current mapping can be edited for any layout, without common ascii characters limits.
