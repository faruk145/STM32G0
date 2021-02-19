/*
 *
 * Author:
 *		Omer Faruk Aydin 171024002
 *
 */

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOA_BASE,       (0x50000000)          // GPIOA base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOA ODR register offset

.equ GPIOB_BASE,       (0x50000400)          // GPIOB base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_IDR,        (GPIOB_BASE + (0x10)) // GPIOB IDR register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOB ODR register offset



/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss
	 * not necessary for rom only code
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
/******************* SETUP START *******************/
	push {lr}
	/* enable GPIOB and GPIOA clock,IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	/* movs expects imm8, so this should be fine */
	movs r4, 0x3
	orrs r5, r5, r4
	str r5, [r6]

	/* setup (on board)A1234567 for 7 led (SSD) in MODER */
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0x3C0FF0C
	bics r5, r5, r4
	ldr r4, =0x1405504
	orrs r5, r5, r4
	str r5, [r6]

	/* setup (on board)D8 D10 D11 D12 for set digits (SSD) and D7 for button in MODER */
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0XFFFFFFFF
	orrs r5, r5, r4
	ldr r4, =0XFFF5F5CF
	ands r5, r5, r4
	str r5, [r6]
	ldr r2, =#1

	ldr r6, = GPIOB_MODER
	ldr r5, [r6]
    ldr r4, =0x2
    bics r5, r5, r4
    str r5, [r6]
/******************* SETUP END *******************/

loop:
	/* loop */
	b Idle
	b loop

	pop {pc} //return 0

Idle: // show the last four numbers (and also check button for random number)
	adds r2, r2, #1 // number for randomize every iteration plus r2
	ldr r0, =#4
	bl Light_the_1st_Digit
	ldr r1, =#500
	bl delay
	ldr r0, =#0
	bl Light_the_2nd_Digit
	ldr r1, =#500
	bl delay
	ldr r0, =#0
	bl Light_the_3rd_Digit
	ldr r1, =#500
	bl delay
	ldr r0, =#2
	bl Light_the_4th_Digit
	ldr r1, =#500
	bl delay

	ldr r6, =GPIOB_IDR // to check button if it has been pressed
	ldr r5, [r6]
	ldr r4, =0x104 //0000_0100 for PB2
	cmp r5, r4
	beq randomGenerateBuffer

	b loop


Light_the_1st_Digit:
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r3, =0XFFFFFFFF
	ldr r4, =0X010
	orrs r5, r5, r3
	ands r5, r5, r4
	str r5, [r6]
	cmp r0, #0
	beq Number_0Buffer
	cmp r0, #1
	beq Number_1Buffer
	cmp r0, #2
	beq Number_2Buffer
	cmp r0, #3
	beq Number_3Buffer
	cmp r0, #4
	beq Number_4Buffer
	cmp r0, #5
	beq Number_5Buffer
	cmp r0, #6
	beq Number_6Buffer
	cmp r0, #7
	beq Number_7Buffer
	cmp r0, #8
	beq Number_8Buffer
	cmp r0, #9
	beq Number_9Buffer

Light_the_2nd_Digit:
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r3, =0XFFFFFFFF
	ldr r4, =0X020
	orrs r5, r5, r3
	ands r5, r5, r4
	str r5, [r6]
	cmp r0, #0
	beq Number_0Buffer
	cmp r0, #1
	beq Number_1Buffer
	cmp r0, #2
	beq Number_2Buffer
	cmp r0, #3
	beq Number_3Buffer
	cmp r0, #4
	beq Number_4Buffer
	cmp r0, #5
	beq Number_5Buffer
	cmp r0, #6
	beq Number_6Buffer
	cmp r0, #7
	beq Number_7Buffer
	cmp r0, #8
	beq Number_8Buffer
	cmp r0, #9
	beq Number_9Buffer

