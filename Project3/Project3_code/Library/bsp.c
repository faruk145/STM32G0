/*Author: Ömer Faruk Aydın
 * 171024002
 * Project-3*/
#include "bsp.h"

void delay_ms(uint32_t s) {
	for (int i = 0; i < s; ++i);
}

void initTIM2(void) {

	RCC->APBENR1 |= (1U << 0);
	TIM2->DIER |= (1 << 0);
	TIM2->CCER |= (1U << 4);	//For ch-2 Enable Capture mode
	TIM2-> CR1 |= (1U << 7);	//Auto Preload Enable TIM1_ARR register is buffered.
	TIM2-> CCMR1 &= ~(1U << 24);
	TIM2-> CCMR1 |= (1U << 14);
	TIM2-> CCMR1 |= (1U << 13);
	TIM2-> CCMR1 &= ~(1U << 12);	//PWM mode 1 0110
	TIM2-> CCMR1 |= (1U << 11); 	//Output compare enabled

	//PWM freq = Fclk / ((PSC+1) * ARR)
	//PWM Duty = CCRx / ARR

	TIM2->ARR = 255;
	TIM2->CCR2 = 2000;

	TIM2->EGR |= (1U << 0);		//Update Generation

	TIM2-> CR1 |= (1U << 0);	//Counter Enable
	NVIC_SetPriority(TIM2_IRQn, 2);
	NVIC_EnableIRQ(TIM2_IRQn);

}

void initTIM3(void) {
	RCC->APBENR1 |= (1U << 1);

	  TIM3->CR1 = 0;
	  TIM3->CR1 |= (1U << 7);
	  TIM3->CNT = 0;

	  TIM3->CR2 |= (2U << 4);    //MMS REGISTER UPDATE MODE

	  	  //16Mhz/((PSC+1)*ARR)
	  TIM3->PSC = 0;
	  TIM3->ARR = 2500;//2500

	  TIM3->DIER |= (1U << 0);
	  TIM3->CR1 |= (1U << 0);	// ENABLE TIM3

		NVIC_SetPriority(TIM3_IRQn, 1);
		NVIC_EnableIRQ(TIM3_IRQn);

}

void clearRowsKeypad(void){
	GPIOA->ODR &= ~(1U << 15);	//PA15->R1
	GPIOA->ODR &= ~(1U << 1);	//PB1->R2
	GPIOB->ODR &= ~(1U << 10);	//PA10->R3
}

void setAllRows(void){
	GPIOA->ODR |= (1U << 15);	//PA15->R1
	GPIOA->ODR |= (1U << 1);	//PB1->R2
	GPIOB->ODR |= (1U << 10);	//PA10->R3
}

void Init_Keypad(void){
	RCC->IOPENR |= (3U << 0);	//Enable GPIOA and GPIOB ports
	//Setup Membrane 4x4 Keypad
	GPIOA->MODER &= ~(3U << 2*15);	//PA15 for R1 in 4x4 Keypad
	GPIOA->MODER |= (1U << 2*15);

	GPIOB->MODER &= ~(3U << 2*1);	//PB1 for R2 in 4x4 Keypad
	GPIOB->MODER |= (1U << 2*1);

	GPIOA->MODER &= ~(3U << 2*10);	//PA10 for R3 in 4x4 Keypad
	GPIOA->MODER |= (1U << 2*10);

	GPIOA->MODER &= ~(3U << 2*9);	//PA9 for C1 in 4x4 Keypad
	GPIOA->PUPDR |= (2U << 2*9);	//Pull-down mode

	GPIOB->MODER &= ~(3U << 0);		//PB0 for C2 in 4x4 Keypad
	GPIOB->PUPDR |= (2U << 0);		//Pull-down mode

	GPIOB->MODER &= ~(3U << 2*2);	//PB2 for C3 in 4x4 Keypad
	GPIOB->PUPDR |= (2U << 2*2);	//Pull-down mode

	//Setup interrupts for inputs
	EXTI->EXTICR[2] |= (0U << 8*1);	//PA9->C1
	EXTI->EXTICR[0] |= (1U << 0);	//PB0->C2
	EXTI->EXTICR[0] |= (1U << 8*2);	//PB2->C3

	//Rising Edge
	EXTI->RTSR1 |= (1U << 9);	//9th pin
	EXTI->RTSR1 |= (1U << 0);	//0th pin
	EXTI->RTSR1 |= (1U << 2);	//2nd pin

	//Mask
	EXTI->IMR1 |= (1U << 9);	//9th pin
	EXTI->IMR1 |= (1U << 0);	//0th pin
	EXTI->IMR1 |= (1U << 2);	//2nd pin

	//NVIC
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	NVIC_EnableIRQ(EXTI0_1_IRQn);

	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	//Set all rows for 5V
	GPIOA->ODR |= (1U << 15);	//PA15->R1
	GPIOB->ODR |= (1U << 1);	//PB1->R2
	GPIOA->ODR |= (1U << 10);	//PA10->R3
	clear_SSD();
}

