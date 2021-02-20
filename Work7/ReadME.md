Problem1:In this problem, you will implement a light dimmer with a potentiometer.

-Connect a pot using a resistor divider setting.

-Connect two external LEDs. These LEDs will light up in opposite configuration.

-By changing the pot you will change the brightness of these LEDs. For example if the pot is all they way down,
first LED should light up, and second LED should be o, and if the pot is all the way up, first LED should be o
and the second LED should light up. Their brightness should change in between.

-You will need PWM for the LED driving to change the brightness. 0 duty cycle will turn o the LEDs and 100%
duty cycle will light them up completely

Problem2:In this problem, you will work on implementing a knock counter.

-Connect SSD that will show the number of knocks.

-Connect an external button that will reset the counter.

-Connect a microphone that will pick up the sounds.

-When you knock on the table, you should increment the counter by one.

-There should be no mis-increments, or multiple increments as much as possible.

-You may include an IMU sensor readings to improve your accuracy, but still need to incorporate ADC samples.