Light_the_3rd_Digit:
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r3, =0XFFFFFFFF
	ldr r4, =0X200
	orrs r5, r5, r3
	ands r5, r5, r4
	str r5, [r6]
	cmp r0, #0
	beq Number_0Buffer
	cmp r0, #1
	beq Number_1Buffer
	cmp r0, #2
	beq Number_2Buffer
	cmp r0, #3
	beq Number_3Buffer
	cmp r0, #4
	beq Number_4Buffer
	cmp r0, #5
	beq Number_5Buffer
	cmp r0, #6
	beq Number_6Buffer
	cmp r0, #7
	beq Number_7Buffer
	cmp r0, #8
	beq Number_8Buffer
	cmp r0, #9
	beq Number_9Buffer

Light_the_4th_Digit:
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r3, =0XFFFFFFFF
	ldr r4, =0X100
	orrs r5, r5, r3
	ands r5, r5, r4
	str r5, [r6]
	cmp r0, #0
	beq Number_0Buffer
	cmp r0, #1
	beq Number_1Buffer
	cmp r0, #2
	beq Number_2Buffer
	cmp r0, #3
	beq Number_3Buffer
	cmp r0, #4
	beq Number_4Buffer
	cmp r0, #5
	beq Number_5Buffer
	cmp r0, #6
	beq Number_6Buffer
	cmp r0, #7
	beq Number_7Buffer
	cmp r0, #8
	beq Number_8Buffer
	cmp r0, #9
	beq Number_9Buffer

Number_0Buffer:
	b Number_0

Number_1Buffer:
	b Number_1

Number_2Buffer:
	b Number_2

Number_3Buffer:
	b Number_3

Number_4Buffer:
	b Number_4

Number_5Buffer:
	b Number_5

Number_6Buffer:
	b Number_6

Number_7Buffer:
	b Number_7

Number_8Buffer:
	b Number_8

Number_9Buffer:
	b Number_9

randomGenerateBuffer:
	b randomGenerate

loopBuffer1:
	b loop

Number_0:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X1872
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_1:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X30
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_2:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X1892
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_3:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X10B2
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_4:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0XF0
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_5:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X10E2
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_6:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X18E2
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_7:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X32
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_8:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X18F2
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

Number_9:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r3, =0X0
	ldr r4, =0X10F2
	ands r5, r5, r3
	orrs r5, r5, r4
	mvns r5, r5
	str r5, [r6]
	ldr r1, =#100
	b delay

loopBuffer2:
	b loopBuffer1

randomGenerate:
	ldr r6, =GPIOB_IDR // to check button if it has been pressed
	ldr r5, [r6]
	ldr r4, =0x0
	ands r5, r5, r4

	ldr r5, =0XFFFFF
	ands r2, r2, r5
	ldr r6, =#10000           //Determined the upper boundary of random number
	cmp r2,r6
	ble count
randomGenerateLoop:
	subs r2,r2,r6
	cmp r2, r6
	ble count				//Do not count down until having a random number less than 10.000
	b randomGenerateLoop

count:
	movs r7, r2			//r2 was the random number

countDownLoop:
	subs r7, r7, #1				//After decrementation cleared the registers that will be used.Just in case.
	ldr r0, =0x0
	ldr r1, =0x0
	ldr r5, =0x0
	ldr r6, =0x0
FirstDigit:
	ldr r0, =0x0
	movs r2, r7
	ldr r3, =#1000				//Decrementing 1000 every time and keeping r0 as a counter.
	cmp r2, r3
	ble FirstDigitLed

FirstDigitLoop:
	subs r2, r2, r3
	adds r0, #1
	cmp r2, r3
	bgt FirstDigitLoop

FirstDigitLed:
	bl Light_the_1st_Digit
	ldr r1, =#3000
	bl delay

SecondDigit:
	ldr r1, =0x0
	ldr r3, =#100				//Decrementing 100 every time and keeping r0 as a counter.
	cmp r2, r3
	ble SecondDigitLed

SecondDigitLoop:
	subs r2, r2, r3
	adds r1, #1
	cmp r2, r3
	bgt SecondDigitLoop

