####### Makefile for "BioSig for C/C++" #####################
###
###  Copyright (C) 2017 Alois Schloegl <alois.schloegl@gmail.com>
###  This file is part of the "BioSig for C/C++" repository
###  (biosig4c++) at http://biosig.sf.net/
###
##############################################################

# More information on SWIG-Ruby interface is available here
# http://www.swig.org/Doc1.3/Ruby.html


all:
	$(error support for biosig on ruby is not ready yet)
	swig -c++ -ruby biosig.i
	ruby extconf.rb
	make

clean:
	-rm *.cxx
	-rm *.o
	-rm *.pm
	-rm *.so
