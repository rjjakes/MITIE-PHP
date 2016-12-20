
# MITIE directories and flags

DLIBDIR = ./MITIE/dlib
MITIEDIR = ./MITIE/mitielib

# Set up PHP-CPP compilation
CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -I/home/work/include/ -O2 -std=c++11 -fPIC -W -O3 -I$(MITIEDIR)/include -I$(DLIBDIR)

PREFIX			= /home/work/



#Edit these lines to correspond with your own directories
LIBRARY_DIR		= ${PREFIX}/local/php/lib/php/extensions/no-debug-non-zts-20090626/

PHP_CONFIG_DIR	= /home/work/local/php/lib/

LD              = g++
LD_FLAGS        = -Wall -Wl,-rpath,/home/work/lib -shared -O2 -L/home/work/lib
RESULT          = MITIE.so

PHPINIFILE		= 30-MITIE.ini

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

#install:
#		cp -f ${RESULT} ${LIBRARY_DIR}
#		cp -f ${PHPINIFILE}	${PHP_CONFIG_DIR}

new:
		clean
		make

