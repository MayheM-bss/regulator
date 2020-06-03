/*
 * regulator.c
 *
 * Created: 28.05.2020 1:18:42
 * Author : Aleksey
 */ 
#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

const float dt = 0.035;
int reqTemp;

unsigned char USARTRecevied()
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

ISR (TIMER0_OVF_vect)
{
	cli();
	
	sei();
}

ISR (USART0_RX_vect)
{
	cli();
	unsigned char message[5];
	for (int i = 0; i < 5; i++)
	{
		message[i] = USARTRecevied();
	}
	for (int i = 0; i < 5; i++)
	{
		reqTemp = reqTemp + message[i];
	}
	sei();
}


void TimerInit()
{
	TCCR0 = 0x07;
	TCNT0 = 0;
	TIMSK |= (1<<TOIE0);
	sei();
}

void UARTInit()
{
	int speed = 52;
	UCSR0A = 0;
	UCSR0B = 0x98;
	UCSR0C = 0x06;
	UBRR0H = (unsigned char)(speed >> 8);
	UBRR0L = (unsigned char)(speed);
}

void SPIInit()
{
	
}

void PWMInit()
{
	TCCR1A = 0x83;
	TCCR1B = 0x09;
	TCNT1H = 0x00;
	TCNT1L = 0x00;
}

void USARTTransmitted(char str[])
{
	register char i = 0;
	while(str[i])
	{
		while(!( UCSR0A & (1<<UDRE0)))
		{
			UDR0 = str[i++];
		}
	}
	while(!( UCSR0A & (1<<UDRE0)))
	{
		UDR0 = ((char)13);
	}
	while(!( UCSR0A & (1<<UDRE0)))
	{
		UDR0 = ((char)10);
	}
}


void SetPWM()
{
	
}

int main(void)
{
	SPIInit();
	UARTInit();
	PWMInit();
	TimerInit();
	
    /* Replace with your application code */
    while (1) 
    {
	
    }
}

