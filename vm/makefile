CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = src

all: vmi vbasm

vmi: $(SRC_DIR)/vmi.c $(SRC_DIR)/vm.c $(SRC_DIR)/vm.h
	$(CC) $(CFLAGS) -o vmi $(SRC_DIR)/vmi.c $(SRC_DIR)/vm.c

vbasm: $(SRC_DIR)/vbasm.c $(SRC_DIR)/vm.c $(SRC_DIR)/vm.h
	$(CC) $(CFLAGS) -o vbasm $(SRC_DIR)/vbasm.c $(SRC_DIR)/vm.c

# dbasm: $(SRC_DIR)/dbasm.c $(SRC_DIR)/vm.c $(SRC_DIR)/vm.h
# 	$(CC) $(CFLAGS) -o dbasm $(SRC_DIR)/dbasm.c $(SRC_DIR)/vm.c

clean:
	rm -f vmi vbasm dbasm fib.out
