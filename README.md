![LOGO](/dist/freedesktop/128x128/space.fips.Fips.png)

[![Build Status](https://github.com/matwey/fips3/actions/workflows/tests.yml/badge.svg)](https://github.com/matwey/fips3/actions/workflows/tests.yml)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/matwey/fips3?branch=master&svg=true)](https://ci.appveyor.com/project/matwey/fips3)
[![ascl:1808.006](https://img.shields.io/badge/ascl-1808.006-blue.svg?colorB=262255)](https://ascl.net/1808.006)

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

-   (Windows only) [WiX Toolset](https://wixtoolset.org/)

Installation
------------

### Installing from package manager

#### openSUSE Leap 15.1 (and later) and openSUSE Tumbleweed

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
zypper in fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### openSUSE (other versions)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
zypper ar -f obs://science fips
zypper in fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Fedora 30 (and later)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dnf install fips
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### [EPEL](https://fedoraproject.org/wiki/EPEL): RHEL, CentOS, and others (8.2 and later)

Before installing the package, enable
[EPEL](https://fedoraproject.org/wiki/EPEL) repository as recommended
[here](https://fedoraproject.org/wiki/EPEL#Quickstart).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dnf install fips
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

Download [release package](https://github.com/matwey/fips3/releases/latest).

#### Windows

Download [release package](https://github.com/matwey/fips3/releases/latest).

If you haven't found your distro above then you can build from sources or
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
$QtDir = 'E:\Libs\Qt\5.14.1\msvc2017_64' # make sure the path and platform is right
$Generator = 'Visual Studio 16 2019'
$Architecture = 'x64' # or Win32
$Configuration = 'Release' # or Debug

$env:PLATFORM = $Platform
$env:QTDIR = $QtDir

$env:PATH = "$QtDir\bin;$env:PATH"
cmake -G"$Generator" -A $Architecture .
cmake --build . --config $Configuration --target PACKAGE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Community
---------

- Report bugs on [GitHub](https://github.com/matwey/fips3/issues)
- Join us on Telegram [@fips_space](https://t.me/fips_space)

Citation
--------
If you have benefited from using Fips in your research, please find an opportunity to cite paper [Kornilov & Malanchev (2019)](http://adsabs.harvard.edu/abs/2019A%26C....26...61K):

```
@ARTICLE{FIPS,
   author = {Matwey {Kornilov} and Kosntantin {Malanchev}},
    title = "{Fips: An OpenGL based FITS viewer}",
  journal = {Astronomy and Computing},
archivePrefix = "arXiv",
   eprint = {1901.10189},
 primaryClass = "astro-ph.IM",
 keywords = {Techniques, Image processing, Image-based rendering, Graphical user interfaces},
     year = 2019,
    month = jan,
   volume = 26,
      eid = {61},
    pages = {61},
      doi = {10.1016/j.ascom.2019.01.001},
   adsurl = {http://adsabs.harvard.edu/abs/2019A%26C....26...61K}
}
```
