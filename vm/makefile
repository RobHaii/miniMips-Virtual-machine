C_COMPILER=gcc

all: vm

vm: main.o vm.o ../common-lib/common-utils.o
	$(C_COMPILER) main.o ../common-lib/common-utils.o vm.o -o vm

main.o: main.c
	$(C_COMPILER) -c main.c

vm.o: vm.c
	$(C_COMPILER) -c vm.c

clean:
	rm -f *.o vm *.bin ../common-lib/*.o 

