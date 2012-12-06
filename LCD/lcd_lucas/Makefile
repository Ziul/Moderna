
all:
	msp430-gcc *.c -c -lc -lfp -lm -O3 -L/usr/msp430/lib -Os
	msp430-ar rcs libl2f50.a *.o
	rm *.o
	
clean:
	rm *.o *.a	
