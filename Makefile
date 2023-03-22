CC = gcc
CFLAGS = -Wall -std=c18 -ggdb
LDFLAGS = -lm

test : testFloatx
	./testFloatx 32 8 <test1.txt
	./testFloatx 16 4 <test1.txt
	./testFloatx 64 11 <test1.txt
	
gdb : testFloatx
	gdb -ex "break doubleToFloatx" -ex "run" --args ./testFloatx 16 4
	
testFloatx : testFloatx.c floatx.c floatx.h	
	$(CC) $(CFLAGS) -o testFloatx  testFloatx.c floatx.c ${LDFLAGS}
	
clean :
	-rm testFloatx
