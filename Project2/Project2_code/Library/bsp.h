/*
bsp.h
*/
#ifndef BSP_H_
#define BSP_H_
#include "stm32g0xx.h"
//Common API function for Nucleo STM32G031 board
void clear_SSD(void);
void Setup_Pins(void);
void setAllRows(void);
void clearRowsKeypad(void);
void setSSD(int);
void idle(void);
void clear_Digit(int);
void TurnOn_Digit(int);
int ndigit(int);
void Display_Number(int);
void delay(void);
void discard_array(char *);
void display_Overflow(void);
void Display_Floating_Number(double);
void Display_Invalid(void);
void reset_values(void);
#endif /* BSP_H_ */
