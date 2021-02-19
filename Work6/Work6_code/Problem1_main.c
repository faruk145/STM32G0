/*
 *	Problem-1
 *	Author:Ömer Faruk Aydın 171024002
 */
#include "bsp.h"
#include <stdlib.h>
#include <stdio.h>

int i;

void EXTI0_1_IRQHandler(void){
	clearRowsKeypad();
	GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
	if ((GPIOB->IDR >> 1) & 1){
		i = 500;				//D
	}
	GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
	GPIOB->ODR ^= (1U << 0);	//PB0 -> R3

	if ((GPIOB->IDR >> 1) & 1){
		i = 1000;				//C
	}

	GPIOB->ODR ^= (1U << 0);	//PB0 -> R3
	GPIOA->ODR ^= (1U << 9);	//PA9 -> R2

	if ((GPIOB->IDR >> 1) & 1){
		i = 1500;			//B
	}
	GPIOA->ODR ^= (1U << 9);	//PA9 -> R2
	GPIOA->ODR ^= (1U << 10);	//PA10 -> R1

	if ((GPIOB->IDR >> 1) & 1){
		i = 2000;			//A
	}
	GPIOA->ODR ^= (1U << 10);	//PA10 -> R1
	EXTI->RPR1 |= (1U << 1);	//Clear interrupt flag
	setAllRows();
}


void EXTI4_15_IRQHandler(void){
	if ((EXTI->RPR1 & (1 << 15))) {	//Interrupt comes from PA15
		clearRowsKeypad();
		GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
		/*# equal button is pressed*/
		if ((GPIOA->IDR >> 15) & 1){

			i = 2500;			//#
		}

		GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
		GPIOB->ODR ^= (1U << 0);	//PB0 -> R3

		if ((GPIOA->IDR >> 15) & 1){
			i = 3000;			//9
		}

		GPIOB->ODR ^= (1U << 0);	//PB0 -> R3
		GPIOA->ODR ^= (1U << 9);	//PA9 -> R2

		if ((GPIOA->IDR >> 15) & 1){
			i = 3500;			//6
		}

		GPIOA->ODR ^= (1U << 9);	//PA9 -> R2
		GPIOA->ODR ^= (1U << 10);	//PA10 -> R1

		if ((GPIOA->IDR >> 15) & 1){
			i = 4000;			//3
		}

		GPIOA->ODR ^= (1U << 10);	//PA10 -> R1
		EXTI->RPR1 |= (1U << 15);	//Clear interrupt flag
		setAllRows();
	}

	else if ((EXTI->RPR1 & (1 << 7))) {	//Interrupt comes from PB7
		clearRowsKeypad();
		GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
		if ((GPIOB->IDR >> 7) & 1){
			i = 4500;			//0
		}

		GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
		GPIOB->ODR ^= (1U << 0);	//PB0 -> R3

		if ((GPIOB->IDR >> 7) & 1){
			i = 5000;			//8
		}

		GPIOB->ODR ^= (1U << 0);	//PB0 -> R3
		GPIOA->ODR ^= (1U << 9);	//PA9 -> R2

		if ((GPIOB->IDR >> 7) & 1){
			i = 5500;			//5
		}

		GPIOA->ODR ^= (1U << 9);	//PA9 -> R2
		GPIOA->ODR ^= (1U << 10);	//PA10 -> R1

		if ((GPIOB->IDR >> 7) & 1){
			i = 6000;			//2
		}

		GPIOA->ODR ^= (1U << 10);	//PA10 -> R1
		EXTI->RPR1 |= (1U << 7);	//Clear interrupt flag
		setAllRows();
	}

	else if ((EXTI->RPR1 & (1 << 6))) {	//Interrupt comes from PB6
		clearRowsKeypad();
		GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
		if ((GPIOB->IDR >> 6) & 1){
			i = 6500;		//*
		}

		GPIOB->ODR ^= (1U << 2);	//PB2 -> R4
		GPIOB->ODR ^= (1U << 0);	//PB0 -> R3

		if ((GPIOB->IDR >> 6) & 1){
			i = 7000;		//7
		}

		GPIOB->ODR ^= (1U << 0);	//PB0 -> R3
		GPIOA->ODR ^= (1U << 9);	//PA9 -> R2

		if ((GPIOB->IDR >> 6) & 1){
			i = 9000;		//4
		}

		GPIOA->ODR ^= (1U << 9);	//PA9 -> R2
		GPIOA->ODR ^= (1U << 10);	//PA10 -> R1

		if ((GPIOB->IDR >> 6) & 1){
			i = 0;			//1
		}

		GPIOA->ODR ^= (1U << 10);	//PA10 -> R1
		EXTI->RPR1 |= (1U << 6);	//Clear interrupt flag
		setAllRows();
	}

}

void InitTimer3(){
		RCC->APBENR1 |=(1U << 1); // timer3 clock is enabled

		TIM3->CR1=0;
		TIM3->CR1 |=(1U << 7); //Setting ARPE bit
		TIM3->CNT =0;
		TIM3->PSC=0;
		TIM3->ARR=16000; // 1 sn
		TIM3->DIER |=(1U << 0); // enable interrupts

		TIM3->CCMR1 |=(1U << 3);

		TIM3->CCMR1 &= ~ (1U << 16); //0110  bits 16-6-5-4 for PWM mode 1
		TIM3->CCMR1 |=(1U << 6);
		TIM3->CCMR1 |= (1U << 5);
		TIM3->CCMR1 &= ~(1U << 4);

		TIM3->CCER |=(1U << 0);


		TIM3->CCR1 =0x84;


		TIM3->EGR |= (1U << 0);
		TIM3->CR1 |= (1U << 0);

		NVIC_SetPriority(TIM3_IRQn,1); // Setting Priority for timer handler
		NVIC_EnableIRQ(TIM3_IRQn);

}

void InitTimer2(){
		RCC->APBENR1 |=(1U << 0); // timer3 clock is enabled

		TIM2->CR1=0;
		TIM2->CNT =0;
		TIM2->PSC=0;
		TIM2->ARR=200;
		TIM2->DIER |=(1U << 0); // enable interrupts

		TIM2->CR1 |= (1U << 0);

		NVIC_SetPriority(TIM2_IRQn,1); // Setting Priority for timer handler
		NVIC_EnableIRQ(TIM2_IRQn);

}

void TIM2_IRQHandler(void) {
	TIM3->ARR= i;			//Assign frequency
	TIM3->EGR |= (1U << 0);		//Update generated event
	Display_Number(i);
	TIM2->SR &= ~(1U << 0);		//Update Status Register
	TIM2->SR &=  ~(1U<<1);

}

void TIM3_IRQHandler(void) {
	TIM3->SR &= ~(1U << 0);		//Update Status Register
	TIM3->SR &=  ~(1U<<1);
}


void SpeakerInit(){
	 /* Enable GPIOB and GPIOA clock */
	    RCC->IOPENR |= (3U << 0);
	 /* Setup PA6 as alternate function */

		GPIOA->MODER &= ~(3U << 2*6);
		GPIOA->MODER |=  (2<< 2*6) ;

		GPIOA->AFR[0]  &= ~(0xFU<<4*6);
		GPIOA->AFR[0]  |=  (1<<4*6);

}

int main(void) {
	Setup_Pins();
	SpeakerInit();
	 InitTimer2();
	 InitTimer3();
    while(1) {

    }
    return 0;
}
