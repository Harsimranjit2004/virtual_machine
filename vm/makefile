CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS=
all: vasm vmi
vasm: vbasm.c vm.c
	$(CC) $(CFLAGS) -o vasm vbasm.c $(LIBS)

vmi: vmi.c vm.c
	$(CC) $(CFLAGS) -o vmi vmi.c $(LIBS)