void InitSSD(void){
	RCC->IOPENR |= (3U << 0);	//Enable GPIOA and GPIOB ports

	//Setup SSD

	GPIOA->MODER &= ~(3U << 2*1);	//PA1 for A in SSD
	GPIOA->MODER |= (1U << 2*1);

	GPIOA->MODER &= ~(3U << 2*4);	//PA4 for B in SSD
	GPIOA->MODER |= (1U << 2*4);

	GPIOA->MODER &= ~(3U << 2*5);	//PA5 for C in SSD
	GPIOA->MODER |= (1U << 2*5);

	GPIOA->MODER &= ~(3U << 2*12);	//PA12 for D in SSD
	GPIOA->MODER |= (1U << 2*12);

	GPIOA->MODER &= ~(3U << 2*11);	//PA11 for E in SSD
	GPIOA->MODER |= (1U << 2*11);

	GPIOA->MODER &= ~(3U << 2*6);	//PA6 for F in SSD
	GPIOA->MODER |= (1U << 2*6);

	GPIOA->MODER &= ~(3U << 2*7);	//PA7 for G in SSD
	GPIOA->MODER |= (1U << 2*7);

	GPIOB->MODER &= ~(3U << 2*4);	//PB4 for D1 in SSD
	GPIOB->MODER |= (1U << 2*4);

	GPIOB->MODER &= ~(3U << 2*5);	//PB5 for D2 in SSD
	GPIOB->MODER |= (1U << 2*5);

	GPIOB->MODER &= ~(3U << 2*9);	//PB9 for D3 in SSD
	GPIOB->MODER |= (1U << 2*9);

	GPIOB->MODER &= ~(3U << 8);		//PB8 for D4 in SSD
	GPIOB->MODER |= (1U << 8);
}


void setSSD(int x){
	clear_SSD();
	switch(x){
	case 0:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 11);	//PA11	E
		GPIOA->ODR &= ~(1U << 6);	//PA6	F
		break;
	case 1:
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		break;
	case 2:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 11);	//PA11	E
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		break;
	case 3:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		break;
	case 4:
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		GPIOA->ODR &= ~(1U << 6);	//PA6	F
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		break;
	case 5:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		GPIOA->ODR &= ~(1U << 6);	//PA6	F
		break;
	case 6:
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 11);	//PA11	E
		GPIOA->ODR &= ~(1U << 6);	//PA6	F
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		break;
	case 7:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		break;
	case 8:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 11);	//PA11	E
		GPIOA->ODR &= ~(1U << 6);	//PA6	F
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		break;
	case 9:
		GPIOA->ODR &= ~(1U << 1);	//PA1	A
		GPIOA->ODR &= ~(1U << 4);	//PA4	B
		GPIOA->ODR &= ~(1U << 5);	//PA5	C
		GPIOA->ODR &= ~(1U << 12);	//PA12	D
		GPIOA->ODR &= ~(1U << 6);	//PA6	F
		GPIOA->ODR &= ~(1U << 7);	//PA7	G
		break;
	}

}

void clear_SSD(void){
	GPIOA->ODR |= (1U << 1);	//PA1	A
	GPIOA->ODR |= (1U << 4);	//PA4	B
	GPIOA->ODR |= (1U << 5);	//PA5	C
	GPIOA->ODR |= (1U << 12);	//PA12	D
	GPIOA->ODR |= (1U << 11);	//PA11	E
	GPIOA->ODR |= (1U << 6);	//PA6	F
	GPIOA->ODR |= (1U << 7);	//PA7	G
}

void clear_Digit(int x){
	switch(x){
		case 1:
			GPIOB->ODR &= ~(1U << 4);	//PB4 for D1 in SSD
			break;
		case 2:
			GPIOB->ODR &= ~(1U << 5);	//PB5 for D2 in SSD
			break;
		case 3:
			GPIOB->ODR &= ~(1U << 9);	//PB9 for D3 in SSD
			break;
		case 4:
			GPIOB->ODR &= ~(1U << 8);	//PB8 for D4 in SSD
			break;
	}
}

