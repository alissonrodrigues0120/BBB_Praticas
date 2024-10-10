CHAINPATH=
CHAIN=$(CHAINPATH)arm-none-eabi
CFLAGS=-std=c99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8
AFLAGS=-mfpu=neon
IPATH=-Iinc/
SRC=src/
OBJ=obj/
BIN=bin/

all: app

app: start.o interrupt.o clock_module.o control_module.o gpio.o pad.o timer.o config.o main.o
	$(CHAIN)-ld $(OBJ)start.o $(OBJ)interrupt.o $(OBJ)main.o $(OBJ)timer.o $(OBJ)gpio.o $(OBJ)clock_module.o $(OBJ)control_module.o $(OBJ)pad.o $(OBJ)config.o  -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)spl.boot -O binary
	cp $(BIN)spl.boot /tftpboot/snake.bin

start.o: $(SRC)start.s
	$(CHAIN)-as $(AFLAGS) $(SRC)start.s -o $(OBJ)start.o

main.o: $(SRC)main.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)main.c -o $(OBJ)main.o

clock_module.o: $(SRC)clock_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)clock_module.c -o $(OBJ)clock_module.o

control_module.o: $(SRC)control_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)control_module.c -o $(OBJ)control_module.o

gpio.o: $(SRC)gpio.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)gpio.c -o $(OBJ)gpio.o

config.o: $(SRC)config.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)config.c -o $(OBJ)config.o

timer.o: $(SRC)timer.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)timer.c -o $(OBJ)timer.o



interrupt.o: $(SRC)interrupt.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)interrupt.c -o $(OBJ)interrupt.o

pad.o: $(SRC)pad.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)pad.c -o $(OBJ)pad.o





copy:
	cp $(BIN)spl.boot /tftpboot/snake.bin

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.boot

dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf
