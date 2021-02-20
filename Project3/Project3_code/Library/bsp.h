/*Author: Ömer Faruk Aydın
 * 171024002
 * Project-3*/
#ifndef INC_BSP_H_
#define INC_BSP_H_

#include "stm32g0xx.h"

void initTIM1(void);
void initTIM2(void);
void initTIM3(void);
void initPWMPin(void);
void initADC(void);
uint16_t startADC(void);
void setSSD(int);
void InitSSD(void);
void clear_SSD(void);
void clear_Digit(int);
void TurnOn_Digit(int);
int ndigit(int);
void Display_Number(int);
void Init_Keypad(void);
void setAllRows(void);
void clearRowsKeypad(void);
void delay_ms(uint32_t);
void I2C_Init(void);
void display_Record(void);
void display_Playback(void);
void initTIM16(void);
void write(uint8_t , uint16_t,uint8_t*, int);
void read(uint8_t ,uint16_t,uint8_t* , int);
#endif /* INC_BSP_H_ */
