# **Solder Reflow Plate - 70mm by 50mm**

## How-To - Programming the Board
This Solder Reflow Plate has been specifically designed to be Arduino based and easily programmed through the Arduino IDE or any other similar environment.
For ease, here is a guide to programming the board via the Arduino IDE.

### Preparation
1) Download and install the Arduino IDE from the [Arduino Website](https://www.arduino.cc/). The sketch was originally developed on version 1.8.19, though any recent and all future versions of the Arduino IDE should be fully supported.
2) Install the MiniCore additional boards to allow programming of bare Atmega (8bit) chips. Go to 'File->Preferences' and copy the '.json' link below into the 'Additional Boards Manager URLs' textbox. If you have multiple sources, they must be separated by commas. 
https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
3) Add the boards to the board manager by searching for and installing MiniCore in 'Tools->Board->Board Manager'.
4) Search for and install Adafruit_GFX and Adafruit_SSD1306 libraries in 'Tools->Manage Libraries'.

### Program
1) Connect your programmer to the ICSP headers on the Solder Reflow Board. This can be any supported USBISP, USBasp, or even an Arduino programmed with Arduino as ISP. If using a commercial programmer make sure your drivers are installed. If using another Arduino make sure to upload the Arduino as ISP sketch and correctly connect the ISCP pins. Use [this](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP) guide in the Arduino docs if you are unsure.
2) Select the following settings under the 'Tools' tab
- Board -> MiniCore -> Atmega328
- Clock -> Internal 8MHz
- BOD -> BOD 2.7V
- EEPROM -> EEPROM retained
- Compiler LTO -> LTO Disabled
- Variant -> 328P / 328PA
- Bootloader -> No bootloader
3) Set the programmer of your choice, e.g. 'Arduino as ISP (MiniCore)', 'USB ASP', etc, and set the correct port.
4) Burn bootloader to the Solder Reflow Board. This sets fuses and registers correctly so that the sketch needs not directly deal with them.
5) Compile and upload to the Solder Reflow Board.

### PWM Frequency

By default, the PWM control of the heating element operates at the default PWM frequency used by Arduinos of around 490Hz. This can be increased to roughly 63kHz by uncommenting the definition of the FAST_PWM macro. Note that with current hardware designs, this will likely result in overheating of the input capacitor. The circuit requires redesign to correct this fault. If FAST_PWM is enabled, this will also modify the messages on the screen to remind the user that the input capacitor may get hot.
