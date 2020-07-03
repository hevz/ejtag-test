/* uart.h
 * Heiher <r@hev.cc>
 */

#ifndef __UART_H__
#define __UART_H__

#define UART_TX                 0    /* Out: Transmit buffer */
#define UART_LSR                5    /* In:  Line Status Register */
#define UART_LSR_THRE           0x20 /* Transmit-hold-register empty */

#define PORT(base, offset) (volatile unsigned char *)(base + offset)

static inline unsigned int
serial_in (unsigned char *base, int offset)
{
	return *(PORT(base, offset));
}

static inline void
serial_out (unsigned char *base, int offset, int value)
{
	*PORT(base, offset) = value;
}

void prom_putchar (char c);
int puts (const char *s);

#define putchar	prom_putchar

#endif /* __UART_H__ */
