/* regs.h
 * Heiher <r@hev.cc>
 */

#ifndef __REGS_H__
#define __REGS_H__

struct regs
{
	unsigned long at;
	unsigned long v0;
	unsigned long v1;
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
	unsigned long a4;
	unsigned long a5;
	unsigned long a6;
	unsigned long a7;
	unsigned long t0;
	unsigned long t1;
	unsigned long t2;
	unsigned long t3;
	unsigned long s0;
	unsigned long s1;
	unsigned long s2;
	unsigned long s3;
	unsigned long s4;
	unsigned long s5;
	unsigned long s6;
	unsigned long s7;
	unsigned long t8;
	unsigned long t9;
	unsigned long gp;
	unsigned long s8;
	unsigned long ra;
	unsigned long c0_dbg;
	unsigned long c0_depc;
};

#endif /* __REGS_H__ */
