
# MITIE directories and flags

DLIBDIR = ./MITIE/dlib
MITIEDIR = ./MITIE/mitielib

# Set up PHP-CPP compilation
CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11 -fPIC -W -O3 -I$(MITIEDIR)/include -I$(DLIBDIR)


#Edit these lines to correspond with your own directories
EXTENSION_DIR	=	$(shell php-config --extension-dir)

PHP_INI_DIR	    = /etc/php/5.6/mods-available/
PHP_CONFIG_DIR	= /etc/php/5.6/cli/conf.d/

LD              = g++
LD_FLAGS        = -Wall -Wl,-rpath,/home/work/lib -shared -O2 -L/home/work/lib
RESULT          = MITIE.so

PHPINIFILE		= MITIE.ini

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

install:
		cp -f ${RESULT} ${EXTENSION_DIR}
		cp -f ${PHPINIFILE}	${PHP_INI_DIR}
		ln -s -f ${PHP_INI_DIR}${PHPINIFILE} ${PHP_CONFIG_DIR}30-${PHPINIFILE}

new:
		clean
		make

