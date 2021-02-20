/*Author: Ömer Faruk Aydın
 * 171024002
 * Project-3*/
#include "bsp.h"

uint8_t EEPROM1ADDRESS = 0X50;
uint8_t audioBuffer[32];
uint8_t audioRecord[32];
uint8_t audio;
uint16_t targetAddress = 0X00;
volatile int frequency = 16;
volatile uint32_t tick;
int readVoice;
int startRecord;
int display_record;
int display_readVoice;
int i;
int j;


void TIM3_IRQHandler(void) {
	if(readVoice == 1) {
		display_readVoice = 1;
		frequency = ((uint32_t)audioRecord[j]);
		TIM2->CCR2 = frequency;
		j++;

		if(j == 32) {
			read(EEPROM1ADDRESS, targetAddress, &audioRecord, 32);
			targetAddress += 32;
			j=0;
			++i;
		}

		if(i == 1000) {
			readVoice = 0;
			targetAddress = 0x00;
		}
	}

	TIM3->SR &= ~(1U << 0);
}

void TIM2_IRQHandler(void){
	TIM2->SR &= ~(1U << 0);		//Update Status Register

}

void EXTI0_1_IRQHandler(void){
	//Interrupt comes from PB0->C2
	clearRowsKeypad();
	GPIOA->ODR ^= (1U << 10);	//PA10->R3
	if ((GPIOB->IDR >> 0) & 1){
		//5
		readVoice = 1;
		TIM3->DIER |= (1U << 0);
		delay_ms(1000);
	}
	GPIOA->ODR ^= (1U << 10);	//PA10->R3
	GPIOB->ODR ^= (1U << 1);	//PB1->R2

	if ((GPIOB->IDR >> 0) & 1){
		//8

	}
	GPIOB->ODR ^= (1U << 1);	//PB1->R2
	GPIOA->ODR ^= (1U << 15);	//PA15->R1

	if ((GPIOB->IDR >> 0) & 1){
		//2

	}
	GPIOA->ODR ^= (1U << 15);	//PA15->R1
	EXTI->RPR1 |= (1U << 0);	//Clear interrupt flag
	setAllRows();
}

void EXTI2_3_IRQHandler(void){
	//Interrupt comes from PB2->C3
	clearRowsKeypad();
	GPIOA->ODR ^= (1U << 10);	//PA10->R3
	if ((GPIOB->IDR >> 2) & 1){
		//6
		startRecord = 1;
	}
	GPIOA->ODR ^= (1U << 10);	//PA10->R3
	GPIOB->ODR ^= (1U << 1);	//PB1->R2

	if ((GPIOB->IDR >> 2) & 1){
		//9
	}
	GPIOB->ODR ^= (1U << 1);	//PB1->R2
	GPIOA->ODR ^= (1U << 15);	//PA15->R1

	if ((GPIOB->IDR >> 2) & 1){
		//3
	}
	GPIOA->ODR ^= (1U << 15);	//PA15->R1
	EXTI->RPR1 |= (1U << 2);	//Clear interrupt flag
	setAllRows();
}

void EXTI4_15_IRQHandler(void){
	//Interrupt comes from PA9->C1
	clearRowsKeypad();
	GPIOA->ODR ^= (1U << 10);	//PA10->R3
	if ((GPIOA->IDR >> 9) & 1){
		//4
	}
	GPIOA->ODR ^= (1U << 10);	//PA10->R3
	GPIOB->ODR ^= (1U << 1);	//PB1->R2

	if ((GPIOA->IDR >> 9) & 1){
		//7
	}
	GPIOB->ODR ^= (1U << 1);	//PB1->R2
	GPIOA->ODR ^= (1U << 15);	//PA15->R1

	if ((GPIOA->IDR >> 9) & 1){
		//1
		for (int h = 0; h < 32; ++h){
		audioBuffer[h] = 0;
		audioRecord[h] = 0;
		}
		targetAddress = 0X00;
		tick = 0;
		i = 0;
		j = 0;
		frequency = 30;
		startRecord = 0;
		readVoice = 0;
		TIM2->CCR2 = 0;
		audio = 0;
		TIM3->DIER &= ~(1U << 0);
	}
	GPIOA->ODR ^= (1U << 15);	//PA15->R1
	EXTI->RPR1 |= (1U << 9);	//Clear interrupt flag
	setAllRows();
}

void write(uint8_t devAddr, uint16_t memAddr,uint8_t* data, int size){
	//Send address and register to read
	I2C1->CR2 = 0;
	I2C1->CR2 |= (uint32_t)(devAddr << 1);
	I2C1->CR2 |= (uint32_t)((size + 2)<< 16);
	I2C1->CR2 |= (1U << 25);	//Autoend
	I2C1->CR2 |= (1U << 13);	//Generate start

	while(!(I2C1->ISR & (1 << 1)));	//high address
	I2C1->TXDR = (uint32_t)(memAddr >> 8);

	while(!(I2C1->ISR & (1 << 1)));	//low address
	I2C1->TXDR = (uint32_t)(memAddr & 0xFF);

	while(size){
		while(!(I2C1->ISR & (1 << 1)));	//is flag busy
		I2C1->TXDR = (*data++);	//send data
		size--;
	}
}

void read(uint8_t devAddr,uint16_t memAddr,uint8_t* data, int size){
	//Send address and register to read
	I2C1->CR2 = 0;
	I2C1->CR2 |= (uint32_t)(devAddr << 1);
	I2C1->CR2 |= (2U << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 13);	//Generate Start

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(memAddr >> 8);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(memAddr & 0xFF);

	while(!(I2C1->ISR & (1 << 6)));	//is transmission complete

	//read data
	I2C1->CR2 = 0;
	I2C1->CR2 |= (uint32_t)(devAddr << 1);
	I2C1->CR2 |= (1U << 10);	//Read mode
	I2C1->CR2 |= (uint32_t)(size << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 25);	//AUTOEND
	I2C1->CR2 |= (1U << 13);	//Generate start

	while(size){
		while(!(I2C1->ISR & (1 << 2)));
		(*data++) = (uint8_t)I2C1->RXDR;
		size--;
	}

}

void ADC_COMP_IRQHandler(void){
	if (startRecord == 1) {
		display_record = 1;
		audio = (uint8_t) (ADC1->DR);
		audioBuffer[j] = audio % 256;
		j++;

		if(j == 32) {
			write(EEPROM1ADDRESS, targetAddress, &audioBuffer, 32);
			targetAddress += 32;
			j=0;
			++i;
			}

		if(i == 1000) {
			startRecord = 0;
			i=0;
			targetAddress = 0x00;
			ADC1->IER &= (0U << 2);
			TIM3->DIER &= (0U << 0);
			ADC1->CR |= (1U << 1);
		}

		}
		tick++;
		ADC1->ISR &= (1U << 2); // CLEAR INTERRUPT
}

void TIM16_IRQHandler(void) {
	if (display_record == 1)
		display_Record();

	if (display_readVoice == 1)
		display_Playback();

	TIM16->SR &= ~(1U << 0);
}

int main(void) {
	initTIM2();
    initPWMPin();
	Init_Keypad();
	InitSSD();
	initADC();
	initTIM3();
	I2C_Init();
	start_conversion();
	initTIM16();
	while (1) {

	}
    return 0;
}

uint16_t startADC(void){
	while(!(ADC1->ISR & (1U << 2)));	// Wait the data
	return ADC1->DR;					// EVENTUALLY DATA FROM PIN
}
