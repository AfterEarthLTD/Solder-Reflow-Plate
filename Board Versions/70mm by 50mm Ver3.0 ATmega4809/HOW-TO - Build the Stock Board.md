# **Solder Reflow Plate - 70mm by 50mm**

## How-To - Build the Stock Board
Building this solder reflow plate is a intermediate level task.
While not overly complicated it still features many small surface mount components and a few polarity sensitive components.
Future versions of the board are being developed to make home building easier to accomplish.

## WARNING!!!

This design lacks safety features to limit the amount of power it may draw or the temperature it may reach and there are other issues with the electrical design. With a suitable power supply and as long as the software doesn't have any major bugs, it can be operated without issue but there is a real risk that it could start a fire in the event of a software bug or other fault. Do not leave it unsupervised while connected to power.

#### Preparations
Firstly, download the Gerber files and Bill of Materials.
While you are there, if you download the Pick and Place Doc you can have select board manufacturers populate the boards for you, normally at extra cost.
Find yourself a board manufacturer and upload the Gerber files and Bill of Materials.
I have used JLCPCB and NextPCB, both have given great quality boards and I've never had a problem with either.
Use the default board settings
- PCB thickness = 1.6mm
- Copper Weight = 1oz
- Material = FR-4
- Colour to your liking
It can make life easier picking up a stencil to help you solder the components on later.
Order as many boards as you like ensuring you order the materials too.
Delivery from China nrormally takes between 2 and 6 weeks.

Some other bits to consider
- You will need to buy a 0.91" OLED seperately. Ebay or Amazon are normally good for this. Make sure to get the 4 pin I2C version.
- You will need a 12V power supply. Minimum of 5A (take a lot of power to heat to 180C). An enclosed power brick with a 2.5mm inner diameter barrel jack is a good option. Or you can rig one yourself from an open supply like [these](https://www.ebay.co.uk/itm/164636015985?_trkparms=ispr%3D1&hash=item2655123d71:g:0~EAAOSwWe9f-qaI&amdata=enc%3AAQAGAAACkPYe5NmHp%252B2JMhMi7yxGiTJkPrKr5t53CooMSQt2orsSLY2M1Gjmuwt9c03vWNfiRs%252FQ%252FM%252BYAJ6j8WnNhVqBTnBTru6OB86J8pc%252FYfL%252FNaBizpiin5VQ2FRoMM2xlSMmLikPq1Kfo827DnvZ98oP5KQz5lhOmHCrLavgEdachmpNk6IFLLp0VfjgkVc%252Fs%252B%252BX8KFBqiDdz4rX9wcFb7PwH1fO8ZEtIwpVbAxrW3%252FafmJJ18bdW%252BrzPZTBljdLcVAzgt7L33U2%252BnDSJ5JRD%252BA%252FmCWPD5Ah6XsDLI%252BsRJa7Xu6sBtNsFRM2CBmXp9DI6UX8qZYagrwBZa%252FjGHZTInWGRK3dJPABW%252Beq5G3GBKLwihaUJqCC5VI3fRwt%252B4pYKBpKseUG0vriRotickOTQR46RbQNNKgE4Wmf0QthOdwjOadBEksewf0Sqfs8FhBQd5ILciZSYxv%252F9MTtMLWLLqJaqGC7vd09YHPBfKJxCdZ3O0yACOYApZvqe1qnKExBBOneiSwT9%252BcrHj2LbBg53rFOsAVowVYD3uUypZ2x8WsQAM06zeZe6LoYHAKAfjmte4DfzaamhbOEAo%252FGx2hSSPkfUC8cGohhcl%252BUBomTqERhc9gly0pHWbekyhBpsr0flCOde9DuiSv3GefdP2yGA3%252FG6q34VpSfsxQUoaMNcuMXBKjUssQpRn3PitZj1Ipbor7HEPB8q3%252B8qAmTxWw5V1EUYUShj95m58kDRc9oVv3eEyryFRcMXJjavWD0leQ%252FEekao2hEheLD1U3thR%252B61JfoOMSXatZ0bMyf7NaTVmPFyDvA8AKqjWJ2jxogJ2asMWqSMCzpT92CwMjXS4NQ%252FLzdmxXwQ%252B77K20%252F9Tx%252BA2awmW1x%7Cclp%3A2334524%7Ctkp%3ABFBMmpvD9tdf), and a 2.5mm diameter barrel jack adapter. Just be careful as you will be dealing with mains (Live and Neutral) wiring so only do so if you are competent and confident.
- If you picked up a stencil make sure to get some solder paste too.
- Finally, some small hardware is good. The board has holes just bigger than M3 so standard motherboard stand-offs and similar should fit. The board will function without but lifting it off the desk is best.

#### Soldering
I strongly suggest using the stencil and soldering paste to solder this board due to the size of some of the components.
Follow the following rough guidance to get you on your way.
- Place your first board down surrounded by 4 others that are taped to the desk. This holds your working board still.
- Tape the stencil overtop so that the holes align with the pads on your working board. 
- Use a credit card or something flat to push a blob of solder paste over the stencil holes.
- Carefully remove the stencil and clean it.
- Starting with the smaller components, gently place them onto the board following the circuit drawing.
- If you have a working solder reflow board you can now reflow your working board (if any someone would develop one).
- If you have a decent lab power supply that can output 5A or better and a spare board, you could power one of your spare boards directly by attaching to the large pad of the MOSFET (the drain pad) and the lower pad of D2 (it's cathode pad). This is relatively easy if you solder some wires to those pads. Use current limiting to control heating. Slowly and cautiously increase the heat until you see the solder paste start to activate and melt. If you have a multimeter with a thermocouple or a thermal imager, some test runs to see the temperature at different current levels is advisable. Somewhere around 3-5A will probably be enough.
- If that sounds too crazy/dangerous, use a fine pointed soldering iron to dab solder the smaller components.
- More heat will be needed to solder the larger components, especially the large tab of the MOSFET.
- Check for good solder joints and that there are no shorts, especially around the microcontroller.
- Move onto the through-hole components. The switches need to be checked for polarity before fitting else they will confuse the software on first start-up. Fit them with the notch pointing towards the OLED.
- Lots of heat will be needed for the ground connections on the input jack.
- Makes sure to solder the temperature chip as close to the board as possible to ensure it best follows the temperature of the plate.

#### Finishing up
Congratulations! You've soldered a thing that will definitely maybe work on the first try!
Give the board a clean with some cleaning solvent or cleaning alcohol.
You can now move onto the programming of the board so open up that document and get cracking.
