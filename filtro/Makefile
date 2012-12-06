# Nome do alvo desejado 
TARGET=prog

# Arquivos de entrada
SRC= *.c

# Objetos
OBJ=$(SRC:.c=.o)

# Se for gerar um arquivo HEX
HEX=_$(TARGET).hex

# Nome do arquivo ELF
ELF=$(TARGET).elf

# Programa para copia de objeto
OBJCOPY=msp430-objcopy

# Programa para compilar/linkeditar
CC=msp430-gcc

# Flags de warning
WAR=-pedantic -Wall

# Demais Flags
FLAGS= -mmcu=msp430g2553 -L/usr/msp430/lib -lc -lfp -lm -O3
#	Escolha do dispositivo: -mmcu
#	Localização das libs: -L
#	Linkando com algumas outras bibliotecas já disponíveis (não obrigatório ate o momento): -lc -lfp -lm
#	Flag para otimizar o objeto: -O3

#Primeiro alvo
build: clean all

all:$(HEX)

#Gerar arquivo HEX
$(HEX):$(ELF)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)

#Gerando arquivo ELF
$(ELF): $(OBJ)
#	msp430-strip $(OBJ) -o $(OBJ)
	$(CC) $(OBJ) -Wl,-Map=_$(TARGET).map -o $(ELF) $(FLAGS) $(WAR) 
#	$(CC) $(OBJ) -o $(ELF) -lm
	
#Gerando arquivo assembly (não passa por este alvo)
assembly: $(SRC)
	$(CC) $(SRC) -Wl,-Map=_$(TARGET).map -o $(TARGET).s $(FLAGS) -S
	
#Gerando objetos
$(OBJ):$(SRC)
	$(CC) -Wall -c $(SRC) -L/usr/msp430/lib -lm -O3
	
#Limpando os objetos
clean:
	clear
	rm -rf *.o
	
#Limpando tudo
clean_all:
	clear
	rm -rf *.o *.elf *.hex *.map *.s
