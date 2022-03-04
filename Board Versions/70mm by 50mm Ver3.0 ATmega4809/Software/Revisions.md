# **Software Revisions**
###### 70*50mm Solder Reflow plate

## Note
Software not yet complete for this board version. I have several boards on the way, due late March. Software should take a day or two to convert from the original HW Ver2.4 design. It will have all the same feature-set and should be indistinguishable from the original design.

## S/W Rev 1.0
###### H/W Rev 3.0
This is the first complete software revision and has been developed and finalised for hardware version 2.4. The user interface and feature scope of this project was large for the size of board. This initial release contains the following main pages and features...

#### Start-up (contained within setup() function)
- Duration 3 seconds
- Displays Logo
- Displays S/W and H/W versions

#### Main Menu
- Swap display on the left explains user interface
- Temperature and Board Thickness data on the right
- Available temperatures are 140C, 150C, 160C, 170C, 180C, 190C, 200C
- Available thicknesses are 0.4mm, 0.6mm, 0.8mm, 1.0mm, 1.2mm, 1.6mm, 2.0mm
- Push individual buttons to cycle through settings
- Setting saved to EEPROM and loaded from EEPROM on start-up
- Hold both buttons for 100ms, then release, to enter heating mode

#### Heating
- Pauses for 2 seconds before enabling MOSFET, during which time it displays the maximum temperature and board thickness the user has selected
- Displays current input voltage and plate temperature
- Voltage and temperature readings are averaged from 20 consequtive samples
- Heating animation
- Heating process follows the industry standard heating profile, climbing slowly to 75% of the maximum temperature, briefly peaking to the maximum temperature, then beginning cooldown
- PWM value will be held lower if the input voltage falls below a minimum voltage (10.5V)
- Once the plate temperature reaches the user specified maximum this temperature will be held for the board thickness in mm multiplied for 5 seconds. E.g. 2.0mm thickness will be held at peak temp for 10s to allow the peak temp to propogate throughout the board
- If process is not complete within 8 minutes the heating process is cancelled due to 'time out'
- Process can be cancelled by pressing any button

#### Cool Down
- Heating completion triggers cool down screen
- Shows current temperature
- Screen will automatically show completed screen if temperature falls below minimum value (45C)

#### Completed
- Shows that the heating process is complete
- Board now cool enough to handle

#### Cancelled (User Button Pressed)
- Occurs when button pressed during heating process
- Press any button to return to main menu

#### Cancelled (Time Out)
- Occurs when heating process takes longer than 5 minutes to complete
- Press any button to return to main menu
- Suggests using a power supply with a higher maximum current