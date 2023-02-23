#include"stm32f0xx.h"
//#include "display.h"
//This is a driver of a standard  HD 44780 alphanumeric display.
//Data flows through PORT A,  RW connect to the ground , data flows as chunks (4 bits).
// 
//  porta0-d4
//  porta1-d5
//  porta2-d6
//  porta3-d7
//   porta4-RS
//   porta5-CE
//!!!!!!!!!!The port is configurated in the  InitDisplay()  - if you use an another port - change it!
//
//
//
#define LCDPORT 	GPIOA->ODR  //data flows through port A PA0-PA3        
#define SET_RS GPIOA->BSRR=GPIO_BSRR_BS_4;
#define RESET_RS GPIOA->BSRR=GPIO_BSRR_BR_4;
#define HIGH_CE GPIOA->BSRR=GPIO_BSRR_BS_5;
#define LOW_CE GPIOA->BSRR=GPIO_BSRR_BR_5;
#define ONEMS (uint32_t)(60000/4);// 32MHz - clock frequency of a MCU
#define ONEUS  (uint32_t)(62/4);// 32MHz - clock frequency of a MCU
		

volatile uint32_t d1;
void Delay_ms(uint32_t de)
{
	d1=de*ONEMS;
	while(d1>0)
	{d1--;
	}
	
}
void Delay_us(uint32_t de)
{
	 d1=de*ONEUS;
	while(d1>0)
	{d1--;
	}
	
}



void SendData(uint8_t data){

	uint8_t reciveddata;
	
	reciveddata=data;
	///////////////high 4 bits
	reciveddata>>=4;
	SET_RS //HIGH RS
	
	LCDPORT&=(uint32_t)0xf0;//0xf0;//resete А0-А3
	LCDPORT|=(uint32_t)reciveddata;//high nibble
	Delay_us(25);
	HIGH_CE	//HIGH CE  
	Delay_us(25);      					//
  LOW_CE   //LOW CE
  Delay_us(25);
///////////////////low 4 bits
	reciveddata=data;
	reciveddata&=0x0f;
	LCDPORT&=(uint32_t)0xf0;//reset bits 
	LCDPORT|=(uint32_t)reciveddata;//low nibble
	Delay_us(25); 
	HIGH_CE  //  HIGH CE
	Delay_us(25);							 //
	LOW_CE  // LOW CE
	Delay_us(25);
	RESET_RS //LOW RS
	Delay_us(300);
	
}
void SendCommand(uint8_t com){
	
	 
	uint8_t reciveddata;
	
	reciveddata=com;
	///////////////high 4 bits
	reciveddata>>=4;
	RESET_RS //LOW RS
	LCDPORT&=(uint32_t)0xf0;;//reset bits А0-А3
	LCDPORT|=(uint32_t)reciveddata;//high nibble
	Delay_us(25);
	HIGH_CE 	//HIGH CE  
	Delay_us(25);      					//
	LOW_CE   //LOW CE
  Delay_us(25);
///////////////////low 4bits
	reciveddata=com;
	reciveddata&=0x0f;
	LCDPORT&=(uint32_t)0xf0;;//reset bits 
	LCDPORT|=(uint32_t)reciveddata;//low nibble
	Delay_us(25); 
	HIGH_CE  //  HIGH CE
	Delay_us(25);							 //
	LOW_CE  // LOW CE
	Delay_us(25);
	 SET_RS //HIGH RS
	Delay_us(300);

}


void InitDisplay(void){
	
	GPIOA->MODER|=(uint32_t)GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER2_0|GPIO_MODER_MODER3_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0;
	GPIOA->OSPEEDR|=(uint32_t)GPIO_OSPEEDR_OSPEEDR0|GPIO_OSPEEDR_OSPEEDR1|GPIO_OSPEEDR_OSPEEDR2|GPIO_OSPEEDR_OSPEEDR3|GPIO_OSPEEDR_OSPEEDR4|GPIO_OSPEEDR_OSPEEDR5;
	Delay_ms(150);
	LCDPORT=0;//CLR all bits
	///////////////////////////////////////////////////first time command num 1
	LCDPORT&=(uint32_t)0xf0;;//clear А0-А3
	LCDPORT|=(uint32_t)3;//command number 1 8bit interface
	Delay_us(25);
	HIGH_CE 	//HIGH CE  
	Delay_us(25);      					//
	LOW_CE   //LOW CE
  Delay_ms(12);
		///////////////////////////////////////////////////second time
	Delay_us(25);
	HIGH_CE 	//HIGH CE  
	Delay_us(25);      					//
	LOW_CE   //LOW CE
  Delay_us(100);
///////////////////////////////////////////////////thrid  time
	
	HIGH_CE 	//HIGH CE  
	Delay_us(25);      					//
	LOW_CE   //LOW CE
  Delay_us(100);
	
	
	////////////////////////////////
	LCDPORT&=(uint32_t)0xf0;;//clear А0-А3
	LCDPORT|=(uint32_t)2;//the command number 2 8bit interface
	Delay_us(25);
	HIGH_CE 	//HIGH CE  
	Delay_us(25);      					//
	LOW_CE   //LOW CE
  Delay_ms(1);
	///////////////////////////////////
	SendCommand(0x2c);//2 strings , big font
	 Delay_us(100);
	 	SendCommand(8);//off
		Delay_us(100);
		SendCommand(0x08);//clear
		Delay_ms(3);
	SendCommand(0x06);
		Delay_us(100);
		SendCommand(0x0e);//turn on the d isplay the cursor not blinking
		Delay_us(100);
	
	
}


void Clear(void)
{
	SendCommand(0x01);
	Delay_ms(3);
}

void RetHome(void)
{
	SendCommand(0x02);
	Delay_ms(3);
}
void GoTo2(void)
{
	SendCommand(0xa8);
	
	
}
void GoTo1(void)
{
	SendCommand(0x80);
	
	
}

void DisPuts1(const char* str)
{
	
	 uint8_t c=0;
	while((c<40)&&(str[c]!=0))
	{
		SendData(str[c]);
		c++;
	}
	
	
	
}

