CC=gcc
CFLAGS=-march=mips64r2 -mabi=64 -O2 -nostdlib -mno-abicalls -T ld.script

SOURCES=start.S main.c uart.c ejtag.c string.c

ejtag-test : $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm -f ejtag-test
