#include "stm32f4xx.h"                  // Device header

void adc1_configure(void);
short adc1_getSample(void);

void delay_ms(int time_in_ms);
short sampledVal;

int main(){
	adc1_configure();
	while(1){
		sampledVal=adc1_getSample();//save sample in sampledVal
		delay_ms(0xFFF);
	}
	//return 0;
}

void adc1_configure(void){
	/*
		PIN		ANALOG CHANNEL
		PB0		AN8
		PB1   AN9
	*/
	//Turn on GPIOB
	RCC->AHB1ENR|=(RCC_AHB1ENR_GPIOBEN);
	//Configure as analog inputs PB0 and PB1
	GPIOB->MODER&=~(GPIO_MODER_MODER0|GPIO_MODER_MODER1);//clean MODER0 and MODER1 Fields 
	GPIOB->MODER|=((0x3<<2)|// [3:2] MODER1 = 0x3: Analog mode
								(0x3<<0));// [1:0] MODER0 = 0x3: Analog mode
	//Turn con  ADC1
	RCC->APB2ENR|=(RCC_APB2ENR_ADC1EN);
	//configure ADC1 in single sample mode
	ADC1->CR1=0;//clear CR1 register 
	ADC1->CR2=0;//clear CR2 register
	ADC1->CR2|=(ADC_CR2_ADON);//enable adc 
	ADC1->SQR1&=~(ADC_SQR1_L);//clean L field
	ADC1->SQR1|=(0x0<<20);// [23:20] L = 0x0: 1 conversion  
	ADC1->SQR3&=~(ADC_SQR3_SQ1);//clean SQ1 field
	ADC1->SQR3|=((0x9<<5)|//[9:5] SQ2 = 0x9: conversion 2 from AN9 (since L = 0 this will be ingored) 
							(0x8<<0));//[4:0]	SQ1 = 0x8: conversion 1 from AN8 
}

void delay_ms(int time_in_ms){
	for(int i=0; i<time_in_ms; i++);
}

short adc1_getSample(void){
	ADC1->CR2|=(ADC_CR2_SWSTART);//start adc;
	delay_ms(0xFF);
	return ADC1->DR;
}
