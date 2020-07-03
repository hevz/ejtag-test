/* uart.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"

void
prom_putchar (char c)
{
	int timeout;
	unsigned char *uart_base;

	uart_base = (unsigned char *) 0xffffffffbfe001e0ul;
	timeout = 1024;

	while (((serial_in (uart_base, UART_LSR) & UART_LSR_THRE) == 0) &&
			(timeout-- > 0))
		;

	serial_out (uart_base, UART_TX, c);
}

int
puts (const char *s)
{
	while ('\0' != *s)
		putchar (*s ++);

	return 0;
}
