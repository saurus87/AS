﻿/*Подсчет импульсов на заданной ножке с использованием таймера/счетчика TC1*/

#pragma once

#include <avr/interrupt.h>

volatile unsigned int g_seconds = 0;
volatile unsigned int g_period = 0;

//PIN1
#define PORT1_freq PINB
#define PIN1_freq PINB4

//PIN2
#define PORT2_freq PIND
#define PIN2_freq PIND4

//PIN3
#define PORT3_freq PINB
#define PIN3_freq PINB5

//PIN4
#define PORT4_freq PINC
#define PIN4_freq PINC0


//Настройка на подсчет секунд глобальной переменной g_seconds
//period - период замера импульсов в секундах 
void setup_TC1(unsigned int period) {
	TCCR1A = 0;			//Настройка таймера 1, канала А
	TCCR1B = 0x5;			//Предделитель CLK/1024;
	OCR1A = 0x2DC6;			//Прерывание раз в секунду на частоте 12MГц
	TIMSK1 = 0x2;			//Запуск таймера по совпадению 1А
	sei();				//Разрешаем прерывания (запрещаем: cli(); )
	g_period = period;
}

//Обработчик прерывания по совпадению 1А
//Счетчик времени
ISR(TIMER1_COMPA_vect)
{
  if (g_seconds >= g_period-1) 
  {
    g_seconds = 0;
  }
  else 
  {
    g_seconds++;
  }  
  TCNT1 = 0;		//Обнуление регистра счетчика TCNT1
}


//Мгновенный замер импульсов на порте PORT_name и ножке PIN_Freq
//time_metering - длительность замера
/*
int freq(volatile uint8_t *PORT_name, uint8_t PINS_freq, unsigned int time_metering)
{
	g_seconds = 0;
	TCNT1 = 0;
	int frequency=0;
	int prev=0, real=0;
	while (g_seconds < time_metering)
	{
		real = (*PORT_name & PINS_freq);
		if (real != prev)
		{
			frequency++;
		}
		prev = real;
	}
	return (int)(frequency/2/time_metering);
}
*/

//Мгновенный замер импульсов на порте PORT_name, на ножках PINS_freq
//time_metering - длительность замера
//Выводит импульсы в массив по указателю *frequency соответственно ножкам порта (*frequency[0]=PINx0, *frequency[0]=PINx1,...)
/*
void freq(int* frequency[], volatile uint8_t *PORT_name, uint8_t PINS_freq, unsigned int time_metering)
{
	g_seconds = 0;
	TCNT1 = 0;
	int prev[8]={0,};
	int real[8]={0,};
	int i=0;
	char reg_while=0;
	while (g_seconds < time_metering)
	{
		if (reg_while ==0)
		{
			for (i=0;i<=7;i++)
			{
				*frequency[i]=0;
			}
			reg_while=1;
		}

		for (i=0;i<=7;i++)
		{
			if ((PINS_freq>>i)& 1)
			{
				real[i] = (*PORT_name & (i+1));
				if (real[i] != prev[i])
				{
					*frequency[i]=*frequency[i]+1;
				}
				prev[i] = real[i];
			}
		}
	}
	if (reg_while==1)
	{
		for (i=0;i<=7;i++)
		{
			*frequency[i]=(int)(*frequency[i]/2/time_metering);
		}
	}
	
}
*/

//Счетчик импульсов на порте PORT_name и ножке PINS_freq с периодом 
//period - период замера импульсов в секундах, time_start - время начала замера, time_metering - длительность замера
/*
int freq(volatile uint8_t *PORT_name, uint8_t PINS_freq, unsigned int time_start, unsigned int time_metering)
{
	int frequency=0;
	int prev=0, real=0;
	while ((g_seconds >= time_start ) &(g_seconds < time_start + time_metering))
	{
		real = (*PORT_name & PINS_freq);
		if (real != prev)
		{
			frequency++;
		}
		prev = real;
	}
	return (int)(frequency/2/time_metering);
}
*/


