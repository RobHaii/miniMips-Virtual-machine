C_COMPILER=gcc

all: vm

vm: main.o vm.o ../common-lib/common-utils.o
	$(C_COMPILER) main.o ../common-lib/common-utils.o vm.o -o vm

../common-lib/common-utils.o: ../common-lib/common-utils.c
	$(C_COMPILER) -c ../common-lib/common-utils.c
	mv common-utils.o ../common-lib/

main.o: main.c
	$(C_COMPILER) -c main.c

vm.o: vm.c
	$(C_COMPILER) -c vm.c

clean:
	rm -f *.o vm *.bin

