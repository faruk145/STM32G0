#ifndef BSP_H_
#define BSP_H_

#include "stm32g0xx.h"
//Common API function for Nucleo STM32G031 board
void clear_SSD(void);void Setup_Pins(void);
void printChar(uint8_t);
int _write(int, char*, int);
void BSP_UART_init(uint32_t);
void print(char*);
void SysTick_Handler(void);
void delay_ms(uint32_t);
uint8_t Single_read(uint8_t, uint8_t);
void Multi_read(uint8_t, uint8_t, uint8_t *, uint32_t);
void Single_write(uint8_t, uint8_t, uint8_t);
void Multi_write(uint8_t , uint8_t *, uint32_t);
#endif /* BSP_H_ */
