![LOGO](/dist/freedesktop/128x128/fips.png)

[![Build Status](https://travis-ci.org/matwey/fips3.svg?branch=master)](https://travis-ci.org/matwey/fips3)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/matwey/fips3?branch=master&svg=true)](https://ci.appveyor.com/project/matwey/fips3)

FIPS — OpenGL based FITS image viewer
=====================================

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

-   [Qt](https://www.qt.io) 5.6+

-   C++11 compatible C++ compiler, e.g. `gcc` 4.8+ or `clang` 3.4+

-   (Windows only) [NSIS](http://nsis.sourceforge.net/Main_Page)

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

For Windows (make sure you have `cmake` in your `PATH`, then run in PowerShell):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~powershell
$Platform = 'x64' # or 'x86'
$QtDir = 'E:\Libs\Qt\5.9.1\msvc2017_64' # make sure the path and platform is right
$Generator = 'Visual Studio 15 2017 Win64' # drop ' Win64' to build x86 version
$Configuration = 'Release' # or Debug

$env:PLATFORM = $Platform
$env:QTDIR = $QtDir

$env:PATH = "$QtDir\bin;$env:PATH"
cmake -G"$Generator" . -DFIPS_CONFIGURATION="$Configuration"
cmake --build . --config $Configuration --target PACKAGE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
