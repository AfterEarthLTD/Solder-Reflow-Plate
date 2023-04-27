# **Software Revisions**
###### 70*50mm Solder Reflow plate

## Note
This software is barely functional. It was not written by me nor would I consider it actively maintained by anybody. The original developer appears to have abandoned the project.

I am working on new designs to address many of the hardware and software design issues. In the meantime, this design can work with some limitations. Specifically:

  - It's a little slow.
  - It's not well suited for anything other than low temperature solder alloys (ie. SnBi alloys that melt around 140 celsius).
  - The temperature measurements that it gives are wildly inaccurate.
  - It won't follow a solder profile. It basically just heats up until it hits something it thinks is the requested maximum temperature and then shuts off power to the hotplate.

## User Interface

Use the buttons to adjust maximum temperature up and down in 10 degree increments.

Press both buttons together to start the reflow cycle.

## Operational Explanation

The logic used to drive the heating algorithm is fundamentally flawed if the goal is to follow some kind of reflow profile.

  - It doesn't consider time while the heating cycle is running, so how can it know what temperature is should be targeting at any given point in the reflow cycle.
  - It does vary the PWM duty cycle during the heating cycle, but from the logic all it really does it:
    - Quickly ramp up the PWM duty cycle to a "warm-up" value.
    - Once the reported temperature reaches a warm-up value, quickly ramps up the PWM duty cycle to a maximum duty cycle value.
    - Once the reported temperature reaches the maximum temperature requested, shuts off power to the heating element.
    - The reported temperature continues to be monitored until the reported temperature falls below 45 celsius.

## Potential Improvement Areas

### Follow an actual reflow profile.

There are designs out there that already do this, but I've not seen any firmware shared to do this. This would require a control method that understands how much time has passed from the beginning of the reflow cycle, can calculate the expected temperature at that time and produce a control signal to closely follow that expected temperature. PID control is probably the most appropriate method to do this, although there are other possible approaches.

### PID Control with Autotuning

PID control is relatively simple to implement, but how well it works is highly dependent on the proportional, integral and differential gains used. An auto-tuning cycle using some common auto-tuning algorithm like Ziegler-Nichols would be desirable so that users can relatively easily arrive at a reaonable working set of gains for their hotplate.

### Reflow profile customization

Once you have PID control and auto-tuning, you can follow a reflow profile provided that the device has sufficient power to keep up. The reflow profiles should be able to be customized. The simplest way would be to provide a relatively straightforward means of defining the reflow profile in the code and simply let users upload new firmware to set up a new profile. Whether it's possible to provide customization of reflow profiles via the two-button UI is dubious. While it could definitely be implemented, whether this can be done in a way that's usable enough to justify the effort is questionable.

A more likely option may be to define several different hard-coded profiles in the firmware and provide a simple UI to select on of several reflow profiles instead of the current UI which allows selection of one of several maximum temperature settings.

