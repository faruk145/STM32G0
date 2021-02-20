/*Author: Ömer Faruk Aydın
Problem-2
*/
#include "bsp.h"

uint16_t audio;
int i;

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
	audio = startADC(); 			/* 0 - 255 VALUE RANGE */
	delay_ms(1000);
	Display_Number(i);

	if (audio > 230){
		Display_Number(i++);
		delay_ms(1000);
	}

	TIM1->SR &= ~(1U << 0);

}

void EXTI4_15_IRQHandler(void){

	if ((GPIOB->IDR >> 4) & 1){
		i = 0;		//Reset Counter
		delay_ms(1000);
	}
	EXTI->RPR1 |= (1U << 4);	//Clear interrupt flag
}


int main(void) {
	InitSSD();
	InitButton();
    initTIM3();
    initADC();
    initTIM1();

	while (1) {
	}
    return 0;
}

uint16_t startADC(void){				/* START ADC */
	while(!(ADC1->ISR & (1U << 2)));	/* ANY DATA? TAKE IT AND LEAVE */
	delay_ms(1000);
	return ADC1->DR;					/* EVENTUALLY DATA FROM PIN */
}
