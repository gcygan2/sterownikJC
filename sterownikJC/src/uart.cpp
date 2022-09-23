#include "uart.h"

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif


#define USART_BAUDRATE 19200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 

void uart_init ()
{
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	UBRR0 = BAUD_PRESCALE;
}

void uart_put (char c)
{
	while ( !( UCSR0A & (1<<UDRE0)) ); 
	UDR0 = c;	
}

char uart_get ()
{
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

uint8_t uart_get_hex ()
{
	char c;
	uint8_t b;

	c = uart_get();
	uart_put (c);
	b = (c - '0') << 4;
	c = uart_get();
	uart_put (c);
	b |= c - '0';
	return b;
}

void uart_puts (const char *str)
{
	while (*str) uart_put (*str++);
}
