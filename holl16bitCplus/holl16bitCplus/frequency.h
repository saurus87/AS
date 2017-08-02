/*������� ��������� �� �������� ����� � �������������� �������/�������� TC1*/

#pragma once

#include <avr/interrupt.h>

#define	PORT_Freq	PINC		//���� �����������
#define	PIN_Freq	1			//���� �����������

volatile unsigned int g_seconds = 0;

//��������� �� ������� ������ ���������� ���������� g_seconds
void setup_TC1() {
	TCCR1A = 0;				//��������� ������� 1, ������ �
	TCCR1B = 0x5;			//������������ CLK/1024;
	OCR1A = 0x3D09;			//���������� ��� � ������� �� ������� 16M��
	TIMSK1 = 0x2;			//������ ������� �� ���������� 1�
	sei();					//��������� ���������� (���������: cli(); )
}

//���������� ���������� �� ���������� 1�
//������� �������
ISR(TIMER1_COMPA_vect)
{
  if (g_seconds > 2) 
  {
    g_seconds = 0;
  }
  else 
  {
    g_seconds++;
  }  
  TCNT1 = 0;		//��������� �������� �������� TCNT1
}

//����� ��������� �� ����� PORT_Freq � ����� PIN_Freq � ��������
int freq(unsigned int period, unsigned int time_start, unsigned int time_metering)		//period - ������ ������ ��������� � ��������, time_start - ����� ������ ������, time_metering - ������������ ������
{
	int frequency=0;
	int prev=0, real=0;
	while ((g_seconds >= time_start ) &(g_seconds <= time_start + time_metering))
	{
		{
			real = (PORT_Freq & PIN_Freq);
			if (real != prev)
			{
				frequency++;
				//USART_Transmit('/');
				//Serial.println('*');
			}
			prev = real;
		}
	}
	return (int)frequency/2;
}

//���������� ����� ��������� �� ����� PORT_Freq � ����� PIN_Freq 
int freq(unsigned int time_metering)		 //time_metering - ������������ ������
{
	g_seconds = 0;
	TCNT1 = 0;
	int frequency=0;
	int prev=0, real=0;
	while (g_seconds < time_metering)
	{
		{
			real = (PORT_Freq & PIN_Freq);
			if (real != prev)
			{
				frequency++;
			}
			prev = real;
		}
	}
	return (int)frequency/2;
}