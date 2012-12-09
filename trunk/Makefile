#INCLUDES = -I../LCD/lcd_$(USER) -I./
INCLUDES = 	-I../LCD/lcd_lucas \
			-I./ \
			-L/usr/msp430/lib \
			-L../LCD/ \
			-L./
			
SRC =	adc.c \
		ploc.c \
		uart.c

LCD = l2f50
MCU = msp430g2553
#MCU = msp430g2452

CC = msp430-gcc
OD = msp430-objdump
FLAGS= -mmcu=$(MCU) -lc -lfp -lm -O3 -Os -l$(LCD) -lfast -g

TARGET=prog
ELF = $(TARGET).elf

all: libs
#	$(CC) -c *.c $(INCLUDES) $(FLAGS)
	$(CC) -c $(SRC) $(INCLUDES) $(FLAGS)
	msp430-ar rcs libfast.a *.o
	$(CC) $(INCLUDES) main.c -o $(ELF) -Wl,-Map=./_$(TARGET).map $(FLAGS) 
	$(OD) -S -g ./$(ELF) > ./_$(TARGET).list
	rm *.o
	@size $(ELF) -Adt

libs:
	cd ../LCD/lcd_lucas ; $(MAKE) ; cp lib$(LCD).a ../
	
clean:
	rm *.o *.list *.map *.elf
	cd ../LCD/lcd_lucas ; $(MAKE) clean
	
