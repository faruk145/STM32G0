/*
 *	Problem-2
 *	Author:Ömer Faruk Aydın 171024002
 */
#include "bsp.h"
#include <stdlib.h>
#include <stdio.h>

#define MPU6050_ADDRESS			0x68
#define MPU6050_WHO_AM_I		0x75
#define MPU6050_PWR_MGMT_1		0x6B

#define MPU6050_ACCEL_XOUT_H	0x3B
#define MPU6050_ACCEL_XOUT_L	0x3C

#define MPU6050_ACCEL_YOUT_H	0x3D
#define MPU6050_ACCEL_YOUT_L	0x3E

#define MPU6050_GYRO_XOUT_H		0x43
#define MPU6050_GYRO_XOUT_L		0x44


void I2C1_IRQHandler(void){

	//Only enters when error.
}

/*MPU6050-> PB8 PB9*/
void init_I2C(void){
		/*Enable GPIOB*/
		RCC->IOPENR |= (1U << 1);
		/*Setup PB8 as AF6*/
		GPIOB->MODER &= ~(3U << 2*8);
		GPIOB->MODER |= (2U << 2*8);
		GPIOB->OTYPER |= (1U << 9);
		/*Choose AF6 from mux*/
		GPIOB->AFR[1] &= ~(0xFU << 4*0);
		GPIOB->AFR[1] |= (6U << 4*0);
		/*Setup PB9 as AF6*/
		GPIOB->MODER &= ~(3U << 2*9);
		GPIOB->MODER |= (2U << 2*9);
		/*Choose AF6 from mux*/
		GPIOB->AFR[1] &= ~(0xFU << 4*1);
		GPIOB->AFR[1] |= (6U << 4*1);

		/*I2C1 enable*/
		RCC->APBENR1 |= (1U << 21);
		I2C1->CR1 = 0;
		I2C1->CR1 |= (1U << 7);

		I2C1->TIMINGR |= (3U << 28);	//PRESC
		I2C1->TIMINGR |= (0x13U << 0);	//SCLL
		I2C1->TIMINGR |= (0xFU << 8);	//SCLH
		I2C1->TIMINGR |= (0x2U << 20);	//SDADEL
		I2C1->TIMINGR |= (0x4U << 20);	//SCLDEL

		I2C1->CR1 = (1U << 0);	//PE

		NVIC_SetPriority(I2C1_IRQn, 1);
		NVIC_EnableIRQ(I2C1_IRQn);
}

uint8_t Single_read(uint8_t devAddr, uint8_t regAddr){
	/*WRITE OPERATION (Send address and register to read)*/
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);	//slave address
	I2C1->CR2 |= (1U << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 13);	//Generate Start
	while(!(I2C1->ISR & (1U << 1)));	//TXIS

	I2C1->TXDR = (uint32_t)regAddr;
	while(!(I2C1->ISR & (1U << 6)));	//TC

	/*READ OPERATION (read data)*/
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);
	I2C1->CR2 |= (1U << 10);	//READ mode
	I2C1->CR2 |= (1U << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 15);	//Nack
	I2C1->CR2 |= (1U << 25);	//Autoend

	I2C1->CR2 |= (1U << 13);	//Generate Start
	while(!(I2C1->ISR & (1U << 2)));	//Wait until RXNE is 1

	uint8_t data = (uint8_t)(I2C1->RXDR);
	return data;
}

void Multi_read(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint32_t num){
	/*WRITE OPERATION (Send address and register to read)*/
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);	//slave address
	I2C1->CR2 |= (1U << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 13);	//Generate Start
	while(!(I2C1->ISR & (1U << 1)));	//TXIS

	I2C1->TXDR = (uint32_t)regAddr;
	while(!(I2C1->ISR & (1U << 6)));	//TC

	/*READ OPERATION (read data)*/
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);
	I2C1->CR2 |= (1U << 10);	//READ mode
	I2C1->CR2 |= (num << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 15);	//Nack
	I2C1->CR2 |= (1U << 25);	//Autoend
	I2C1->CR2 |= (1U << 13);	//Generate Start

	for (size_t i = 0; i < num; i++){
		while(!(I2C1->ISR & (1U << 2)));	//Wait until RXNE is 1
		data[i] = (uint8_t)(I2C1->RXDR);
	}
}

void Single_write(uint8_t devAddr, uint8_t regAddr, uint8_t data){
	/*WRITE OPERATION (Send address and register to read)*/
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);	//slave address
	I2C1->CR2 |= (2U << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 25);	//Autoend
	I2C1->CR2 |= (1U << 13);	//Generate Start

	while(!(I2C1->ISR & (1U << 1)));	//TXIS
	I2C1->TXDR = (uint32_t)regAddr;

	while(!(I2C1->ISR & (1U << 1)));	//TXIS
	I2C1->TXDR = (uint32_t)data;
}

void Multi_write(uint8_t devAddr, uint8_t *data, uint32_t num){
	/*WRITE OPERATION (Send address and register to read)*/
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);	//slave address
	I2C1->CR2 |= (num << 16);	//Number of bytes
	I2C1->CR2 |= (1U << 25);	//Autoend
	I2C1->CR2 |= (1U << 13);	//Generate Start

	for (size_t i = 0; i < num; ++i){
		while(!(I2C1->ISR & (1U << 1)));	//TXIS
		I2C1->TXDR = data[i];
	}
}

int main(void) {
	BSP_UART_init(9600);
	init_I2C();
	uint8_t data[10];	//stack, garbage data

	Multi_read(MPU6050_ADDRESS, MPU6050_WHO_AM_I, data, 1);
	printf("WHOAMI:%x\r\n",data[0]);

	Multi_read(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, data, 1);
	printf("PWR MG:%x\r\n",data[0]);

	Single_write(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 0x00);
	delay_ms(1000);

	Multi_read(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, data, 1);
	printf("PWR MG:%x\r\n",data[0]);

    while(1) {
    	delay_ms(10);
    	Multi_read(MPU6050_ADDRESS, MPU6050_GYRO_XOUT_L, data, 2);
    	uint32_t b = (uint32_t)data[0] | ((uint32_t)(data[1]) << 8);
    	//float a = (float)data / 16384.0;
    	double a = (double)b / 131.0;
    	printf("%f\r\n",a);
    }
    return 0;
}
