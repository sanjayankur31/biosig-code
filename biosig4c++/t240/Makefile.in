-include modules.mk

# This file may be used as an input for make(3)
# Remove the lines below to convert it into a pure .am file
WIN32  = ../win32/libcnsfef.a ../win32/libcnsfef.dll
WIN64  = ../win64/libcnsfef.a ../win64/libcnsfef.dll
CFLAGS += -pipe -fPIC -Wall -O2 -Wextra  -I.
OBJS=${ASN_MODULE_SOURCES:.c=.o}
WINOBJS=${ASN_MODULE_SOURCES:.c=.obj}
W64OBJS=${ASN_MODULE_SOURCES:.c=.o64}

ifeq (Darwin,$(shell uname))
	CFLAGS        += -mmacosx-version-min=10.7
endif

CC ?= gcc
MinGWCC   = $(CROSS)-gcc 
MinGW64CC = $(CROSS64)-gcc 

all: libcnsfef.a $(WIN32)

libcnsfef.a: $(OBJS) 
	$(AR) libcnsfef.a $(OBJS) 

../win32/libcnsfef.a: $(WINOBJS) 
	$(CROSS)-$(AR) ../win32/libcnsfef.a $(WINOBJS) 

../win64/libcnsfef.a: $(W64OBJS) 
	$(CROSS64)-$(AR) ../win64/libcnsfef.a $(W64OBJS) 

../win32/libcnsfef.dll: $(WINOBJS)
	$(MinGWCC) -shared -fPIC  $(WINOBJS)  -Wl,--output-def,../win32/libcnsfef.def,--out-implib,../win32/libcnsfef.dll

../win64/libcnsfef.dll: $(WINOBJS)
	$(MinGW64CC) -shared -fPIC  $(W64OBJS)  -Wl,--output-def,../win64/libcnsfef.def,--out-implib,../win64/libcnsfef.dll


.SUFFIXES:
.SUFFIXES: .c .o

%.obj: %.c
	$(CROSS)-$(CC) $(CFLAGS) -o "$@" -c "$<"
%.o64: %.c
	$(CROSS64)-$(CC) $(CFLAGS) -o "$@" -c "$<"

.c.o:
	$(CC) $(CFLAGS) -o "$@" -c "$<"

clean:
#	rm -f $(ASN_MODULE_HEADERS)
#	rm -f $(ASN_MODULE_SOURCES)
	rm -f libcnsfef.a $(WIN32)
	rm -f $(OBJS)
	rm -f $(WINOBJS)
	rm -f $(W64OBJS)
	rm -f Makefile.am.sample libfef.a 

regen: regenerate-from-asn1-source 

regenerate-from-asn1-source:
	asn1c -fcompound-names fef.asn
	awk '/^#/ {exit} {print}' Makefile.am.sample >modules.mk
	rm -f converter-sample.c Makefile.am.sample libfef.a



