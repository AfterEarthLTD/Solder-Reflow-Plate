# **Solder Reflow Plate - 70mm by 50mm**

## How-To - Designing a board
Designing a solder reflow board is relatively simple. They consist of
- A low resistance heating element (0.5 - 3ohms)
- A power semiconductor, commonly a MOSFET
- Control circuitry
- Suitably powerful power supply
- Filtering and final touches
Please use the designs provided either in their entirety or as a starting point for your own projects.

#### Heating Element
The heating element on the 70*50mm Ver2.4 solder reflow plate is a 1.4mm single ounce track over a length of 2000mm. It is laid out in a zig-zag grill shape with 0.4mm spacing between lines. This gives the heater element a cold resistance of approximately 0.9ohms and allows it to easily reach a temperature of 200C at only 8.4V. For smaller or larger element sizes these values change so please see the other sized boards for guidance, or experiment for yourself.

#### Power Semiconductor
There are several options for types of semiconductors that could be used in an application such as this, however the most sensible option is a MOSFET.
MOSFETs are easily PWM controlled by low voltage microcontrollers and can provide massive continuous load currents with a very low drain to source resistance and therefore create only small heat losses. The MOSFET chosen for the 70*50mm solder reflow plate is the [UMW30N06](https://lcsc.com/product-detail/MOSFETs_span-style-background-color-ff0-UMW-span-Youtai-Semiconductor-Co-Ltd-UMW30N06_C369599.html) in the package TO-252. This chip is capable of up to 25A continuous current and is plenty strong enough to provide current to the heater element, even at 100% PWM at 12VDC, the board's rated voltage. This MOSFET was mainly chosen for its abundance in the supplier LCSC, as recommended by [EasyEDA](https://easyeda.com/) (online circuit design software) and [JLCPCB](https://jlcpcb.com/) (board manufacturer). However, nearly any TO-252 should be a direct drop-in replacement. Simply ensure that
- The pin layout is the same 
- The continuous current rating is a little over the maximum current your heater element can draw (CurrentContinuous = Vsupply / HeaterResistance)

IGBTs are also a suitable type of semiconductor, though they tend to be more expensive and likely over-specced for this application.

BJTs could also be used but will be more difficult to drive (lower gain values), will come with a much higher heat loss, and require larger sizes to sustain high constant currents. 

I strongly suggest sticking to MOSFETs for this application.

#### Control Circuitry
This is the part of the board that gives you the most freedom. Controlling a MOSFET is as simple as providing a voltage between the gate and source pins greater than that of the minimum threshold but less than the maximum threshold. This function could be achieved by anything from a basic pushbutton and pull-down resistor to a multi-million pound/dollar/euro super-computer. The level of complexity you choose relies heavily on your aspirations for the feature-set.
I designed the 70*50mm solder reflow plate with an Atmega328p as its brain. This is the same microcontroller found on many Arduino and Arduino compatible boards, and so is very easy to program and implement into custom designs. I wanted to include an OLED display, two user push buttons, temperature and voltage sensing, on top of the PWM MOSFET control. Smaller microcontrollers could easily achieve this but the program I wrote could only be accommodated by a microcontroller with larger flash storage.
For ultra-simple control try a 555 timer, a potentiometer, or even a PWM pin on a separate Arduino development board.
For a more complicated build try multiple temperature measurements, splitting the heater element into 'zones' for heater size control, or implementing USB or IoT control for implementation into a production line.

Feel free to use the designs within this Git page to help develop your own solutions. 

More on this solution
- OLED, is the common 0.91" 128x32 pixel display. Connects to the microcontroller via I2C and easily implemented with the [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) and [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) Arduino libraries.
- Temperature Sensing, is done by a [LMT85LP](https://lcsc.com/product-detail/Temperature-Sensors_Texas-Instruments-LMT85LP_C75856.html). This component outputs a voltage inversely proportional to the temperature. This is read by an analogue-in pin on the 328p. While only rated to 150C, it is accurate enough to infer plate temperatures up to 200C with an accuracy of +-10C. Not a high accuracy but cheaper and less complicated than incorporating a thermocouple.
- Voltage sensing, is done by a simple voltage divider that quarters the input voltage. This is also read by an analogue-in pin on the 328p. Resistor tolerance leads to an accuracy variation of +-0.2V from board to board.
- MOSFET PWM, is handled by a PWM capable pin and written to using the analogWrite() Arduino function. This pin is directly connected to the gate of the MOSFET with the only connected components being a 10k pulldown resistor and a filtering capacitor. 

#### Power Supply
Throughout development I have used a large 12V 20A power supply from an old 3D printer and connected using a 6.2mm by 3mm DC barrel jack. This has been perfect for providing a consistent 12V for reliable board testing. The 70*50mm design of board can pull a maximum of 5A, leading to a maximum power use of 60W. This is large but essential in order to provide enough heat to the board under reflow. 
It is important to ensure that the power supply you choose can provide the continuous current required to run your design and over the time period it is required to be run for. 
In my personal experience, lower powered variable bench power supplies may not be capable of providing the current required. In this case consider a different source of power or redesign your board to function at a higher voltage level while reducing the MOSFET duty cycle to keep the heater element power consistent.
Also, your microcontroller will need an appropriate power supply. This can commonly be derived from the 12V or similar main input supply. In my design I have a small SOT-89 5V regulator rated to 1A.

#### Filtering and Final Touches
I've learnt a lot from the development of this board, through its many revisions. Some of my troubles include running out of flash memory, heat spreading to the control circuitry, large voltages seen across the heating element, exceeding fuse current ratings, and so on. Thus, I present to you my lessons in the hope to save you some of the trouble that I faced.
- Choose an appropriate fuse or do not add one to your board. Too weak a fuse can lead to failure due to loss of power under high-current situations. Depending on your power supply, you should be able to rely on their current limiting features or their internal fuses to protect against overcurrent.
- Choose a microcontroller with sufficient flash memory to hold your program. When I added the animations and logos to my program it dramatically increased the program size to over 21kB. If you are designing a very basic control, then you can get away with cheaper/smaller microcontrollers. If you need logos and animations, stick with the 328p or choose an equivalent.
- Preventing heat spreading. You will notice two large slots in the solder reflow plate to limit the heat transfer from the heater element to the control circuit. These are not essential but recommended to improve the life of your components. 
- Large positive voltage spikes can be observed across the heater element when the MOSFET de-energises. The 70*50mm design produced short _____nS peaks of approximately 80V. To protect the surrounding circuitry, it is important to filter this effect. I achieved this by a combination of a 19V Zener Diode and a 1nF ceramic capacitor connected in parallel across the heater element. 
- Temperature sensing is better if part of the heater element copper is extended out to surround the footprint of the temperature sensor. This couples the sensor closer to the element and can result in more accurate measurements.