
#gcc -I/usr/share/R/include/ sload.c -lbiosig
 
CFLAGS += -I/usr/share/R/include
LFLAGS += -lR -lbiosig -lbiosig2

sload.so: sload.c
	R CMD SHLIB sload.c -lbiosig -lbiosig2

sload.o: sload.c
	gcc $(CFLAGS) -c "$<" -o "$@"


