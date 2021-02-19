Problem1: In this problem, you will work on creating an accurate delay function using SysTick exception. Create a SysTick exception with 1 millisecond interrupt intervals. Then create a delay_ms(..) function that will accurately wait for (blocking) given number of milliseconds.

Problem2:In this problem, you will work with general purpose timers. Set up a timer with lowest priority that will be used to toggle on-board LED at 1 second intervals. Change the blinking speed using an external button. Each button press should increase the blinking speed by 1 second up to a maximum of 10 seconds. Next button press after 10 should revert it back to 1 second. All the functionality should be inside your interrupts.

Problem3:In this problem, connect your seven segment display and implement a count up timer. It should sit at zero, once the external button is pressed, it should count up to the max value (i.e. 9999) then once it overflows to 0, it should stop, and light up an LED (on-board or external). Pressing the button again should clear the LED and count again. For this problem, your main loop should not have anything. All the functionality should be inside your interrupts.

Note: Arrange it so that the LSD of the number increments in 1 second intervals. (i.e it should take 10 seconds to go
from 0000 to 9999)

Problem4:In this problem, you will work with watchdog timers.Calculate the appropriate reset time and implement it. Add the necessary handler routine for reseting the device.

Problem5: In this problem, you will implement your watchdog timer in Problem 3. Figure out a way to properly incorporate it to your code when it all works with timers.


