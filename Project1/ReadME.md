Your objective for this project is to implement a randomized counter in Assembly. A 4-digit SSD should be connected that will display your ID (last 4 digits) when your code is not counting (idle state). When an external button is pressed, it will generate a 4-digit random number, and start counting down to 0. The generated random number should be between 1000 - 9999. When the counter reaches 0, the number 0000 should be displayed for a second, then the code should go back to idle state waiting for the next button press. Pressing the button while counting down should pause counting, and pressing again should resume counting.

Technical requirements:

● Written in Assembly.

● An external button should be attached that will generate a random number when pressed

○ Generated random number should be between 1000 - 9999.

● An external LED that will be turned off when the counting is in progress, and will be turned
on as soon as the number reaches to 0000

● A 4-digit seven segment display that will show the 4-digit number

○ the first digit should display the seconds, the last three digits should display the
milliseconds, so it will count down from 9 at most.

● When the system is idle, your school ID will be shown (last 4-digits)

○ Number 0000 should be displayed at least 1 second before going to ID mode

● Pressing the button should pause the counting and pressing again should resume counting.

Quality requirements:

● No debouncing on the button

● No considerable delay with button presses

● No flickering on the displays

● Should be as accurate as possible with the seconds

● Code should be properly commented with your name / school ID added in the beginning
