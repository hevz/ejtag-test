/* start.S
 * Heiher <r@hev.cc>
 */

#define CORE_BUF_BASE	0x900000003ff01000

	.set	mips64r2
	.set	noreorder

	.text

	.align	10
	.global	_bios_1k
	.ent	_bios_1k
	.type	_bios_1k, @function
_bios_1k:
	.set	push
	.set	noreorder
	.set	noat
	.space	0x80
	daddiu	$sp, $sp, -240
	sd	$at, 0x00($sp)
	sd	$v0, 0x08($sp)
	sd	$v1, 0x10($sp)
	sd	$a0, 0x18($sp)
	sd	$a1, 0x20($sp)
	sd	$a2, 0x28($sp)
	sd	$a3, 0x30($sp)
	sd	$a4, 0x38($sp)
	sd	$a5, 0x40($sp)
	sd	$a6, 0x48($sp)
	sd	$a7, 0x50($sp)
	sd	$t0, 0x58($sp)
	sd	$t1, 0x60($sp)
	sd	$t2, 0x68($sp)
	sd	$t3, 0x70($sp)
	sd	$s0, 0x78($sp)
	sd	$s1, 0x80($sp)
	sd	$s2, 0x88($sp)
	sd	$s3, 0x90($sp)
	sd	$s4, 0x98($sp)
	sd	$s5, 0xa0($sp)
	sd	$s6, 0xa8($sp)
	sd	$s7, 0xb0($sp)
	sd	$t8, 0xb8($sp)
	sd	$t9, 0xc0($sp)
	sd	$gp, 0xc8($sp)
	sd	$s8, 0xd0($sp)
	sd	$ra, 0xd8($sp)
	mfc0	$t0, $23
	sd	$t0, 0xe0($sp)
	dmfc0	$t0, $24
	sd	$t0, 0xe8($sp)

	dla	$t9, ejtag_handler
	jalr	$t9
	move	$a0, $sp

	ld	$at, 0x00($sp)
	ld	$v0, 0x08($sp)
	ld	$v1, 0x10($sp)
	ld	$a0, 0x18($sp)
	ld	$a1, 0x20($sp)
	ld	$a2, 0x28($sp)
	ld	$a3, 0x30($sp)
	ld	$a4, 0x38($sp)
	ld	$a5, 0x40($sp)
	ld	$a6, 0x48($sp)
	ld	$a7, 0x50($sp)
	ld	$t0, 0x58($sp)
	ld	$t1, 0x60($sp)
	ld	$t2, 0x68($sp)
	ld	$t3, 0x70($sp)
	ld	$s0, 0x78($sp)
	ld	$s1, 0x80($sp)
	ld	$s2, 0x88($sp)
	ld	$s3, 0x90($sp)
	ld	$s4, 0x98($sp)
	ld	$s5, 0xa0($sp)
	ld	$s6, 0xa8($sp)
	ld	$s7, 0xb0($sp)
	ld	$t8, 0xb8($sp)
	ld	$t9, 0xc0($sp)
	ld	$gp, 0xc8($sp)
	ld	$s8, 0xd0($sp)
	ld	$ra, 0xd8($sp)
	ld	$t0, 0xe8($sp)
	dmtc0	$t0, $24
	daddiu	$sp, $sp, 240
	sync
	deret
	.set	pop
	.end	_bios_1k

	.align	2
	.global	ejtag_trigger
	.ent	ejtag_trigger
	.type	ejtag_trigger, @function
ejtag_trigger:
	.set	push
	.set	noreorder
	sdbbp
	jr	$ra
	nop
	.set	pop
	.end	ejtag_trigger

	.align	2
	.global	_start
	.ent	_start
	.type	_start, @function
_start:
	mfc0	$t0, $12
	li	$t1, 7
	ins	$t0, $t1, 28, 3
	ori	$t0, 0x80
	mtc0	$t0, $12

	dla	$sp, _sp0
	dli	$t0, CORE_BUF_BASE
	sd	$zero, 0x38($t0)
	sync

	# core 1
	dli	$t0, CORE_BUF_BASE + 0x100
	dla	$t1, _sp1
	sd	$t1, 0x28($t0)
	sync
	dla	$t1, _slave_start
	sd	$t1, 0x20($t0)
	# core 2
	dli	$t0, CORE_BUF_BASE + 0x200
	dla	$t1, _sp2
	sd	$t1, 0x28($t0)
	sync
	dla	$t1, _slave_start
	sd	$t1, 0x20($t0)
	# core 3
	dli	$t0, CORE_BUF_BASE + 0x300
	dla	$t1, _sp3
	sd	$t1, 0x28($t0)
	sync
	dla	$t1, _slave_start
	sd	$t1, 0x20($t0)

	jal	master_main
	nop

	b	_start
	nop
	.end	_start

	.global	_slave_start
	.ent	_slave_start
	.type	_slave_start, @function
_slave_start:
	mfc0	$t0, $12
	li	$t1, 7
	ins	$t0, $t1, 28, 3
	ori	$t0, 0x80
	mtc0	$t0, $12

	mfc0	$t0, $15, 1
	andi	$t0, 0x3ff
	andi	$s0, $t0, 0x3	# local cpuid

	jal	slave_main
	move	$a0, $s0

_slave_wait:
	dli	$t0, CORE_BUF_BASE
	sll	$s0, 8
	or	$t0, $s0

	sd	$zero, 0x20($t0)
1:
	ld	$t9, 0x20($t0)
	beqz	$t9, 1b
	nop
	ld	$sp, 0x28($t0)
	jalr	$t9
	nop
	.end	_slave_start

	.data

	.align 3
	.space	0x10000
_sp0:
	.space	0x10000
_sp1:
	.space	0x10000
_sp2:
	.space	0x10000
_sp3:
