####### Makefile for "BioSig for C/C++" #####################
###
###  Copyright (C) 2006-2015 Alois Schloegl <alois.schloegl@ist.ac.at>
###  This file is part of the "BioSig for C/C++" repository
###  (biosig4c++) at http://biosig.sf.net/
###
##############################################################

##### Target: GNU/Linux
#
## make && make install    build and install libbiosig, but no other tools.
#             libbiosig is a prerequisite for building all the other tools below.
#
## make save2gdf   - makes converter
## make install_save2gdf   - installs converter
#
## make mex4o      - makes mexSLOAD, mexSOPEN for Octave (requires Octave-headers)
## make mex4m      - makes mexSLOAD, mexSOPEN for Matlab (requires Matlab, mex -setup must be configured)
## make mexw32     - makes mexSLOAD.mexw32, mexSOPEN.mexw32 (requires that mingw32, gnumex libraries from Matlab/Win32)
## make mexw64     - makes mexSLOAD.mexw64, mexSOPEN.mexw64 (requires that mce-w32, gnumex libraries from Matlab/Win64)
## make mex        - mex4o, mex4m, mexw32, mexw64 combined
## make biosig4python - makes python interface (requires Python)
## make biosig4java - makes Java interface (experimental)
## make biosig4php - makes PHP interface (experimental)
## make biosig4perl - makes perl interface (experimental)
## make biosig4ruby - makes ruby interface (experimental)
## make biosig4tcl - makes tcl/tk interface (experimental)
##
## make win32 and make win64 are obsolete. save2gdf.exe, and libbiosig.{a,dll} for windows can now be built
## with the mingw-cross-compiler environment (mxe.cc).
##    git clone https://github.com/schloegl/mxe.git
##    make biosig
## should do what you want. Please note, that win32mma does now also rely that libbiosig is built with MXE.
## Make sure that CROSS or CROSS64 is properly defined when running `make`
##
## ???
## make sigviewer  - makes sigviewer

##### Target: Win32
## make win32      - makes save2gdf.exe,libbiosig.lib,libbiosig.dll, for MSWindows, requires MinGW
## make mexw32 mex/mexSLOAD.mexw32   - requires MinGW32 and GNUMEX libraries from Matlab/Win32
## make win32/sigviewer.exe 	- requires sources of SigViewer, and MinGW32 (mex: make suitesparse zlib qt )

##### Target: Win64
## make win64      - makes save2gdf.exe,libbiosig.lib,libbiosig.dll, for MSWindows, requires MinGW
## make mexw64 mex/mexSLOAD.mexw64   - requires MCE-W64 and GNUMEX libraries from Matlab/Win64
## make win64/sigviewer.exe 	- requires sources of SigViewer and MCE-W64, make suitesparse zlib qt 

##### Target: MacOSX w/ homebrew
## make install_homebrew	installs libbiosig and save2gdf
	# requires: brew tap homebrew/dupes
	#	    brew install libiconv

###############################
# whether dynamic or static linking is used, can be controlled with
# LIBEXT. Setting it to 'a' links statically, 'so' links dynamically
#
LIBEXT        = a
#LIBEXT	      = so
###############################


## TARGET dependencies
ifeq (,$(TARGET))
	CC      ?= gcc
	CXX     ?= g++
	AR      := ar rcs
	PKGCONF := pkg-config
ifneq (Darwin,$(shell uname))
	LFLAGS  += -Wl,-z,relro,-z,now
endif
else ifeq (intel,$(TARGET))
	CC      := /opt/intel/bin/icc
	CXX     := /opt/intel/bin/icc
	LD      := /opt/intel/bin/xild
	AR      := /opt/intel/bin/xiar crs
else 
	prefix  := $(dir $(shell which $(TARGET)-gcc))
	CC      := $(TARGET)-gcc
	CXX     := $(TARGET)-g++
	LD      := $(TARGET)-ld
	AR      := $(TARGET)-ar rcs
	PKGCONF := $(TARGET)-pkg-config
	ifeq (mingw, $(findstring mingw, $(TARGET)))
		## add gnulib's getlogin
		SOURCES += win32/getlogin.c win32/getline.c win32/getdelim.c
		OBJECTS += getlogin.o getline.o getdelim.o
		## exclude conflicting definition of sopen from mingw's io.h
		CFLAGS  += -I.
		IO_H_FILE2 = io.h
		CFLAGS  += -I$(prefix)/include
		LIBS    += -lssp
		LDLIBS  += -liconv -liberty -lws2_32
	endif
endif

ifeq (Darwin,$(shell uname))
	## Homebrew:
	##	brew tap homebrew/dupes
	##	brew install libiconv
	##
	LD	       = ld
	CFLAGS        += -I/usr/local/opt/libiconv/include
	LDLIBS        += -L/usr/local/opt/libiconv/lib/ -liconv -lstdc++
	LDFLAGS        += -dylib -arch x86_64 -macosx_version_min 10.9
	DLEXT          = dylib
	FULLDLEXT      = ${BIOSIG_VERSION}.dylib
	SONAME_PREFIX  = -install_name # the last space character is crucial
	SONAME_POSTFIX = .${MAJOR_VERSION}.${MINOR_VERSION}.$(DLEXT)
else
	LD	       = $(CXX)
	LDFLAGS       += -shared
	DLEXT          = so
	FULLDLEXT      = so.${BIOSIG_VERSION}
	SONAME_PREFIX  = -Wl,-soname=
	SONAME_POSTFIX = .$(DLEXT).${MAJOR_VERSION}.${MINOR_VERSION}
endif

ifeq (,$(prefix))
	prefix  := /usr/local
endif

DEFINES_ALL   = #-D=NDEBUG

DEFINES      += $(DEFINES_ALL)
#DEFINES      += -D=WITH_SON 
#DEFINES      += -D=WITHOUT_SCP_DECODE
#DEFINES      += -D=WITH_TIMESTAMPEVENT
#DEFINES      += -D=WITH_TIMESTAMPCHANNEL
#DEFINES      += -D=WITH_CURL
DEFINES      += -D=__4HAERTEL__
DEFINES      += -D=WITH_FAMOS
DEFINES      += -D=WITH_FIFF
DEFINES      += -D=WITHOUT_NETWORK
#DEFINES      += -D=WITH_HDF
#DEFINES      += -D=WITH_MATIO
#DEFINES      += -D=WITH_LIBXML2 
DEFINES      += -D=WITH_FEF
#DEFINES      += -D=WITH_PDP
#DEFINES      += -D=WITH_DCMTK
#DEFINES      += -D=WITH_DICOM
#DEFINES      += -D=WITH_GDCM
#DEFINES      += -D=WITH_GSL
#DEFINES      += -D=WITH_EEPROBE
#DEFINES      += -D=WITH_TDMS
#DEFINES      += -D=WITH_ATF
#DEFINES      += -D=WITH_AVI
#DEFINES      += -D=WITH_RIFF
#DEFINES      += -D=WITH_WAV
#DEFINES      += -D=WITH_NEV
DEFINES      += -D=MAKE_EDFLIB


ifneq (,$(findstring WITH_GDCM, $(DEFINES)))
  INCPATH    += -I$(wildcard /usr/include/gdcm-2*/)
  LDLIBS     += -lgdcmCommon -lgdcmDSED -lgdcmMEXD -lgdcmMSFF -lgdcmDICT -lgdcmIOD
endif
ifneq (,$(findstring WITH_LIBXML2, $(DEFINES)))
  LDLIBS     += -lxml2
endif
ifneq (,$(findstring WITH_CURL, $(DEFINES)))
  LDLIBS     += -lcurl
endif
ifneq (,$(findstring WITH_HDF, $(DEFINES)))
  LDLIBS     += -lhdf5
endif
ifneq (,$(findstring WITH_DCMTK, $(DEFINES)))
  LDLIBS     += -ldcmdata -loflog -lofstd
endif
ifneq (,$(findstring WITH_MATIO, $(DEFINES)))
  LDLIBS     += -lmatio
endif
ifneq (,$(findstring stack-protector, $(CFLAGS)))
ifneq (,$(findstring CYGWIN, $(shell uname)))
  LDLIBS     += -lssp -liconv
endif 
endif

LIBS         += $(LDLIBS)

DELETE        = rm -f
COPY          = cp -f
DATA_DIR      = data/
DATA_DIR_CFS  = $(HOME)/L/data/test/CFS/
TEMP_DIR      = test/
SED           = sed
VERBOSE	     := -V0

##########################################################
## set Matlab and Octave variables
ifeq (undefined,$(origin MATLABDIR))
  ifeq (Darwin,$(shell uname))
    MATLABDIR := $(shell find /Applications/MATLAB* -name bin -type d -depth 1 2>/dev/null | head -1)
  else
    MATLABDIR ?= $(shell find /usr/local/MATLAB* -maxdepth 2 -name bin -type d 2>/dev/null | head -1)
  endif
