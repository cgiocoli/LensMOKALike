#####################################################################
#                                                                   #
#               Make file for LensMOKALike                          #
#                                                                   #
#                         cgiocoli@gmail.com                        #
#####################################################################

# executable name
PROG = $(HOME)/bin/LensMOKALike-beta

MAIN = mainLMOKAL.cpp

# .cpp lib internal in MOKA
SOURCES = ../Moka/utilities.cpp \
          ../Moka/cosmology.cpp \
          ../Moka/fftw_lens.cpp 

# gsl, cfitsio, CCfits, fftw
LIBS = -L/Users/cgiocoli/lib/gsl-1.13/lib  -lgslcblas -lgsl \
       -L/Users/cgiocoli/lib/cfitsio/lib \
       -L/Users/cgiocoli/lib/CCfits/lib -lCCfits -lcfitsio \
       -L/Users/cgiocoli/lib/fftw-3.2.2/lib -lfftw3 -lm 

# gsl, cfitsio, CCfits, fftw  
ALLFLAGS = -I/Users/cgiocoli/lib/gsl-1.13/include/gsl \
	   -I/Users/cgiocoli/lib/gsl-1.13/include \
           -I/Users/cgiocoli/lib/cfitsio/include \
           -I/Users/cgiocoli/lib/CCfits/include \
           -I/Users/cgiocoli/lib/fftw-3.2.2/include

# 
DEBUG = #-g -Wall -O2
CC = /usr/bin/cc -std=c++11 -lstdc++
#
RM = rm -f -r
#
OBJ = $(SOURCES:.cpp=.o)
#

CFLAGS=$(ALLFLAGS) $(DEBUG)
#
CLEAR = clear

default: moka
moka: 
	$(CC) -c $(SOURCES) $(CFLAGS)
	ar r libmokalike.a *.o
	$(CC) $(MAIN) -L. -lmokalike $(CFLAGS) -o $(PROG) $(LIBS) 

main: 
	$(CC) $(MAIN) -L. -lmokalike $(CFLAGS) -o $(PROG) $(LIBS) 

clean:
	$(RM) $(PROG) *.o

lib:
	$(CC) -c $(SOURCES) $(CFLAGS)
	ar r libmokalike.a *.o

clall:
	$(RM) $(PROG) *.o libmokalike.a *~

clall0:
	$(RM) $(PROG) *.o libmokalike.a *~ html

new: clean default


