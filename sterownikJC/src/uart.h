#include <avr/io.h>

extern void uart_init ();

extern void uart_put (char c);

extern char uart_get ();

extern uint8_t uart_get_hex ();

extern void uart_puts (const char *str);
