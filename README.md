![LOGO](./dist/freedesktop/scalable/fips.svg) FIPS — OpenGL based FITS image viewer
===================================================================================
[![Build Status](https://travis-ci.org/matwey/fips3.svg?branch=master)](https://travis-ci.org/matwey/fips3)

FIPS is a cross-platform [FITS](https://fits.gsfc.nasa.gov) viewer with
responsive user interface. Unlike other FITS viewers FIPS uses GPU hardware via
OpenGL to provide usual functionality such as zooming, panning and level
adjustments. OpenGL 2.1 and later is supported.

FIPS supports [all](http://archive.stsci.edu/fits/users_guide/) 2D image formats
except of 64-bit floating point numbers (`BITPIX=-64`). FITS image extension has
basic limited support.

Build requirements
------------------

-   [Cmake](https://cmake.org)

-   [QT](https://qt.io) 5.6+

-   C++11 compatible C++ compiler, e.g. `gcc` 4.8+ or `clang` 3.4+

Installation
------------

### Installing from package manager

#### openSUSE

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
zypper ar -f obs://home:matwey:fips fips
zypper in fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you haven't found your distro above than you can build from sources or
contribute building packages for your favourite distro. Let us know to insert
missing instructions.

### Building from sources

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
git clone https://github.com/matwey/fips3
mkdir -p fips3/build
cd fips3/build
cmake ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 
