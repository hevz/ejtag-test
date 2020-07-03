/* main.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"
#include "ejtag.h"
#include "string.h"

static volatile int sync;

static void
ejtag_db_test (unsigned int id)
{
	char buf[32];
	unsigned long tmp;
	unsigned long val;

	puts ("==== CPU ");
	ultostr (buf, id);
	puts (buf);
	puts (":\r\n");

	puts ("WRITE DB\r\n");
	ejtag_write_db (0, (unsigned long) &val, 3);

	puts ("ACCESS MEM\r\n");
	asm volatile (
		".set push \n\t"
		".set noreorder \n\t"
		"nop \n\t"
		"nop \n\t"
		"ld %[t], 0(%[v])\n\t"
		"sd %[t], 0(%[v])\n\t"
		"nop \n\t"
		"nop \n\t"
		".set pop \n\t"
		:[t]"+r"(tmp)
		:[v]"r"(&val)
		:"memory"
	);
}

void
master_main (void)
{
	char buf[32];

	puts ("INIT\r\n");
	ejtag_init ();

	puts ("EJTAG IBCN: ");
	ultostr (buf, ejtag_ibcn ());
	puts (buf);
	puts ("\r\n");

	puts ("EJTAG DBCN: ");
	ultostr (buf, ejtag_dbcn ());
	puts (buf);
	puts ("\r\n");

	ejtag_db_test (0);

	sync++;
	asm volatile ("sync":::"memory");
	while (1);
}

void
slave_main (unsigned int id)
{
	while (sync != id)
		asm volatile ("sync":::"memory");

	ejtag_db_test (id);

	sync++;
	asm volatile ("sync":::"memory");
	while (1);
}
