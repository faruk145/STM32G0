Main objective of this project is to create a fully operational scientific calculator in C. This calculator will have a keypad connected to enter the numbers and execute basic 
scientific and trigonometric functions. A 4-digit seven segment display should be used to display these numbers.

Technical requirements:

• Written in C. No HAL or equivalent libraries.

• A keypad and a seven-segment display should be attached.

• On power up SSD should show your ID (first 2 and last 2 digits). o As soon as a number is pressed, everything should be cleared and only your number should be displayed. o If no button is pressed for 10 seconds, the SSD should turn off - go back to the IDLE state.

• When keys are entered, the SSD should shift the numbers to the left, while not displaying anything for empty digits.

• If the digits are already full, new number key presses should be ignored.

• ABCDEF keys should be used as:

o A is for addition

o B is for subtraction

o C is for multiplication

o D is for division

o E key is scientific mode, and will expect another keypress.

■ EA is for log 

• EB is for ln

■ EC is for sqrt 

■ ED is for x^2 

• EE is for trigonometric mode, and will expect another keypress.

■ EEA is for sin 

■ EEB is for cos 

• EEC is for tan 

■ EED is for cot 

• EEE is for pi (will replace the number with 3.141) 

o F key is for enter/equal 

• Scientific and trigonometric modes will require floating point number system.

o Floating point numbers should be displayed with the appropriate dot. For example if you want to show 1.2345152 

- SSD should display 1.234 and if you want to display 4213.123 it should display 4213.
- 
-  • Negative numbers should have a negative sign. i.e -124 on the SSD. 
-  
-  • If the numbers overflows 9999 or -999, it should display overflow (i.e. Ou FL) 
-  
-  • If the operation is invalid (i.e. 3/0 or sqrt( -2 )) it should display invalid (i.e. Invd) 
-  
-  • If no keys are pressed for 10 seconds, the SSD should turn off. - go back to IDLE state.
-  
-   • If directly a function is invoked, the current value should be used. For example, if the last answer is 4 and 
-   
-   - 4 is pressed, it should do 4 - 4 operation and display 0. If in the beginning, the number should be assumed 0. 

Quality requirements:

• No bouncing on the buttons

• No considerable delay with button presses 

• No flickering on the displays 

• Code should be properly commented with your name / school ID added in the beginning 

