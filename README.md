![LOGO](/dist/freedesktop/128x128/space.fips.Fips.png)

[![Build Status](https://travis-ci.org/matwey/fips3.svg?branch=master)](https://travis-ci.org/matwey/fips3)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/matwey/fips3?branch=master&svg=true)](https://ci.appveyor.com/project/matwey/fips3)

FIPS — OpenGL based FITS image viewer
=====================================

FIPS is a cross-platform [FITS](https://fits.gsfc.nasa.gov) viewer with
responsive user interface. Unlike other FITS viewers FIPS uses GPU hardware via
OpenGL to provide usual functionality such as zooming, panning and level
adjustments. OpenGL 2.1 and later is supported.

FIPS supports [all](http://archive.stsci.edu/fits/users_guide/) 2D image formats
(except for floating point formats on OpenGL 2.1). FITS image extension has
basic limited support.

![Screenshot](/.screenshot/1.png)

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
zypper ar -f obs://science fips
zypper in fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### FlatPak
[FlatPak](https://flatpak.org) is cross-distribution Linux desktop package
manager with isolation.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub space.fips.Fips
flatpak run space.fips.Fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### macOS

Download [release package](https://github.com/matwey/fips3/releases/latest) or
install using [Homebrew](https://brew.sh) [Caskroom](http://caskroom.github.io):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
brew cask install fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Windows

Download [release package](https://github.com/matwey/fips3/releases/latest).

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

Community
---------

- Report bugs on [GitHub](https://github.com/matwey/fips3/issues)
- Join us on Telegram [@fips_space](https://t.me/fips_space)

