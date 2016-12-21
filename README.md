MITIE-PHP
---------

**ABOUT**

A PHP 5 extension based on PHP-CPP that wraps the excellent MITIE (MIT Information Extraction) library https://github.com/mit-nlp/MITIE

**INSTALLATION**

There are no pre-compiled binaries (yet), so you will have to compie from source. The installation instructions below are relevant for a Debian/Ubuntu box.

The library is based on PHP-CPP, so first you need to install php-dev:

`sudo apt-get install php5-dev` or `sudo apt-get install php5.6-dev` (depending on which version of PHP5 you have installed).

Download and install the PHP-CPP library by following the instructions here:
http://www.php-cpp.com/documentation/install

Clone the PHP-MITIE repo:

`git clone https://github.com/rjjakes/MITIE-PHP.git`

And pull down the MITIE library which is referenced as a git submodule:

`git pull --recurse-submodules`

Edit these two lines in the Makefile so the config and ini directories point to your local directories (in the example below, I have PHP5.6 installed and will be attaching the library to the command line interface instance of PHP).
 
`PHP_INI_DIR	    = /etc/php/5.6/mods-available/`

`PHP_CONFIG_DIR	= /etc/php/5.6/cli/conf.d/`


Now build the project:

`make && sudo make install`

**USAGE**

@todo