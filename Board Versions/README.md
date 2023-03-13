# **Designing Your Own Hotplate**

If you're the kind of person interested in building a project like this, you may also be the kind of person who might want to improve upon it or adjust it for a different operating environment. For example, you may want to operate from a higher voltage supply, design a larger sized hotplate or other similar options. What do we know from this design that could help inform such modifications?

My experience with hotplate design is based on designing separate hotplates (ie. without integrated electronics onboard) that fall within the size limitations of the cheap Chinese PCB manufacturers (ie. 100x100mm maximum size). The heated area of my hotplates are roughly 86mm square due to allowances for mounting the hotplate. Secondly, I am designing for reflowing traditional Tin-Lead (SnPb) solder alloys, which melt at approximately 180 celsius. If you are only intersted in reflowing lower temperature solder alloys like Tin-Bismith (SnBi) alloys, the power requirements can be lowered slightly. The common commercial lead-free alloys (ie. SAC305, SAC405, etc) are probably unsuitable for reflowing on a PCB based hotplate, so this discussion will not consider them.

## Decide on Target Heating Power ##

Calibration data from my hotplates suggest that a hotplate of that shape and size has a thermal resistance to ambient air of about 2.4 K/W. For reflowing SnPb, you want to aim for a peak temperature of at least 205 celsius. This is lower than most commercial reflow profiles, but from my experience this is more than hot enough to get reliable and repeatable results. For SnBi, typical reflow profiles aim for around 180 celsius peak temperatures. Assuming that we have an ambient air temperature of about 20 celsius, then we're looking to support a temperature delta of about 160 kelvin for SnBi or about 185 kelvin for SnPb. For SnPb, the calculations end up being:

  185 K / 2.4 K/W = ~77 W

That is how much power you need to simply maintain that temperature against ambient air at 20 celsius. To get there in a reasonable amount of time, you will need more. My rule of thumb is to add 20-50% more power. Higher power will allow you to achieve higher temperature gradients and improve you ability to closely follow a reflow profile. 20% additional power should be good enough to achieve reliable and repeatable results but may struggle with some of the steeper temperature gradients during prehead in some reflow profiles. 50% should give you more than enough power to follow most reflow profiles pretty closely. So for a hotplate my size to reflow SnPb solder we're looking at somwhere around 90-115W as a good range of power to aim for in our heating element. If you do the calculations for SnBi, you get slightly lower power range of about 80-100W. Note that you can reflow either of these alloys with less power, but you will need to trade off either hotplate size or heating speed or both in order to do this.

One last comment on heating power requirements... If you're designing a different sized hotplate, you will need to scale these numbers somehow. While I doubt this will scale linearly, in the absence of a better theoretical model, I would suggest starting with linear scaling the power requirement by area. My heating element area is close to 75 square centimetres, so the holding power required is roughly 1W per square centimetre. This makes the target power range to be roughly 1.2 - 1.5 W per square centimetre of heating element. Given how heat transfer works, I expect smaller hotplates will lose heat to the environment more quickly than larger hotplates, so these numbers may estimate low for smaller sizes and might estimate high for larger hotplates. That's just a guess on my part. For smaller hotplates, I'd err closer to the 1.5 W per square centimetre to be safe. For larger hotplates, if you're power constrained, you may be able to work closer to 1.2 W per square centimetre or possibly lower and still get decent results.

## Drive Voltage ##

Let's assume you're designing to run from something like a laptop power brick or USB-PD supply. Laptop power bricks commonly output 19V and are often rated at 90W or more. USB-PD can support voltages up to 20V at currents up to 5A, so 100W. We'll work with the laptop power brick specs, as the lower specs will be more limiting for our hotplate perforance, but I expect these results will still work reasonably well for USB-PD and give us a hotplate that can be supported by a wide range of available hardware that's already out there in the world. We know that:

  P = V^2 / R

So if we square the voltage and divide by the maximum output power, we get a resistance for the hotplate to achieve that power when driven by that voltage. So:

  19 x 19 / 90 = ~4 ohms

## Resistance Versus Temperature ##

We need to allow for the fact that our heating element's resistance will increase as it gets hotter. In fact, this is one good option you can use to determine the temperature of the hotplate if your control scheme has access to accurate real-time voltage and current information from the hotplate. It's impractical to try to be overly precise as there can be substantial variation between boards in a batch as well as process variation between batches and between manufacturers. Because of this, it's probably not worth more than a rough approximation here. At reflow temperatures, your heating element's resistance will be roughly 70% higher than it will be at ambient temperatures. I typically take the resistance value calculated above as the resistance of the heating element at peak temperature (ie. when "hot"), so to determine the target resistance for my heating element at ambient temperatures, I take the number from above and divide it by 1.7.

  4.01 ohms / 1.7 = ~2.4 ohms

That's the value I design for in my heating element.

## PCB Design To Resistance ##

Some PCB design software may provide tools for estimating the resistance of PCB traces. In the absence of that, the following approach may be close enough, although from my results, I suspect that what happens around corners or at changes in trace width may be more complex than this model allows, so if your design includes very large numbers of corners or variations in width, the real world results may be different from what you get from this calculation.

To calculate the resistance of a linear trace of constant width, you would use the formula:

  R = rho x length / width / thickness

where rho is the resistivity of the material in ohm-m and length, width and thickness are all distances measured in *metres*. For 1 oz copper, the theoretical thickness of the copper foil on a PCB would be about 35 x 10^-6m thick (ie. 0000035m thick). For annealed copper, rho is about 1.72 x 10^-8 ohm-m. So for a copper trace 1.5mm wide, 70mm long in 1 oz copper, we would have R = 1.72 x 10^-8 x 0.07 / 0.0015 / 0.000035 = 0.02293 or about 23 milliohms. From working with JLCPCB for some of our earlier revisions, the actual resistances seemed to suggest that the copper thickness may be closer to 29 microns (ie. 0.000029m) than the 35 microns we expect. Although I now recommend PCBWay as a better option for manufacturing hotplates, I have less experience with them so far. I think 29 microns may also be a reasonable approximation of copper thickness there, but I have had some surprising results with some of my later, more complex layouts, so maybe my expectations are wrong or some process has changed at PCBWay in recent months. Your mileage may vary. Based on this, we can proceed as follows:

1. Break your heating element up into a series of linear traces with constant width.
1. For each linear trace, measure the length of the trace along the centre of the trace. The ends of each trace should touch the ends of neighbouring traces, so at a corner the end would be in the mid-point of the corner for both traces entering or leaving that corner.
1. Sum the lengths of all the traces with the same width.
1. Calculate the total resistance for each trace width you have using the formula as shown above.
1. Sum all of the resistances calculated for each different trace width.

That last number is the estimated resistance of the entire heating element. It can take some trial and error to dial in the resistance just where you want it, so be patient. Alternatively, use someone else's well tested design if it's close enough to your design requirements.

Hopefully this gives enough of a background that someone interested in the challenge of designing their own hotplate can get a start without having to reinvent everything. Good Luck!
