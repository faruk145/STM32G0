Main objective of this project/final is to create a digital voice recorder that can record your voice, playback a selected voice recording, and delete single or all recording data.
During this project, you will use various modules such as Timer, PWM, ADC, and External Interrupts.

Technical requirements:

-Written in C. No HAL or equivalent libraries. (Using CMSIS, and stm32xxx.h kind of headers is fine.)

-Connect a microphone to record your voice. Keep in mind that if this microphone does not have an on-board amplifier, you will need to build one yourself.

-Build an amplifier and connect a speaker with variable pot to playback the recording.

-Connect a 24LC512 EEPROM on the same I2C bus. Keep in mindm when wiring the bus will require pull-up resistors on both lines, and each of these devices need dierent address to
communicate.

-You should be able to record 1 track with 5 seconds. 5 seconds should be fixed, but if you can fit more tracks that is fine.


-A keypad should be attached to operate the device.

-Assign a key for recording a voice. The recording will go for 5 seconds and automatically stop/save it. After the track is played, it will stop and go back to IDLE state.

-Pressing any other button should not have any effect.

-Assign a key for playing/pausing the selected track when not recording. After the track is played, it will stop and go back to IDLE state.

-Pressing any other button should not have any effect.

-Assign a key for deleting the selected track. After the track is deleted, it will go back to IDLE state.

-A 7SD should be attached to display the operations and status.

-You should have multiple states, some of which include:

-START state which only happens when the board powers up 7SD should show your ID (first 2 and last 2 digits)

-IDLE state which displays IdLE on the 7SD and does not do anything else. (waiting for track select or record start)

-FULL state which displays FuLL on the 7SD and prevents going into RECORD state.

-RECORD state where the 7SD shows rcd and a count down from 5 seconds indicating the recording. (i.e. rcd3, rcd2)

-PLAYBACK state where the 7SD shows PLb and the track being played back (i.e. PLb2, PLb1)

-If no button is pressed for 10 seconds, the device should go back to IDLE state.

Quality requirements:

-No bouncing on the buttons

-No considerable delay with button presses

-No flickering on the displays AND no brightness difference on the 7SDs

-Recording should be undersdandable

-Code should be properly commented with your name / school ID added in the beginning

