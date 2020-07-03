/* ejtag.h
 * Heiher <r@hev.cc>
 */

#ifndef __EJTAG_H__
#define __EJTAG_H__

#include "regs.h"

#define DRSEG_BASE	(0xffffffffff300000ul)

void ejtag_init (void);
unsigned int ejtag_ibcn(void);
unsigned int ejtag_dbcn(void);
void ejtag_write_ib (unsigned int idx, unsigned long addr, unsigned long mask);
void ejtag_write_db (unsigned int idx, unsigned long addr, unsigned long mask);

static inline unsigned long
ejtag_read_ibs (void)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x1000ul);
}

static inline void
ejtag_write_ibs (unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x1000ul) = v;
}

static inline unsigned long
ejtag_read_iba (unsigned int i)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x1100ul + 0x100 * i);
}

static inline void
ejtag_write_iba (unsigned int i, unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x1100ul + 0x100 * i) = v;
}

static inline unsigned long
ejtag_read_ibm (unsigned int i)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x1108ul + 0x100 * i);
}

static inline void
ejtag_write_ibm (unsigned int i, unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x1108ul + 0x100 * i) = v;
}

static inline unsigned long
ejtag_read_ibc (unsigned int i)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x1118ul + 0x100 * i);
}

static inline void
ejtag_write_ibc (unsigned int i, unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x1118ul + 0x100 * i) = v;
}

static inline unsigned long
ejtag_read_dbs (void)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x2000ul);
}

static inline void
ejtag_write_dbs (unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x2000ul) = v;
}

static inline unsigned long
ejtag_read_dba (unsigned int i)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x2100ul + 0x100 * i);
}

static inline void
ejtag_write_dba (unsigned int i, unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x2100ul + 0x100 * i) = v;
}

static inline unsigned long
ejtag_read_dbm (unsigned int i)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x2108ul + 0x100 * i);
}

static inline void
ejtag_write_dbm (unsigned int i, unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x2108ul + 0x100 * i) = v;
}

static inline unsigned long
ejtag_read_dbc (unsigned int i)
{
	return *(volatile unsigned long *)(DRSEG_BASE + 0x2118ul + 0x100 * i);
}

static inline void
ejtag_write_dbc (unsigned int i, unsigned long v)
{
	*(volatile unsigned long *)(DRSEG_BASE + 0x2118ul + 0x100 * i) = v;
}

#endif /* __EJTAG_H__ */
