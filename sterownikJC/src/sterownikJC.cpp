#include <stdio.h>
#include "sterownikJC.h"
#include "uart.h"

#define ARD_PCF8583 0xd0

#define LED_PIN (1<<5)
#define LED_PORT PORTB
#define LED_DDR DDRB


char Sterownik::buf[24];

void Sterownik::set ()
{
	i2c_start(ARD_PCF8583);
	i2c_write(0);

	i2c_write(0);
	i2c_write(rtc_set_m);
	i2c_write(rtc_set_h);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);

	i2c_write(0);
	i2c_stop();	
}

void Sterownik::store()
{
	i2c_start(ARD_PCF8583);
	i2c_write(0x08);
	
	i2c_write(alarm);
	i2c_write(off_m);
	i2c_write(off_h);
	i2c_write(on_m);
	i2c_write(on_h);
	i2c_stop();
}

void Sterownik::check ()
{
	i2c_start(ARD_PCF8583);
	i2c_write(0);
	i2c_stop();
	i2c_start(ARD_PCF8583|1);
	rtc_s = i2c_read_ack();
	rtc_m = i2c_read_ack();
	rtc_h = i2c_read_nack();
	i2c_stop();

	if ((rtc_h == on_h) && (rtc_m == on_m)) {
		LED_PORT |= LED_PIN;
		alarm = 1;
		store();
	} else if ((rtc_h == off_h) && (rtc_m == off_m)) {
		LED_PORT &= ~LED_PIN;
		alarm = 0;
		store();
	}
}


void Sterownik::init ()
{
	LED_DDR |= LED_PIN;

	uart_init();
	i2c_init();

	i2c_start(ARD_PCF8583);
	i2c_write(0x08);
	i2c_start(ARD_PCF8583|1);
	alarm = i2c_read_ack();
	if (alarm) {
		LED_PORT |= LED_PIN;
	} else {
		LED_PORT &= ~LED_PIN;
	}
	off_m = i2c_read_ack();
	off_h = i2c_read_ack();
	on_m = i2c_read_ack();
	on_h = i2c_read_nack();
	i2c_stop();
}

void Sterownik::run() 
{
	while ( !(UCSR0A & (1<<RXC0)) ) {
		check();
	}
	cmd = UDR0;	
	if (cmd == '1') {
		uart_puts ("\n\rgodzina>");
		on_h = uart_get_hex ();
		uart_puts ("\n\rminuta>");
		on_m = uart_get_hex ();
		uart_puts ("\n\rOK>");
		store();
	} else if (cmd == '2') {
		uart_puts ("\n\rgodzina>");
		off_h = uart_get_hex ();
		uart_puts ("\n\rminuta>");
		off_m = uart_get_hex ();
		uart_puts ("\n\rOK>");
		store();
	} else if (cmd == '0') {
		uart_puts ("\n\rgodzina>");
		rtc_set_h = uart_get_hex ();
		uart_puts ("\n\rminuta>");
		rtc_set_m = uart_get_hex ();
		uart_puts ("\n\rOK>");
		set();
	} else {
		uart_puts ("\n\r1 - ustaw godz. zalaczenia\n\r2 - ustaw godz. wylaczenia\n\r0 - ustaw biezaca godzine");
		sprintf (buf, "\n\rRTC: %02x:%02x.%02x", rtc_h, rtc_m, rtc_s);
		uart_puts (buf);
		sprintf (buf, "\n\r ON: %02x:%02x", on_h, on_m);
		uart_puts (buf);
		sprintf (buf, "\n\rOFF: %02x:%02x\n\r>", off_h, off_m);
		uart_puts (buf);
	}
}
