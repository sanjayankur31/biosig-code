####### Makefile to compile sload.exe for loading data supported by libbiosig into Mathematica
###
###  Copyright (C) 2010,2011,2012,2013,2015,2016 Alois Schloegl <alois.schloegl@gmail.com>
###  This file is part of the "BioSig for C/C++" repository
###  (biosig4c++) at http://biosig.sf.net/
###
##############################################################


###########################################
# user-defined variables 
###########################################
CROSS   ?= /home/schloegl/src/mxe.github.schloegl/usr/bin/i686-w64-mingw32.static
CROSS64 ?= /home/schloegl/src/mxe.github.schloegl/usr/bin/x86_64-w64-mingw32.static

###########################################
## set Mathematica variables

#MLINKDIR = /usr/local/Wolfram/gridMathematicaServer/7.0
#MLINKDIR = /usr/local/Wolfram/Mathematica/8.0
#MLINKDIR = /usr/local/Wolfram/Mathematica/9.0
#MLINKDIR = /usr/local/Wolfram/Mathematica/10.0

ifeq (Darwin,$(shell uname))
MLINKDIR := /Applications/Mathematica.app/Contents/
SYS       = MacOSX-x86-64
LDFLAGS  += -demangle -dynamic -arch x86_64
LDFLAGS  += -framework CoreFoundation -framework CoreVideo -framework VideoDecodeAcceleration
else
MLINKDIR := $(dir $(shell readlink -f $(shell which mathematica) ) )..
SYS 	  = $(notdir $(shell ls -1d $(MLINKDIR)/SystemFiles/Links/MathLink/DeveloperKit/*))
ifneq (1,$(words ${SYS}))
SYS       = Linux-x86-64# Set this value with the result of evaluating $SystemID
endif
endif

# automated detection of most recent MLINTERFACE, without starting mathematica
# 10.0 supports 3 and 4, 9.0 and earlier supports only 3,
MLIVER := $(shell ${CC} mliver.c -I${MLINKDIR}/SystemFiles/Links/MathLink/DeveloperKit/${SYS}/CompilerAdditions/ -E -o - | tail -1)

#
# Windows
#MLINKDIR  = /usr/local/Wolfram/gridMathematicaServer/7.0/SystemFiles/Links/MathLink/DeveloperKit
SYSwin       = Windows# Set this value with the result of evaluating $SystemID
MLLIBwin     = ml32i3m# Set this to ML64i3 if using a 64-bit system
SYSwin64     = Windows-x86-64# Set this value with the result of evaluating $SystemID
MLLIBwin64   = ml64i3m# Set this to ML64i3 if using a 64-bit system
###########################################

CADDSDIR  = ${MLINKDIR}/SystemFiles/Links/MathLink/DeveloperKit/${SYS}/CompilerAdditions
INCDIR    = -I${CADDSDIR} -I${MLINKDIR}/SystemFiles/IncludeFiles/C/
LIBDIR   += -L${CADDSDIR}

EXTRALIBS += -lm -ldl -luuid -lstdc++ # Set these with appropriate libs for your system.

ifeq (Darwin,$(shell uname))
MLLIB   = MLi${MLIVER} # ML32i3 # Set this to ML64i3 if using a 64-bit system
SED	= gsed
else
MLLIB   = ML$(shell getconf LONG_BIT)i${MLIVER} # ML32i3 # Set this to ML64i3 if using a 64-bit system
EXTRALIBS += -lrt
SED	= sed
endif
LD 	= ${CC}


ifneq (${MLIVER},)
CFLAGS += -DMLINTERFACE=${MLIVER}
endif

MPREP     = ${CADDSDIR}/mprep

TARGET = sload

all : ${TARGET}.exe ${SYSwin}/${TARGET}.exe ${SYSwin64}/${TARGET}.exe
mma : ${TARGET}.exe

.c.o:
	${CC} ${CFLAGS} $(shell pkg-config --cflags libbiosig) -c ${INCDIR} "$<"

${TARGET}tm.c: ${TARGET}.tm
	${MPREP} "$<" -o "$@"
	# enable macro expansion
	${SED} -i 's# __DATE__# ",__DATE__," ",__TIME__,"#' "$@"

${TARGET}.exe: ${TARGET}.o ${TARGET}tm.o
	mkdir -p ${SYS}
	${LD} ${LDFLAGS} ${TARGET}tm.o ${TARGET}.o ${LIBDIR} -l${MLLIB} ${EXTRALIBS} -lbiosig $(shell pkg-config --libs libbiosig) -o ${SYS}/"$@"
	ln -sf ${SYS}/"$@"


###########################################
#   Win32 - cross-compilation 
###########################################

CCw32        = $(CROSS)-gcc
CADDSDIRwin  = ${MLINKDIR}/SystemFiles/Links/MathLink/DeveloperKit/${SYSwin}/CompilerAdditions/mldev32
INCDIRwin    = -I${CADDSDIRwin}/include  -I${MLINKDIR}../SystemFiles/IncludeFiles/C/
LIBDIRwin    = ${CADDSDIRwin}/lib

EXTRALIBSwin = -lgdi32 -lm -lstdc++ -lws2_32 $(LDLIBS) # Set these with appropriate libs for your system.

MPREPwin     = ${CADDSDIRwin}/bin/mprep.exe

${TARGET}tm.win32.c : ${TARGET}.tm
	WINEPREFIX=${HOME}/.wine32.mma wine ${MPREPwin} "$<" -o "$@"
	sed -i 's# __DATE__# ",__DATE__," ",__TIME__,"#' "$@"

${TARGET}.obj : ${TARGET}.c
	${CCw32} -c ${INCDIRwin} "$<" -o "$@"
${TARGET}tm.obj : ${TARGET}tm.win32.c
	${CCw32} -c ${INCDIRwin} "$<" -o "$@"

win32mma : ${SYSwin}/${TARGET}.exe

${SYSwin}/${TARGET}.exe : ${TARGET}.obj ${TARGET}tm.obj
	mkdir -p ${SYSwin}
	${CCw32} $^ -mwindows -static-libgcc -static-libstdc++ -lbiosig -lssp -L${LIBDIRwin} -l${MLLIBwin} ${EXTRALIBSwin} -o "$@"
	# copy to users directory if available
	-cp -r ${SYSwin} ~/K/Software/MathematicaTools/${TARGET}


###########################################
#   Win64 - cross-compilation 
###########################################

CCw64         = $(CROSS64)-gcc
CADDSDIRwin64 = ${MLINKDIR}/SystemFiles/Links/MathLink/DeveloperKit/${SYSwin64}/CompilerAdditions/mldev64
INCDIRwin64   = -I${CADDSDIRwin64}/include -I${MLINKDIR}../SystemFiles/IncludeFiles/C/
LIBDIRwin64   = ${CADDSDIRwin64}/lib

MPREPwin64    = ${CADDSDIRwin64}/bin/mprep.exe

${TARGET}tm.win64.c : ${TARGET}.tm
	WINEPREFIX=${HOME}/.wine64.mma wine64 ${MPREPwin64} "$<" -o "$@"
	#${MPREP} "$<" -o "$@"
	sed -i 's# __DATE__# ",__DATE__," ",__TIME__,"#' "$@"

${TARGET}.w64obj : ${TARGET}.c
	${CCw64} -c ${INCDIRwin64} "$<" -o "$@"
${TARGET}tm.w64obj : ${TARGET}tm.win64.c
	${CCw64} -c ${INCDIRwin64} "$<" -o "$@"

win64mma : ${SYSwin64}/${TARGET}.exe

### curentlly the 64bit MXE cross-compilation does not work - use instead use 32bit version i
#${SYSwin64}/${TARGET}.exe : ${TARGET}.w64obj ${TARGET}tm.w64obj
${SYSwin64}/${TARGET}.exe : ${SYSwin}/${TARGET}.exe
	mkdir -p ${SYSwin64}
#	${CCw64} $^ -static-libgcc -static-libstdc++ -lbiosig -lssp -L${LIBDIRwin64} -l${MLLIBwin64} ${EXTRALIBSwin} -o "$@"
	-cp "$<" "$@"


###########################################
clean:
	-$(RM) *.o
	-$(RM) *.obj
	-$(RM) *tm.c
	-$(RM) ${SYSwin64}/${TARGET}.exe ${SYSwin32}/${TARGET}.exe
	-$(RM) ${SYS}/${TARGET} ${SYS}/${TARGET}.exe
	
