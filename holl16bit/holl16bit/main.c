/*
 * holl16bit.c
 *
 * Created: 17.07.2017 12:10:44
 * Author : admin3d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int freq(int,int,int);
int freq(int);

int i;
int counter = 8;
volatile int seconds = 0;
int frequency = 0;
char str[8];

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

void setup() {
//  Serial.begin(9600);
//  DDRD  = 0xFF;   //настройка всех выводов порта D как выходов
  DDRC = 0; //настройка всех выводов порта C как входов
  PORTC = 1;  
  
  TCCR1A = 0;         // настройка таймера 1, канала А
  TCCR1B = 0x1;        // предделитель CLK/1024;
  OCR1A = 0x01FF; // прерывание раз в секунду на частоте 16MГц
  TIMSK1 = 0b00000010; // запуск таймера по совпадению 1А

  sei();              // разрешаем прерывания (запрещаем: cli(); )
  USART_Init(MYUBRR);
}

ISR(TIMER1_COMPA_vect)  //обработчик прерывания по совпадению 1А
{
  USART_Transmit(' ');
  
  itoa(frequency,str,10);
  USART_Transmit(str[0]);
  USART_Transmit(str[1]);
/*  USART_Transmit(str[2]);
  USART_Transmit(str[3]);
  USART_Transmit(str[4]);
  USART_Transmit(str[5]);
  USART_Transmit(str[6]);
  USART_Transmit(str[7]);*/
  if (seconds > 2) {
    seconds = 0;
    frequency = counter;
    //counter = 0;
//  Serial.println("DEFAULT");
  }
  else {
    seconds++;
    if (seconds == 3)
  counter = 0;
//    Serial.println(counter);
  }
//  PORTD = ~ PORTD;
  //обнуляем регистр TCNT
  TCNT1 = 0;
}

/*void loop() {
while(1)
{
  while (seconds == 3)
  {
    {
      real = (PINC & 1);
      if (real != prev)
      {
        counter++;
        //USART_Transmit('/');
        //Serial.println('*');
      }
      prev = real;
    }
  }
}
}*/

int main(void)
{
	setup();
    /* Replace with your application code */
//	loop();
}

int freq(int time_start, int period, int time_metering)
{
int prev=0, real=0;
	  while ((seconds >= time_start) & (seconds < time_start + time_metering))
	  {
		  {
			  real = (PINC & 1);
			  if (real != prev)
			  {
				  counter++;
				  //USART_Transmit('/');
				  //Serial.println('*');
			  }
			  prev = real;
		  }
	  }
}

int freq(int sec_now)
{
	int prev=0, real=0;
	while (seconds = 3)
	{
		{
			real = (PINC & 1);
			if (real != prev)
			{
				counter++;
				//USART_Transmit('/');
				//Serial.println('*');
			}
			prev = real;
		}
	}
}