void TurnOn_Digit(int x){
	switch(x){
		case 1:
			GPIOB->ODR |= (1U << 4);	//PB4 for D1 in SSD
			break;
		case 2:
			GPIOB->ODR |= (1U << 5);	//PB5 for D2 in SSD
			break;
		case 3:
			GPIOB->ODR |= (1U << 9);	//PB9 for D3 in SSD
			break;
		case 4:
			GPIOB->ODR |= (1U << 8);	//PB8 for D4 in SSD
			break;
	}
}

int ndigit(int num){		//Find the number of digits.
	int count = 0;

	if (num == 0)
		return 1;

	while (num != 0) {
		++count;
		num /= 10;
	}

	return count;
}

void Display_Number(int x){		//Display function for integer numbers.
	clear_SSD();
	if (ndigit(x) == 1){
		setSSD(x % 10);
		TurnOn_Digit(4);
		clear_Digit(4);
	}
	else if (ndigit(x) == 2){
		setSSD(x / 10);
		TurnOn_Digit(3);
		clear_Digit(3);
		setSSD(x % 10);
		TurnOn_Digit(4);
		clear_Digit(4);
	}
	else if (ndigit(x) == 3){
		setSSD(x / 100);
		TurnOn_Digit(2);
		clear_Digit(2);
		setSSD((x / 10) % 10);
		TurnOn_Digit(3);
		clear_Digit(3);
		setSSD(x % 10);
		TurnOn_Digit(4);
		clear_Digit(4);
	}
	else if (ndigit(x) == 4){
		setSSD(x / 1000);
		TurnOn_Digit(1);
		clear_Digit(1);
		setSSD((x / 100) % 10);
		TurnOn_Digit(2);
		clear_Digit(2);
		setSSD((x / 10) % 10);
		TurnOn_Digit(3);
		clear_Digit(3);
		setSSD(x % 10);
		TurnOn_Digit(4);
		clear_Digit(4);
	}
}

void initPWMPin(void) {

	RCC->IOPENR |= (1U << 1);			/* B BLOCK RCC OPEN */
	GPIOB->MODER &= ~(3U << 2 *3);		/* PB3 ALTERNA MODE CLEAR */
	GPIOB->MODER |= (2U << 2 *3);	    /* PB3 ALTERNA MODE */
	GPIOB->OSPEEDR |= (3U << 2 *3);	    /* HIGH PRECISION ON CHANGE */
	GPIOB->AFR[0] &= ~(0xFU << 4 *3);   /* AFSEL3 = RESET and PB3->AF1 = TIM1_CH2 */
	GPIOB->AFR[0] |= (2U << 4 *3);	    /* AFSEL3 -> AF2 [0010] */
}


void initADC(void){
	RCC->IOPENR |= (1U << 0);			// A BLOCK RCC OPEN
	GPIOA->MODER &= ~(3U << 0);		// PA0 ANALOG MODE CLEAR
	GPIOA->MODER |= (3U << 0);		// PA0 ANALOG MODE

	RCC->APBENR2 |= (1U << 20);			// ENABLE ADC CLOCK
	ADC1->CR |= (1U << 28);	 			// ENABLE ADC VOLTAGE REGULATOR
	delay_ms(100);

	ADC1->CR |= (1U << 31);				// ENABLE ADC CALIBRATION
	while ((ADC1->ISR & (1 << 11)));		// WILL NOT COME OUT WITHOUT ADC CALIBRATION

	//Enable end of conversion interrupt
	ADC1->IER |= (1U << 2);		//end of conversion

	//Enable ADC for the first time and set it to single mode
	ADC1->CFGR1 &= ~(1U << 13);
	ADC1->CFGR1 |= (1U << 16);

	ADC1->CFGR1 |= (1U << 23);	//AWD	enable
	ADC1->CFGR1 |= (0U << 26);	//AWD

	ADC1->CFGR1 |= (2U << 3);	//8 bit res

	//EXTSEL TRG3 for TIM3
	ADC1->CFGR1 |= (3U << 6);

	//EXTEN
	ADC1->CFGR1 |= (1U << 10);

	//Set the sampling rate
	ADC1->SMPR |= (1U << 8);	//Ch-1
	ADC1->SMPR |= (0x4 << 4);	//19.5 clk cycle -> 12.5 + 19.5 = 32 cycles

	ADC1->CHSELR |= (1U << 0);	//Enable Channels
	ADC1->CR |= (1U << 0);				// ENABLE ADC
	while (!(ADC1->ISR & (1 << 0)));		// WILL NOT COME OUT UNTIL ADC READY
	NVIC_SetPriority(ADC1_IRQn,2);
	NVIC_EnableIRQ(ADC1_IRQn);
}