SecondDigitLed:
	movs r0, r1
	bl Light_the_2nd_Digit
	ldr r1, =#1000
	bl delay

ThirdDigit:
	ldr r5, =0x0
	ldr r3, =#10				//Decrementing 10 every time and keeping r0 as a counter.
	cmp r2, r3
	ble ThirdDigitLed

ThirdDigitLoop:
	subs r2, r2, r3
	adds r5, #1
	cmp r2, r3
	bgt ThirdDigitLoop

ThirdDigitLed:
	movs r0, r5
	bl Light_the_3rd_Digit
	ldr r1, =#1000
	bl delay

FourthDigit:
	ldr r6, =0x0
	movs r6, r2
	movs r0, r6
	bl Light_the_4th_Digit
	ldr r1, =#1000
	bl delay

	ldr r1, =#9000
	cmp r7, #0
	beq zeroWait
	b isButtonPressed
	b countDownLoop				//Go back to decrementing random number again.

countDownLoopBuffer:
	b countDownLoop
delay:
	subs r1, r1, #1
	bne delay
	bx lr

zeroWait:
	push {r1}
    ldr r6, = GPIOB_ODR			//When the count is over display zero for a while and turn on the external LED.
    ldr r5, [r6]
    ldr r4, =0x1
    orrs r5, r5, r4
    str r5, [r6]
    bl Light_the_1st_Digit
    bl Light_the_2nd_Digit
    bl Light_the_3rd_Digit
    bl Light_the_4th_Digit
    pop {r1}
	subs r1, r1, #1
	bne zeroWait
	b loopBuffer2

showThatMomentNumber:
	b showScreen

resume:
	b buttonForResume

delayForButton:
	subs r1, r1, #1
	bne delayForButton
	b showThatMomentNumber

delayForButton2:
	subs r1, r1, #1
	bne delayForButton2
	b countDownLoop

isButtonPressed:
	ldr r1, = #500000
	ldr r6, = GPIOB_IDR
	ldr r5, [r6]
	ldr r4, = 0x104
	cmp r5, r4
	beq delayForButton
	b countDownLoop

buttonForResume:
	ldr r1, =#500000
	ldr r6, = GPIOB_IDR
	ldr r5, [r6]
	ldr r4, =0x4
	ands r5, r5, r4
	cmp r5, r4
	beq delayForButton2
	b showThatMomentNumber

showScreen:
FirstDigit2:
	ldr r0, = 0x0
	movs r2, r7
	ldr r3, =#1000
	cmp r2, r3
	ble FirstDigitLed2

FirstDigitLoop2:
	subs r2, r2, r3
	adds r0, #1
	cmp r2, r3
	bgt FirstDigitLoop2

FirstDigitLed2:
	bl Light_the_1st_Digit
	ldr r1, =#3000
	bl delay

SecondDigit2:
	ldr r1, =0x0
	ldr r3, =#100				//Decrementing 100 every time and keeping r0 as a counter.
	cmp r2, r3
	ble SecondDigitLed2

SecondDigitLoop2:
	subs r2, r2, r3
	adds r1, #1
	cmp r2, r3
	bgt SecondDigitLoop2

SecondDigitLed2:
	movs r0, r1
	bl Light_the_2nd_Digit
	ldr r1, =#1000
	bl delay

ThirdDigit2:
	ldr r5, =0x0
	ldr r3, =#10				//Decrementing 10 every time and keeping r0 as a counter.
	cmp r2, r3
	ble ThirdDigitLed2

ThirdDigitLoop2:
	subs r2, r2, r3
	adds r5, #1
	cmp r2, r3
	bgt ThirdDigitLoop2

ThirdDigitLed2:
	movs r0, r5
	bl Light_the_3rd_Digit
	ldr r1, =#1000
	bl delay
	ldr r6, =0x0
	movs r6, r2
	movs r0, r6
	bl Light_the_4th_Digit
	ldr r1, =#1000
	bl delay
	b resume