//Счетчик импульсов на порте PORT_name, на ножках PINS_freq
//time_start - время начала замера, time_metering - длительность замера
//Выводит импульсы в массив по указателю *frequency соответственно ножкам порта (*frequency[0]=PINx0, *frequency[0]=PINx1,...)
/*
void freq(int* frequency[], volatile uint8_t *PORT_name, uint8_t PINS_freq, unsigned int time_start, unsigned int time_metering)
{
	int prev[8]={0,};
	int real[8]={0,};
	int i=0;
	char reg_while=0;
	while ((g_seconds >= time_start ) &(g_seconds < time_start + time_metering))
	{
		if (reg_while ==0)
		{
			for (i=0;i<=7;i++)
			{
				*frequency[i]=0;
			}
			reg_while=1;
		}

		for (i=0;i<=7;i++)
		{
			if ((PINS_freq>>i)& 1)
			{
				real[i] = (*PORT_name & (i+1));
				if (real[i] != prev[i])
				{
					*frequency[i]=*frequency[i]+1;
				}
				prev[i] = real[i];
			}
		}
	}
	if (reg_while==1)
	{
		for (i=0;i<=7;i++)
		{
			*frequency[i]=(int)*frequency[i]/2/time_metering;
		}
	}	
}
*/


//Счетчик импульсов на портах PORT1_freq, PORT2_freq, PORT3_freq, PORT4_freq
// и ножках PIN1_freq, PIN2_freq, PIN3_freq, PIN4_freq
//time_metering - длительность замера
//Выводит количество подсчитаных за время time_metering импульсов
// в массив по указателю *frequency соответственно ножкам порта (*frequency[0]=PINx0, *frequency[0]=PINx1,...)
void freq(int* frequency[], unsigned int time_start, unsigned int time_metering)
{
	int prev[4]={1,};
	int real[4]={1,};
	int i=0;
	char reg_while=0;
	while ((g_seconds >= time_start ) &(g_seconds < time_start + time_metering))
	{
		if (reg_while == 0)
		{
			for (i=0;i<=3;i++)
			{
				*frequency[i]=0;
			}
			reg_while=1;
		}
		
		//PIN1
		real[0] = (PORT1_freq & (1<<(PIN1_freq)));
		if (real[0] != prev[0])
		{
			*frequency[0]=*frequency[0]+1;
		}
		prev[0] = real[0];
		
		//PIN2
		real[1] = (PORT2_freq & (1<<(PIN2_freq)));
		if (real[1] != prev[1])
		{
			*frequency[1]=*frequency[1]+1;
		}
		prev[1] = real[1];
		
		//PIN3
		real[2] = (PORT3_freq & (1<<(PIN3_freq)));
		if (real[2] != prev[2])
		{
			*frequency[2]=*frequency[2]+1;
		}
		prev[2] = real[2];
		
		//PIN4
		real[3] = (PORT4_freq & ((1<<PIN4_freq)));
		if (real[3] != prev[3])
		{
			*frequency[3]=*frequency[3]+1;
		}
		prev[3] = real[3];
		
	}
	if (reg_while==1)
	{
		for (i=0;i<=3;i++)
		{
			*frequency[i]=(int)((*frequency[i])/2);
		}
	}
	
}


//Мгновенный замер импульсов на портах PORT1_freq, PORT2_freq, PORT3_freq, PORT4_freq
// и ножках PIN1_freq, PIN2_freq, PIN3_freq, PIN4_freq
//time_start - время начала замера, time_metering - длительность замера
//Выводит количество подсчитаных за время time_metering
// в массив по указателю *frequency соответственно ножкам порта (*frequency[0]=PINx0, *frequency[0]=PINx1,...)
void freq(int* frequency[], unsigned int time_metering)
{
	g_seconds = 0;
	TCNT1 = 0;
	int prev[4]={1,};
	int real[4]={1,};
	int i=0;
	char reg_while=0;
	while (g_seconds < time_metering)
	{
		if (reg_while == 0)
		{
			for (i=0;i<=3;i++)
			{
				*frequency[i]=0;
			}
			reg_while=1;
		}
		
		//PIN1
		real[0] = (PORT1_freq & (1<<(PIN1_freq)));
		if (real[0] != prev[0])
		{
			*frequency[0]=*frequency[0]+1;
		}
		prev[0] = real[0];
		
		//PIN2
		real[1] = (PORT2_freq & (1<<(PIN2_freq)));
		if (real[1] != prev[1])
		{
			*frequency[1]=*frequency[1]+1;
		}
		prev[1] = real[1];
		
		//PIN3
		real[2] = (PORT3_freq & (1<<(PIN3_freq)));
		if (real[2] != prev[2])
		{
			*frequency[2]=*frequency[2]+1;
		}
		prev[2] = real[2];
		
		//PIN4
		real[3] = (PORT4_freq & ((1<<PIN4_freq)));
		if (real[3] != prev[3])
		{
			*frequency[3]=*frequency[3]+1;
		}
		prev[3] = real[3];
		
	}
	if (reg_while==1)
	{
		for (i=0;i<=3;i++)
		{
			*frequency[i]=(int)((*frequency[i])/2);
		}
	}
	
}