void start_conversion(void){
	ADC1->CR |= (1U << 2);	//Start Conversion
	while (!(ADC1->ISR & (1 << 2)));
}

void I2C_Init(void){
		RCC->IOPENR |= (1U << 1);	//Enable B port
		//Setup PB6 as AF6
		GPIOB->MODER &= ~(3U << 2*6);
		GPIOB->MODER |=(2U << 2*6);
		GPIOB->OTYPER |=(1U << 6);	//Open-drain for floating Z
		GPIOB->PUPDR |= (1U << 2*6);	//Pull-up

		GPIOB->AFR[0] &= ~(0xFU << 4*6);
		GPIOB->AFR[0] |= (0x6U << 4*6);
		//Setup PB7 as AF6
		GPIOB->MODER &= ~(3U << 2*7);
		GPIOB->MODER |=(2U << 2*7);
		GPIOB->OTYPER |=(1U << 7);	//Open-drain for floating Z
		GPIOB->PUPDR |= (1U << 2*7);	//Pull-up

		GPIOB->AFR[0] &= ~(0xFU << 4*7);
		GPIOB->AFR[0] |= (0x6U << 4*7);
		//Enable I2C1
		RCC->APBENR1 |= (1U << 21);
		I2C1->CR1 = 0;
		I2C1->CR1 |= (1U << 7);	//Error detection
		I2C1->CR2 |= (1U << 15);	//NACK
		I2C1->CR2 |= (1U << 25);	//Autoend

		I2C1->TIMINGR |= (3U << 2*8);	//Prescaler
		I2C1->TIMINGR |= (0x13U << 0);	//SCLL
		I2C1->TIMINGR |= (0xFU << 8);	//SCLH
		I2C1->TIMINGR |= (0x2U << 1*6);	//SDADEL
		I2C1->TIMINGR |= (0x4U << 2*8);	//SCLDEL

		I2C1->CR1 = (1U << 0);	//PE

		NVIC_SetPriority(I2C1_IRQn, 1);
		NVIC_EnableIRQ(I2C1_IRQn);
}
void display_Record(void){
	clear_SSD();

	GPIOA->ODR &= ~(1U << 11);
	GPIOA->ODR &= ~(1U << 6);
	GPIOA->ODR &= ~(1U << 1);	//The letter r

	TurnOn_Digit(1);
	clear_Digit(1);

	GPIOA->ODR &= ~(1U << 11);
	GPIOA->ODR &= ~(1U << 6);
	GPIOA->ODR &= ~(1U << 1);
	GPIOA->ODR &= ~(1U << 12);	//The letter c

	TurnOn_Digit(2);
	clear_Digit(2);

	setSSD(1);
	GPIOA->ODR &= ~(1U << 12);
	GPIOA->ODR &= ~(1U << 11);
	GPIOA->ODR &= ~(1U << 7);	//The letter d

	TurnOn_Digit(3);
	clear_Digit(3);
}

void display_Playback(void){
	clear_SSD();

	GPIOA->ODR &= ~(1U << 11);
	GPIOA->ODR &= ~(1U << 6);
	GPIOA->ODR &= ~(1U << 1);
	GPIOA->ODR &= ~(1U << 7);
	GPIOA->ODR &= ~(1U << 4);	//The letter P

	TurnOn_Digit(1);
	clear_Digit(1);

	GPIOA->ODR &= ~(1U << 11);
	GPIOA->ODR &= ~(1U << 6);
	GPIOA->ODR &= ~(1U << 12);	//The letter L

	TurnOn_Digit(2);
	clear_Digit(2);

	GPIOA->ODR &= ~(1U << 5);
	GPIOA->ODR &= ~(1U << 12);
	GPIOA->ODR &= ~(1U << 11);
	GPIOA->ODR &= ~(1U << 6);
	GPIOA->ODR &= ~(1U << 7);	//The letter b

	TurnOn_Digit(3);
	clear_Digit(3);
}

void initTIM16(void) {
	RCC->APBENR2 |= (1U << 17);

	TIM16->CR1 = 0;
	TIM16->CR1 |= (1U << 7);

	TIM16->CNT = 0;
	TIM16->PSC = 1;
	TIM16->ARR = 1000;

	TIM16->DIER |= (1U << 0);
	TIM16->CR1 |= (1U << 0);

	NVIC_SetPriority(TIM16_IRQn, 0);
	NVIC_EnableIRQ(TIM16_IRQn);
}

