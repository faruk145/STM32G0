Problem1:In this problem, you will connect your board to the PC using UART protocol. For this you will need to create an initialization routine for UART, and create send and receive functions.

Problem2:In this problem, you will implement a PWM signal and drive an external LED using varying duty cycles. Your LED should display a triangular pattern, meaning the brightness should linearly increase and decrease in 1 second.

Problem3:In this problem, you will implement a PWM signal and drive an LED at dierent speeds. You will use keypad to set the duty cycle. Pressing 10# will set the duty cycle to 10% and 90# will set the duty cycle to 90%. Any unusal presses should be ignored. (i.e. 9F#)

-The current duty cycle should print every 2 seconds using UART

-The new duty cycle should also print after a button presses.

-Your buttons should not bounce.

Appendix A-Setting up USART and creating a print() function

Pins PA2 and PA3 are connected to the ST/Link Debugge IC on the back which gets passed to the PC over USB as a
Virtual COM port. This can be seen from the schematic of the board. First, we need to determine the USART modules
PA2 and PA3 are connected to. From stm32g031k8 datasheet, we can see that we have two USART modules named
USART1 and USART2.

Looking at Table 13 - Port A alternate function mapping, we can also see that PA2 and PA3 are connected to USART2
module using Alternate Function 1 mode (AF1). Now that we have the relevant info, we can start initializing steps.
Check the RCC, GPIO and USART sections from rm0444 reference manual for exact registers and relevant bits.

1. enable GPIOA and USART2 module clocks from RCC 
2. enable and setup pins PA2 and PA3 as alternate function for USART2 module
3. enable receive and transmit from USART2 module
4. set baud rate to 9600 bps assuming clock is running at 16Mhz
5. enable uart module
At this point, if we open a serial port from PC, and send a character from MCU, we should be able to see. Let’s write
some helpers for this purpose.
6. implement a printChar function given as below. Now you can just call printChar('a'); from your main
loop and this should be displayed on your PC terminal
7. implement a _print function that will call the printChar function for the given string in a for loop given as
below. Now you can call _print(0, "Hello\n", 6); and it should be displayed on your PC terminal
