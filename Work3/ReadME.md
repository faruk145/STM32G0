Problem1:

• Create a HardFault Handler that will detect possible faults. The handler should properly restore stack pointer, and call reset handler afterwards. After compiling, examine the routine to make sure there are no overheads. 

• Attach an external button to your board, and using polling method execute these hardfaults randomly. For this you can use rand( ) C library function. 

• Optionally you can implement a simple toggle LED mechanism in the beginning of your code to see the reset operation. 

Problem2:

• Setup and implement an external interrupt routine with highest possible priority, and tie it to your external button. Upon pressing, your interrupt routine should execute the previously implemented hardfaults randomly. 

• When interrupt happens, check out the context and explain each section. 

• Your main while loop should be empty for this problem since everything will be taken care of in the interrupt handler routine. 

External interrupts are used to detect edge transitions on external signal. Both rising and falling edge transitions can be configured and detected if the hardware supports it. For Cortex-Mo+, external interrupts are connected to an external interrupt MUX, and each pin can be configured with the desired edge trigger mechanism. 

Problem3:

• Connect two buttons and two LEDs to your board. Enable external interrupt for both of them. Each button should toggle one LED. o Connect one button in one of the following groups: 10,11, {2,3}, {4-15} and the other in the remainin group. 

• You will use two external interrupt handlers. Make sure to check using the pending bit if the correct interrupt is received. 

• Assign different priorities to your buttons, and inside the handlers create a long delay that will last for at least 5-10 seconds)

o You can implement this behavior by turning on the associated LED in the beginning, then an empty for loop, then turning off the associated LED. 
