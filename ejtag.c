/* ejtag.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"
#include "ejtag.h"
#include "string.h"

static void ejtag_bios_setup (void);
extern void ejtag_trigger (int cmd, ...);

static unsigned int ibcn;
static unsigned int dbcn;

unsigned int
ejtag_ibcn (void)
{
	return ibcn;
}

unsigned int
ejtag_dbcn (void)
{
	return dbcn;
}

void ejtag_init (void)
{
	ejtag_bios_setup ();
	ejtag_trigger (0);
}

void
ejtag_write_ib (unsigned int idx, unsigned long addr, unsigned long mask)
{
	ejtag_trigger (1, idx, addr, mask);
}

void
ejtag_write_db (unsigned int idx, unsigned long addr, unsigned long mask)
{
	ejtag_trigger (2, idx, addr, mask);
}

static void
ejtag_bios_setup (void)
{
	extern void _bios_1k(void);
	unsigned int phys_addr;
	int i;

	phys_addr = (unsigned long)&_bios_1k & 0x1fffffffUL;
	memcpy ((void *)0xffffffffa0000000UL + phys_addr, &_bios_1k, 1024);

	for (i = 0; i < 4; i++) {
		int j;

		for (j = 0; j < 8; j++) {
			void *addr = (void *)(0x900000003ff02000UL + (i << 8) + (j << 3));
			volatile unsigned long *bar_base = addr;
			volatile unsigned long *bar_mask = addr + 0x40;
			volatile unsigned long *bar_mmap = addr + 0x80;

			if ((*bar_mmap & 0x80) == 0) {
				*bar_base = 0x1fc00400UL;
				(void) *bar_base;
				*bar_mask = 0xfffffc00UL;
				(void) *bar_mask;
				*bar_mmap = phys_addr + 0xb0;
				(void) *bar_mmap;
				break;
			}
		}
	}
}

void
ejtag_handler (struct regs *regs)
{
	puts ("EJTAG HANDLER\r\n");

	if (regs->c0_dbg & 2) { /* SDBBP */
		switch (regs->a0) {
		case 0: /* Init */
			ibcn = (ejtag_read_ibs () >> 24) & 0xf;
			dbcn = (ejtag_read_dbs () >> 24) & 0xf;
			break;
		case 1: /* Write IBC */
			if (regs->a1 < ibcn) {
				ejtag_write_ibs (ejtag_read_ibs () & ~0x7ffful);
				ejtag_write_iba (regs->a1, regs->a2);
				ejtag_write_ibm (regs->a1, regs->a3);
				ejtag_write_ibc (regs->a1, 1);
			}
			break;
		case 2: /* Write DBC */
			if (regs->a1 < dbcn) {
				ejtag_write_dbs (ejtag_read_dbs () & ~0x7ffful);
				unsigned int dbc = 0x3c3ff1u;

				ejtag_write_dba(regs->a1, regs->a2);
				ejtag_write_dbm(regs->a1, regs->a3);
				ejtag_write_dbc (regs->a1, 0x3c0ff1);
			}
			break;
		default:
			puts ("EJTAG UNKNOWN CMD!\r\n");
			break;
		}
		regs->c0_depc +=4;
	} else { /* Not SDBBP */
		int i;

		puts ("EJTAG BREAKPOINT:\r\n");
		puts ("IB:\r\n");
		for (i = 0; i < ibcn; i++) {
			char buf[32];

			ultostr (buf, i);
			puts (buf);
			puts (": ");
			ultostr (buf, ejtag_read_iba (i));
			puts (buf);
			puts (" ");
			ultostr (buf, ejtag_read_ibm (i));
			puts (buf);
			puts (" ");
			if (ejtag_read_ibs () & (1u << i))
				puts ("1");
			else
				puts ("0");
			puts ("\r\n");
		}
		puts ("DB:\r\n");
		for (i = 0; i < dbcn; i++) {
			char buf[32];

			ultostr (buf, i);
			puts (buf);
			puts (": ");
			ultostr (buf, ejtag_read_dba (i));
			puts (buf);
			puts (" ");
			ultostr (buf, ejtag_read_dbm (i));
			puts (buf);
			puts (" ");
			if (ejtag_read_dbs () & (1u << i))
				puts ("1");
			else
				puts ("0");
			puts ("\r\n");
		}
		regs->c0_depc +=4;
	}
}
