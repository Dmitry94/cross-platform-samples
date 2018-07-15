#!/bin/bash

cmake -D UPDATE_TRANSLATIONS=OFF -D CMAKE_INSTALL_PREFIX=./install $1
make
make translations
make install
