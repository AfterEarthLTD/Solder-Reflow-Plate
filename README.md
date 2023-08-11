# **Working with an ATmega4809**

https://github.com/gotnull/Solder-Reflow-Plate/blob/main/Board%20Versions/70mm%20by%2050mm%20Ver3.0%20ATmega4809/Software/SW1.0_HW3.0_70by50mm.ino

**To Prepare**

1. Include MiniCoreX in additional boards via the Arduino IDE Boards Manager: https://mcudude.github.io/MegaCoreX/package_MCUdude_MegaCoreX_index.json
3. Install Bounce2, Adafruit_GFX and Adafruit_SSD1306 libraries (Tools->Manage Libraries)

**To Program**

1. Select the following settings under (Tools)

![Screenshot 2023-08-11 at 10 29 06 am](https://github.com/AfterEarthLTD/Solder-Reflow-Plate/assets/578908/4205c012-56f0-435f-a444-30ac8e175e1f)

- Board->MegaCoreX->ATmega4809
- Clock->Internal 16MHz
- BOD->BOD 2.6V
- EEPROM->EEPROM retained
- Pinout->48 pin standard
- Reset pin->Reset
- Compiler LTO->LTO Disabled
- Bootloader->No bootloader

**Compile to ATmega4809**

I followed this guide here: https://swharden.com/blog/2022-12-09-avr-programming/

**Specifically this setup:**

![updi-ftdi-serial-programmer](https://github.com/AfterEarthLTD/Solder-Reflow-Plate/assets/578908/fdc413e5-4882-4d11-b5a2-2d483f75b10d)

![unnamed (2)](https://github.com/AfterEarthLTD/Solder-Reflow-Plate/assets/578908/0cbd3a76-353f-4082-be97-bd00825eeeec)

Connect the programmer as shown with the RX pin directly to the UPDI pin of the microcontroller and the resistor between the RX and TX pins. IMPORTANT: The Reflow Plate must be powered externally via 12V with a minimum of 5A during this process.

I used a 10KΩ 0.5W resistor instead. This is even closer to the recommended value. 10KΩ is a common pull-up resistor value used in many electronics applications. It should work well for the UPDI interface.

Ensure a [modern version of Python](https://www.python.org/) is installed on your system

`pip install pymcuprog`

Use the device manager to identify the name of the COM port representing your programmer. In my case it was `/dev/tty/usb.serial-AH00XXXX` on MacOS.

I then interacted with the microcontroller by running `pymcuprog` from a terminal. This command returns the Device ID (1E9651 for my ATmega4809) indicating the UPDI connection is working successfully.

`pymcuprog ping -d atmega4809 -t uart -u /dev/tty.usbserial-AH00XXXX`

> Connecting to SerialUPDI
> Pinging device...
> Ping response: 1E9651
> Done.

I used Arduino IDE to compile my C code and generate the hex file (turn on verbose output during compilation to know the location of your `.ino.hex` file). Now you can use `pymcuprog` to load the hex file onto the chip. It’s slower to program and inconvenient to drop to a terminal whenever you want to program the chip, but it works.

`pymcuprog write -f SW1.0_HW3.0_70by50mm.ino.hex -d atmega4809 -t uart -u /dev/tty.usbserial-AH00XXXX --erase`

> Connecting to SerialUPDI
> Pinging device...
> Ping response: 1E9651
> Erasing device before writing from hex file...
> Writing from hex file...
> Writing flash...
> Done.

# **Solder Reflow Plate**

###### By AfterEarth Ltd

## As Seen On GreatScott! 

This project has recently featured on a DIY or BUY GreatScott! video.

Check it out here - [The Best way to reflow solder? Hot Plate to the rescue! DIY or Buy](https://www.youtube.com/watch?v=QarizoUnRfk)

## UPDATE!!!

Join the [DISCORD!](https://discord.gg/YzhG6FcCRA)

Checkout the new Ver 3.0 design. Very similar to Ver 2.4 but now comes with ATmega4809 as its availability is much better than the 328p at the moment. Code is still in development but the board designs are available in the Board Versions Folder.

## Versions

- 70mm x 50mm (Ver 2.4)
  - 12VDC (5A Minimum)
  - 5.5mm x 2.5mm Barrel Jack
  - 0.91" OLED Display
  - ATmega328p Microprocessor

- 70mm x 50mm (Ver 3.0) **NEW**
  - 12VDC (5A Minimum)
  - 5.5mm x 2.5mm Barrel Jack
  - 0.91" OLED Display
  - ATmega4809 Microprocessor

## About the Project

Like any good nerd does I've been expanding my hobby horizons.
I've gone from breadboard, to perf board, through-hole, to SMD, ever increasing circuit complexity and improving my knowledge.
But I've become frustrated with the difficulty of SMD soldering.
Regular soldering jogs small components and needs lots of flux.
Hot air damages plastics.
Hot plates are expensive and can be big wastes of space.

*"But you're a nerd"* I hear you cry, *"why don't you DIY a solution?"*.

[Electronoobs](https://www.youtube.com/channel/UCjiVhIvGmRZixSzupD0sS9Q) shows a great DIY solution using an old clothes iron hot plate.
You can watch his video [here.](https://www.youtube.com/watch?v=C7blZigaaaA)
Have a go!

While I love this solution, I wanted to develop something smaller and more permanent.
Hence the Solder Reflow Plate was born, a PCB-based, MOSFET controlled heater, perfect for easily reflowing SMD components giving surprisingly professional results. Furthermore, these results are achieved without breaking the bank or taking up an enormous amount of desk space.

And so, this nerd bestows upon the world his project, fully open-source and hackable.
Here's hoping that it may be used to develop skills, build custom circuit boards, and make quality designs possible to all!

## Purchase

- Register your email address at [www.solderreflowplate.co.uk](https://www.solderreflowplate.co.uk) to know when it is available to buy!
- There is a worldwide shortage of ATmega328p Microcontrollers. I'm currently working on a small redesign for a more readily available chip.
- Purchase will be available soon!

## License

All parts of this project are covered by a [MIT license](LICENSE).
That means it is entirely open-source, so you get access to everything! (just without warranty or liability)
Feel free to download, customise, hack, and commercialise to your nerdy heart's content.

Want the **easy** life?!
**Purchase** the default design above!
Its fully functional, cheaper than customising, and helps to feed the creator's caffeine addiction 🙃
