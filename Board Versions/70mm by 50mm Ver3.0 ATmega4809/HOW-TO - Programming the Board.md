# **Solder Reflow Plate - 70mm by 50mm Ver 3.0 ATmega4809**

## How-To - Programming the Board
This Solder Reflow Plate has been specifically designed to be Arduino based and easily programmed through the Arduino IDE or any other similar environment.
For ease, here is a guide to programming the board via the Arduino IDE.
The ATmega4809 is UPDI programmed so you will need a UPDI programmer. If you are harvesting your ATMega4809 from an Arduino Nano Every or other Arduino board, you may be tempted to try to program it whilst still on the source board before transferring it. This is not recommended and will likely end up loading firmware that does not work. You shouldn't break anything. You'll just end up having to rebuild and program on the real board using UPDI.

### Preparation
1) Download and install the Arduino IDE from the [Arduino Website](https://www.arduino.cc/). The sketch was originally developed on version 1.8.19, though any recent and all future versions of the Arduino IDE should be fully supported.
2) Install the [MegaCoreX](https://github.com/MCUdude/MegaCoreX) additional boards to allow programming of bare Atmega4809 and similar microcontrollers. Follow the instructions on the [MegaCoreX repository](https://github.com/MCUdude/MegaCoreX)
3) Add the boards to the board manager by searching for and installing MegaCoreX in 'Tools->Board->Board Manager'.
4) Search for and install the Bounce2, Adafruit_GFX and Adafruit_SSD1306 libraries in 'Tools->Manage Libraries'.
5) Obtain a UPDI programmer. If you don't know what that is or you don't have one, your easiest option is probably to use an Arduino board with the [JTAG2UPDI](https://github.com/ElTangas/jtag2updi) sketch loaded onto it.

### UPDI Programming with JTAG2UPDI
If you already have a purpose built UPDI programmer and know how to use it, you can skip this section and just use your existing UPDI programmer. For the rest of us, if you have a spare Arduino board lying around, you can program it to play the role of a UPDI programmer. I used an Arduino Nano, but many other Arduino compatible boards should also work.
1) Download the [JTAG2UPDI](https://github.com/ElTangas/jtag2updi) code from Github.
2) Rename the source directory to jtag2updi to make the Arduino IDE happy.
3) Load the jtag2updi/jtag2updi.ino sketch into your Arduino IDE.
4) Connect your Arduino board via USB.
5) Select the appropriate board type via the Tools->Board menu item.
6) Select the appropriate port for you UPDI programmer Arduino board.
7) If you're using a capacitor or resistor to disable the reset, *DO NOT* have this in place yet. You won't be able to load the JTAG2UPDI sketch with that in place.
8) Compile and upload the sketch.
9) Connect your capacitor between the reset and ground pins now.
10) Connect a 4.7k resistor to the PD6 pin so it's ready to connect when you're ready to program.
You should now have a UPDI programmer ready to go.

### Program
1) Connect your UPDI programmer to the UPDI pins on the Solder Reflow board - the 3 pin header on the right side of the board. You need to connect the top pin to ground and the centre pin to the UPDI pin on your programmer (PD6 is using JTAG2UPDI). If using JTAG2UPDI, make sure you have 4.7k of resistance between the PD6 and UPDI pins.
2) You need to provide power to the Atmega4809 in order to program it. You have two options. You can connect 12V to the DC connector on the board and the 5V LDO will supply power to the MCU. Alternatively, you can connect the 5V pin on the 3-pin UPDI header to 5V on your UPDI programmer. This will also supply power to the MCU. Pick one. Don't do both.
3) Select the following settings under the Tools menu:
- Board->MegaCoreX->Atmega4809 
- Clock->Internal 16MHz
- BOD->BOD 2.6V
- EEPROM->EEPROM retained
- Pinout->48 pin Standard
- Reset Pin->Reset
- Bootloader->No bootloader
4) Set the programmer of your choice. eg. JTAG2UPDI, Atmel ICE, etc
5) Compile and upload.

