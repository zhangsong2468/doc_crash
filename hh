#! /bin/bash

#
# File to compile and open the pdf version of the document.
#

make clean
make html
google-chrome _build/html/index.html

