# **Solder Reflow Plate**

###### By AfterEarth Ltd

This is a largely unmodified version of the design by AfterEarthLTD that was also seen on a [GreatScott! video](https://github.com/DerSpatz/PCB-reflow-solder-heat-plate). I have made some tweaks to the software to get it to work on the ATMega4809 and also added instructions on how to build it and program it. It does work, but there are some problems with the design. I am working on newer designs but the best design out there today is probably the one by [DerSpatz](https://github.com/DerSpatz/PCB-reflow-solder-heat-plate)

## WARNING!!!

This design lacks safety features to limit the amount of power it may draw or the temperature it may reach and there are other issues with the electrical design. With a suitable power supply and as long as the software doesn't have any major bugs, it can be operated without issue but there is a real risk that it could start a fire in the event of a software bug or other fault. Do not leave it unsupervised while connected to power. If you're designing your own version, some considerations you may want to include are:

- Safety!!! What can happen with your design in the event of a hardware or software fault? Can it draw enough power to catch fire? At a minimum, some kind of overcurrent protection like a fuse would be prudent.
- Heating element resistance. The original board layouts have very low resistance that typically ends up around 0.9 ohms or so. A higher resistance that's closer to your design maximum power at 100% duty cycle would help ensure that the device cannot draw excessive currents, overheat and catch fire in the event of a fault. Since the design goal was about 60W, something closer to 2 ohms at peak temperatures would be more prudent to help prevent large overcurrent faults. For about 2.1 ohms at 180 celsius, you'd want about 1.3 ohms at 20 celsius.
- Poor regulation in the PWM and/or input circuits. The existing design may be suitable for low power scenarios like managing the brightness of small, indicator LEDs, but it's inadequate for higher power devices like a 60+W heating element.
  - Ideally, the PWM should be operating at moderately high frequencies above 30kHz. The original firmware used a frequency of roughly 63kHz, but suffered from the input capacitor getting extremely hot during hearing cycles. The quick fix for this was to lower the PWM frequency to the default frequency of 490Hz, which lowered the power dissipation in the input capacitor.
  - Without sufficient inductance in the circuit to smooth out current ripple, it's impossible to get the ripple currents in the input capacitor to manageable levels. Larger input capacitors can make marginal improvements, but mostly because their larger size allows them to dissipate more heat. Ultimately, they will still end up with large ripple currents with each on-off cycle of the MOSFET.
  - Simple retrofits, such as adding a series inductor inline with the power supply is also unable to address the issue. The position of the MOSFET switch means that when it is off there is nowhere for the inductor to discharge it's energy except into the input capacitor. Such an inline inductor makes the input capacitor power dissipation problem worse, not better.
  - What's really required is a redesign that includes an inductor, capacitor and freewheel diode connected with the heating element and the MOSFET switch situated so as to switch power on and off into these components as a unit in a manner similar to how a buck converter works. It needs to be designed such that when the switch is off, the inductor can discharge its current into the heating element. This filter arrangement should probably be separate from the input capacitor, but should ensure that the device does not excessively load the power supply and keep current ripple to a minimum.
  - The firmwares in this repo have been modified to use the FAST_PWM macro to determine whether or not to use high PWM frequency. In the current code, this define is commented out and the firmware will use the default PWM frequency of about 490Hz. Uncommenting this line will change the PWM frequency to approximately 63kHz. It also modifies the messaging on the screen to suggest that the input capacitor may get hot so that it's clear to users if they're running a FAST_PWM firmware.
  - Since the input capacitors are largely ineffectual in the current design, they could probably be removed although I have not and probably won't try this.
- Thermal gradients. The standard layout has a uniform track layout across the entire board, but since you lose heat much more easily at the edge than in the centre, thermal gradients of 50 celsius between the centre of the board and the edge are not uncommon. Having thinner tracks near the edge can help reduce this effect.
- Thermal sensing. The temperature sensor off the side of the hotplate is ineffective at accurately sensing hotplate temperature and will likely be 20-30 celsius lower than the temperature at the edge of the board, even if you install it as close to the board as possible. Ideally, the sensor setup should measure temperature within the hotplate area itself or on the board being reflowed. Suggested approaches are using the heating element itself as the sensor, using a remote sensor that can be attached to the board being reflowed or possibly using some kind of thermography or IR sensor.

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
  - 12VDC (15A Minimum)
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
