/*
 * holl16bit.c
 *
 * Created: 17.07.2017 12:10:44
 * Author : admin3d
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "frequency.h"

//#define	PORT_Freq	PINC		//Порт вентилятора

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR ((FOSC/16/BAUD)-1)

void USART_Init( unsigned int ubrr)
{
	//Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	// Put data into buffer, sends the data
	UDR0 = data;
}

/*
void setup() {
	//  Serial.begin(9600);
	//  DDRD  = 0xFF;   //настройка всех выводов порта D как выходов
	DDRC = 0; //настройка всех выводов порта C как входов
	PORTC = 1;
	
	TCCR1A = 0;         // настройка таймера 1, канала А
	TCCR1B = 0x5;        // предделитель CLK/1024;
	OCR1A = 0x3D09; // прерывание раз в секунду на частоте 16MГц
	TIMSK1 = 0b00000010; // запуск таймера по совпадению 1А

	sei();              // разрешаем прерывания (запрещаем: cli(); )
	USART_Init(MYUBRR);
}
*/

void loop() {
	int i;	
	char str[8];
//	int *frequency=new int[8];
	int *frequency[8];
	int mass[8]={0,0,0,0,0,0,0,0};
	for (i=0;i<=7;i++)
	{
		frequency[i]=&mass[i];
	}
while(1)
{
//	i=0;
	
//	freq(4,3,1);
	USART_Transmit(' ');
//	frequency=freq(frequency,PINC,1,4,3,1);
//	*frequency[0]=0;
	freq(frequency,&PINC,1,4,3,1);
	for (i=0;i<=7;i++)
	{
		if (*frequency[i]>0)
		{
			itoa(*frequency[i],str,10);
			USART_Transmit(str[0]);
			USART_Transmit(str[1]);
			USART_Transmit(' ');
		}
	}
/*
	i=freq(4,3,1);
	if (i>0)
	itoa(i,str,10);*/
/*
	USART_Transmit(str[0]);
	USART_Transmit(str[1]);*/
//	if (counter = 1) USART_Transmit('[');
}
}

int main(void)
{
	//  DDRD  = 0xFF;   //настройка всех выводов порта D как выходов
	DDRC = 0; //настройка всех выводов порта C как входов
	PORTC = 1;	
	USART_Init(MYUBRR);
	
	setup_TC1();
    /* Replace with your application code */
	loop();
}