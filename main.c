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


void TimerInit()
{
	TCCR0 = 0x07;
	TCNT0 = 0;
	TIMSK |= (1<<TOIE0);
	sei();
}

void UARTInit()
{
	DDRD |= (1<<PORTD3);
	int speed = 52;
	UCSR0A = 0;
	UCSR0B = 0x98;
	UCSR0C = 0x06;
	UBRR0H = (unsigned char)(speed >> 8);
	UBRR0L = (unsigned char)(speed);
}

void TransmitAD7705 (unsigned char byte)
{
	PORTB &= ~(1 << PORTB0);
	SPDR = byte;
	while(!(SPSR & 0x80));
	PORTB |= (1 << PORTB0);
}
unsigned int ReceiveAD7705 ()
{
	unsigned int data;
	while (PORTD & 0x01);
	TransmitAD7705(0x38);
	while (PORTD & 0x01);
	PORTB &= ~(1 << PORTB0);
	SPDR = 0;
	while(!(SPSR & 0x80));
	data = SPDR;
	data <<= 8;
	SPDR = 0;
	while (!(SPSR & 0x80));
	data |= SPDR;
	PORTB |= (1 << PORTB0);
	return data;
}

void SPIInit()
{
	DDRB |= (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB0);
	PORTB |= (1<<PORTB0) | (1<<PORTB1);
	SPCR = 0x7F;
	PORTB &= ~(1 << PORTB2);
	_delay_ms(10);
	PORTB |= (1 << PORTB2);
	TransmitAD7705(0x20);
	TransmitAD7705(0x0C);
	TransmitAD7705(0x10);
	TransmitAD7705(0x40);
	while (!(PORTD & 0x01));
}

void PWMInit()
{
	DDRB |= (1<<PORTB5);
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

