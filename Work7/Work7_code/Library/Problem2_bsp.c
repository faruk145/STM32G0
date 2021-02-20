/*Author: Ömer Faruk Aydın
Problem-2
*/

#include "bsp.h"


volatile int frequency = 16;


void delay_ms(uint32_t s) {
	for (int i = 0; i < s; ++i);
}

void initTIM1(void) {
	/* Setup TIM1 */
	RCC->APBENR2 |= (1U << 11);
	TIM1->CR1 = 0;
	TIM1->CR1 |= (1 << 7);
	TIM1->CNT = 0;
	TIM1->PSC = 9;
	TIM1->ARR = (16); 	/* TIM1->ARR = (1600 * TIM1Count); */	//Bir zamanlar 1000 idi.
	TIM1->DIER |= (1 << 0);
	TIM1->CR1 |= (1 << 0);
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2);
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}





void initTIM3(void) {
	/* Setup TIM3 */
	RCC->APBENR1 |= (1U << 1);
	TIM3->CR1 = 0;
	TIM3->CR1 |= (1U << 7); 		/* AUTO-RELOAD PRELOAD ENABLE */
	TIM3->CNT = 0; 					/* 0 */
	TIM3->PSC = 0;
	TIM3->ARR = 16000; 		/* TIM3->ARR = (1 * frequency); */
	TIM3->DIER |= (1U << 0);

	TIM3->EGR |= (1<<0); 			/* UPDATE GENERATION */
	TIM3->CR1 |= (1U << 0);			/* ENABLE TIM3 */

	NVIC_SetPriority(TIM3_IRQn, 0);
	NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void) {


	TIM3->SR &= ~(1U << 0);
}

void InitSSD(void){
	RCC->IOPENR |= (3U << 0);	//Enable GPIOA and GPIOB ports

	/*Setup SSD*/

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

	GPIOB->MODER &= ~(3U << 2*9);	//PB9 for D1 in SSD
	GPIOB->MODER |= (1U << 2*9);

	GPIOB->MODER &= ~(3U << 2*8);	//PB8 for D2 in SSD
	GPIOB->MODER |= (1U << 2*8);

	GPIOB->MODER &= ~(3U << 2*2);	//PB2 for D3 in SSD
	GPIOB->MODER |= (1U << 2*2);

	GPIOB->MODER &= ~(3U << 0);		//PB0 for D4 in SSD
	GPIOB->MODER |= (1U << 0);
}

void InitButton(void){
	GPIOB->MODER &= ~(3U << 2*4);	//PB4 for button
	GPIOB->PUPDR |= (2U << 2*4);	//Pull-down mode
	EXTI->EXTICR[1] |= (1U << 0);	//PB4 Interrupt Setup
	EXTI->RTSR1 |= (1U << 4);		//Rising Edge
	EXTI->IMR1 |= (1U << 4);		//Mask
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
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
			GPIOB->ODR &= ~(1U << 9);	//PB9 for D1 in SSD
			break;
		case 2:
			GPIOB->ODR &= ~(1U << 8);	//PB8 for D2 in SSD
			break;
		case 3:
			GPIOB->ODR &= ~(1U << 2);	//PB2 for D3 in SSD
			break;
		case 4:
			GPIOB->ODR &= ~(1U << 0);	//PB0 for D4 in SSD
			break;
	}
}

void TurnOn_Digit(int x){
	switch(x){
		case 1:
			GPIOB->ODR |= (1U << 9);	//PB9 for D1 in SSD
			break;
		case 2:
			GPIOB->ODR |= (1U << 8);	//PB8 for D2 in SSD
			break;
		case 3:
			GPIOB->ODR |= (1U << 2);	//PB2 for D3 in SSD
			break;
		case 4:
			GPIOB->ODR |= (1U << 0);	//PB0 for D4 in SSD
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
	GPIOB->MODER &= ~(3U << 2 *4);		/* PB4 ALTERNA MODE CLEAR */
	GPIOB->MODER |= (2U << 2 *4);	    /* PB4 ALTERNA MODE */
	GPIOB->OSPEEDR |= (3U << 2 *4);	    /* HIGH PRECISION ON CHANGE */
	GPIOB->AFR[0] &= ~(0xFU << 4 *4);   /* AFSEL4 = RESET and PB4->AF1 = TIM3_CH1 */
	GPIOB->AFR[0] |= (1U << 4 *4);	    /* AFSEL4 -> AF1 [0001] */

	GPIOB->MODER &= ~(3U << 2 *5);      /* PB5 ALTERNA MODE CLEAR */
	GPIOB->MODER |= (2U << 2 *5);	    /* PB5 ALTERNA MODE */
	GPIOB->OSPEEDR |= (3U << 2 *5);	    /* HIGH PRECISION ON CHANGE */
	GPIOB->AFR[0] &= ~(0xFU << 4 *5);   /* AFSEL5 = RESET and PB5->AF1 = TIM3_CH2 */
	GPIOB->AFR[0] |= (1U << 4 *5);	    /* AFSEL5 -> AF1 [0001] */

}

void initADC(void) {
	RCC->IOPENR |= (1U << 0);			/* A BLOCK RCC OPEN */
	GPIOA->MODER &= ~(3U << 0);		/* PA0 ANALOG MODE CLEAR */
	GPIOA->MODER |= (3U << 0);		/* PA0 ANALOG MODE */

	RCC->APBENR2 |= (1U << 20);			/* ENABLE ADC CLOCK */
	ADC1->CR |= (1U << 28);	 			/* ENABLE ADC VOLTAGE REGULATOR */
	delay_ms(100);						/* WAIT FOR ADC ENABLE  */

	ADC1->CR |= (1U << 31);				/* ENABLE ADC CALIBRATION  */
	while (ADC1->CR & (1 << 31));		/* WILL NOT COME OUT WITHOUT ADC CALIBRATION */

	ADC1->CFGR1 |= (1U << 4);
	ADC1->CHSELR |= (1U << 0);			/* ADC CHANNEL SELECTION REGISTER (PA0) */
	ADC1->CFGR1 |= (1U << 13);


	ADC1->CR |= (1U << 0);				/* ENABLE ADC */
	while (ADC1->ISR & (1 << 0));		/* WILL NOT COME OUT UNTIL ADC READY */


	ADC1->CR |= (1U << 2);
}
