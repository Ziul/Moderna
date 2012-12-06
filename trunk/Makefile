#INCLUDES = -I../LCD/lcd_$(USER) -I./
INCLUDES = 	-I../LCD/lcd_lucas \
			-I./ \
			-L/usr/msp430/lib \
			-L../LCD/

LCD = l2f50
MCU = msp430g2553

CC = msp430-gcc
OD = msp430-objdump
FLAGS= -mmcu=$(MCU) -lc -lfp -lm -O3 -l$(LCD)

TARGET=prog
ELF = $(TARGET).elf

all: libs
	$(CC) -c *.c $(INCLUDES) -mmcu=msp430g2553
	$(CC) *.o $(FLAGS) $(INCLUDES) -o $(ELF) -Wl,-Map=./_$(TARGET).map
	$(OD) -S -g ./$(ELF) > ./_$(TARGET).list

libs:
	cd ../LCD/lcd_lucas ; $(MAKE) ; cp lib$(LCD).a ../
	
clean:
	rm *.o *.list *.map *.elf
	cd ../LCD/lcd_lucas ; $(MAKE) clean
	
