/*Author: Ömer Faruk Aydın 
Problem-1
*/

#include "bsp.h"


volatile int frequency = 16;
int changeComes = 0;

uint16_t voltageValue;
float led1Value = 0;
float led2Value = 0;

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
	TIM1->ARR = (1000); 	/* TIM1->ARR = (1600 * TIM1Count); */
	TIM1->DIER |= (1 << 0);
	TIM1->CR1 |= (1 << 0);
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2);
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
	voltageValue = startADC(); 			/* 0 - 4000 VALUE CHANGE RANGE */
	led1Value = (voltageValue * 0.25);	/* PERCENTAGE VALUE OF THE RANGE LED1 */
	led2Value = 1000 - led1Value;		/* PERCENTAGE VALUE OF THE RANGE LED2 */

	if (led1Value < 1) {
		led1Value = 0;
	}
	else if (led1Value > 999) {
		led1Value = 1000;
	}

	if (led2Value < 1) {
		led2Value = 0;
	}
	else if (led2Value > 999) {
		led2Value = 1000;
	}

	TIM3->CCR1 = frequency * (int)led1Value / 1000;
	TIM3->CCR2 = frequency * (int)led2Value / 1000;

	TIM1->SR &= ~(1U << 0);
}


void initTIM3(void) {
	/* Setup TIM3 */
	RCC->APBENR1 |= (1U << 1);
	TIM3->CR1 = 0;
	TIM3->CR1 |= (1U << 7); 		/* AUTO-RELOAD PRELOAD ENABLE */
	TIM3->CNT = 0; 					/* 0 */
	TIM3->PSC = 999;
	TIM3->ARR = frequency; 		/* TIM3->ARR = (1 * frequency); */
	TIM3->DIER |= (1U << 0);

	/* CH1 SETTING FOR PWM */
	TIM3->CCMR1 &= ~(0x7U << 4);	/* CLEAR OC1M (ATTENTION: OC1M[3] IS NOT IN THE SAME PLACE WITH OTHER BITS) */
	TIM3->CCMR1 &= ~(0x1U << 16);	/* CLEAR OC1M[3] */
	TIM3->CCMR1 |= (0x6U << 4);	    /* OC1M 0110: PWM mode 1 */
	TIM3->CCMR1 |= (1U << 3);	    /* ENABLE OUTPUT COMPARE 1 PRELOAD */
	TIM3->CCER |= (1U << 0);	    /* ENABLE CAPTURE/COMPARE 1 OUTPUT */
	//TIM3->CCR1 = 1000;				/* CHANNEL 1 PRELOAD */
	TIM3->CNT = 0;
	TIM3->CCR1 = frequency ;		/* CAPTURE/COMPARE REGISTER VALUE */

	/* CH2 SETTING FOR PWM */
	TIM3->CCMR1 &= ~(0X7U << 12); 	/* CLEAR OC2M (ATTENTION: OC2M[3] IS NOT IN THE SAME PLACE WITH OTHER BITS) */
	TIM3->CCMR1 &= ~(0X1U << 24); 	/* CLEAR OC2M[3] */
	TIM3->CCMR1 |= (0X6U << 12);  	/* OC2M 0110: PWM mode 1 */
	TIM3->CCMR1 |= (1U << 11); 		/* ENABLE OUTPUT COMPARE 2 PRELOAD */
	TIM3->CCER |= (1U << 4);		/* ENABLE CAPTURE/COMPARE 2 OUTPUT */
	TIM3->CCR2 = frequency;		/* CHANNEL 2 PRELOAD */

	TIM3->EGR |= (1<<0); 			/* UPDATE GENERATION */
	TIM3->CR1 |= (1U << 0);			/* ENABLE TIM3 */

	NVIC_SetPriority(TIM3_IRQn, 0);
	NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void) {


	TIM3->SR &= ~(1U << 0);
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
	GPIOA->MODER &= ~(3U << 2 * 7);		/* PA7 ANALOG MODE CLEAR */
	GPIOA->MODER |= (3U << 2 * 7);		/* PA7 ANALOG MODE */

	RCC->APBENR2 |= (1U << 20);			/* ENABLE ADC CLOCK */
	ADC1->CR |= (1U << 28);	 			/* ENABLE ADC VOLTAGE REGULATOR */
	delay_ms(100);						/* WAIT FOR ADC ENABLE  */

	ADC1->CR |= (1U << 31);				/* ENABLE ADC CALIBRATION  */
	while (ADC1->CR & (1 << 31));		/* WILL NOT COME OUT WITHOUT ADC CALIBRATION */

	ADC1->ISR &= ~(1 << 0);
	ADC1->CR |= (1U << 0);				/* ENABLE ADC */
	while (ADC1->ISR & (1 << 0));		/* WILL NOT COME OUT UNTIL ADC READY */

	ADC1->CHSELR |= (1U << 7);			/* ADC CHANNEL SELECTION REGISTER (PA7) */
	ADC1->CFGR1 |= (1U << 13);
	ADC1->CR |= (1U << 2);
}

uint16_t startADC(void){				/* START ADC */
	while(!(ADC1->ISR & (1U << 2)));	/* ANY DATA? TAKE IT AND LEAVE */
	return ADC1->DR;					/* EVENTUALLY DATA FROM PIN */
}
