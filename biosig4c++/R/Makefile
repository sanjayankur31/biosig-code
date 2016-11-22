
#gcc -I/usr/share/R/include/ sload.c -lbiosig
 
CFLAGS += -I/usr/share/R/include
#LFLAGS += -lR -lbiosig -lbiosig2
LFLAGS += ../libbiosig.a ../libbiosig2.a

sload.so: sload.c
	R CMD SHLIB sload.c ${LFLAGS} ${LIBS}

sload.o: sload.c
	gcc $(CFLAGS) -c "$<" -o "$@"

clean:
	rm *.o *.so
