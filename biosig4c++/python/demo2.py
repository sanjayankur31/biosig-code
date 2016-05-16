####### Demo for Python interface to BioSig" #####################
###
###  Copyright (C) 2009,2016 Alois Schloegl <alois.schloegl@ist.ac.at>
###  This file is part of the "BioSig for Python" repository
###  at http://biosig.sf.net/
###
##############################################################

# download and extract 
#   http://www.biosemi.com/download/BDFtestfiles.zip 
# into /tmp/
# then run this demo 
#
# on linux you can run instead  
#   make test 

import biosig
import numpy as np
import matplotlib.pyplot as plt

## read header 
HDR=biosig.header("data/Newtest17-256.bdf")
print HDR

## read and display data
A=biosig.data("data/Newtest17-256.bdf")

plt.plot(A[:,3:7])
plt.show()


