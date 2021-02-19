/*
 *  Problem 2
 *	Author:Ömer Faruk Aydın 171024002
 */

#include "stm32g0xx.h"

void SystemInitial(void);
void InitTimer(void);
void LedInit(void);

uint32_t i=0 ;
uint8_t flag_for_up_or_down=0;


void SystemInitial(){

    LedInit();
	InitTimer();
}
void LedInit(){
	 /* Enable GPIOB and GPIOA clock */
	    RCC->IOPENR |= (3U << 0);
	 /* Setup PA6 as alternate function */

		GPIOA->MODER &= ~(3U << 2*6);
		GPIOA->MODER |=  (2<< 2*6) ;

		GPIOA->AFR[0]  &= ~(0xFU<<4*6);
		GPIOA->AFR[0]  |=  (1<<4*6);

}


void InitTimer(){
		RCC->APBENR1 |=(1U << 1); // timer3 clock is enabled

		TIM3->CR1=0;
		TIM3->CR1 |=(1U << 7); //Setting ARPE bit
		TIM3->CNT =0;
		TIM3->PSC=10;
		TIM3->ARR=(16000); // 1 sn
		TIM3->DIER |=(1U << 0); // enable interrupts

		TIM3->CCMR1 |=(1U << 3);

		TIM3->CCMR1 &= ~ (1U << 16); //0111  bits 16-6-5-4 for PWM mode 2
		TIM3->CCMR1 |=(1U << 6);
		TIM3->CCMR1 |= (1U << 5);
		TIM3->CCMR1 |= (1U << 4);

		TIM3->CCER |=(1U << 0);


		TIM3->CCR1 =0x0;


		TIM3->EGR |= (1U << 0);
		TIM3->CR1 |= (1U << 0);

		NVIC_SetPriority(TIM3_IRQn,1); // Setting Priority for timer handler
		NVIC_EnableIRQ(TIM3_IRQn);

}
void TIM3_IRQHandler(){

	/*
	 * When the duty cycle goes from 0 to 16000 gradually will become brighter and then flag will change and it will get less brighter.
 	 *
	 */
	if(i==16000)
		flag_for_up_or_down=1;
	else if(i==0)
		flag_for_up_or_down=0;

	if (!flag_for_up_or_down)
		i += 100;

	else if(flag_for_up_or_down)
		i-=100;

	TIM3->CCR1 =i;

	TIM3->SR &=  ~(1U<<0);
	TIM3->SR &=  ~(1U<<1);
}
int main(void) {
	SystemInitial();


    while(1) {
    }

    return 0;
}