endif

ifeq (,$(MATLABDIR))
  #$(warning MATLABDIR is not defined)
else
  MEX         = $(MATLABDIR)/mex
  MEX_EXT    := $(shell $(MATLABDIR)/mexext)
endif


# use environment variable to define OCTAVE_VERSION
#   e.g. export OCTAVE_VERSION=-3.6.2
# Octave - global install  (e.g. from debian package)
#OCTAVE_VERSION = 
# Octave - local install (e.g. compiled from sources) 
#OCTAVE_VERSION = -3.6.1

OCT           := mkoctfile$(OCTAVE_VERSION)
##########################################################


first: mex4o 


#############################################################
#	Compilation: Implicit, default rules
#############################################################


#############################################################
#	MEX-files for Octave and Matlab
#############################################################

# include directory for Win32-Matlab include
W32MAT_INC = $(HOME)/bin/win32/Matlab/R2010b/extern/include/ -I../win32
W64MAT_INC = $(HOME)/bin/win64/Matlab/R2010b/extern/include/ -I../win64
# path to GNUMEX libraries, available from here http://sourceforge.net/projects/gnumex/
GNUMEX   = $(HOME)/bin/win32/gnumex
GNUMEX64 = $(HOME)/bin/win64/gnumex

mexSOPEN.cpp : mexSLOAD.cpp
	echo "#define mexSOPEN" > mexSOPEN.cpp
	cat mexSLOAD.cpp >> mexSOPEN.cpp

MEX_OBJECTS = mexSLOAD.cpp mexSOPEN.cpp mexSSAVE.cpp

mex4o: $(patsubst %.cpp, %.mex, $(MEX_OBJECTS))
oct: $(patsubst %.cpp, %.oct, $(MEX_OBJECTS))
mexw32: $(patsubst %.cpp, %.mexw32, $(MEX_OBJECTS))
mexw64: $(patsubst %.cpp, %.mexw64, $(MEX_OBJECTS))

ifdef MEX_EXT
mex: mex4o mex4m mexw32 mexw64
mex4m: $(patsubst %.cpp, %.$(MEX_EXT), $(MEX_OBJECTS))

ifneq (Darwin,$(shell uname))
%.$(MEX_EXT): %.cpp
	$(MEX) $(MEX_OPTION) $(DEFINES) $(shell $(PKGCONF) --cflags libbiosig) "$<" $(shell $(PKGCONF) --libs libbiosig) -o "$@"
else
%.$(MEX_EXT): %.cpp
	## $(MEX) $(MEX_OPTION) $(DEFINES) "$<" $(shell $(PKGCONF) --libs libbiosig) -o "$@"
	$(MEX) $(DEFINES) $(shell $(PKGCONF) --cflags libbiosig) -outdir mex "$<" -lbiosig
endif

else
mex: mex4o mexw32 mexw64
endif

%.mex: %.cpp
	$(OCT) $(DEFINES) $(shell $(PKGCONF) --cflags libbiosig) -v -g --mex "$<" -lbiosig $(shell $(PKGCONF) --libs libbiosig) -o "$@"

%.oct: %.cpp
	$(OCT) $(DEFINES) $(shell $(PKGCONF) --cflags libbiosig) "$<" -lbiosig $(shell $(PKGCONF) --libs libbiosig) -o "$@"

%.mexw32: %.cpp Makefile biosig.h biosig-dev.h
	## $(CROSS)-g++ is used instead of $(CXX), so it can be called from biosig as well as mxe. 
	$(CROSS)-g++ -shared $(GNUMEX)/mex.def -DMATLAB_MEX_FILE $(DEFINES) -x c++ \
		-I$(prefix)/include \
		-I$(W32MAT_INC) -O2 -o "$@" -L$(GNUMEX) -s "$<" -llibmx -llibmex -lbiosig -liconv -lssp $(LDLIBS) -lws2_32

%.mexw64: %.cpp Makefile biosig.h biosig-dev.h
	$(CROSS64)-g++ -shared $(GNUMEX64)/mex.def -DMATLAB_MEX_FILE $(DEFINES) -x c++ \
		-I$(prefix)/include \
		-I$(W64MAT_INC) -O2 -o "$@" -L$(GNUMEX64) -s "$<" -llibmx -llibmex -lbiosig -liconv -lssp $(LDLIBS) -lws2_32


