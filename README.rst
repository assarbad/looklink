﻿ ``CReparsePoint`` and ``looklink``
====================================

About
-----
``CReparsePoint`` is a C++ utility class without dependencies to the STL, MFC,
ATL or some such. It allows you to retrieve all the data stored inside reparse
points, the mechanism underlying junction points since Windows 2000 and symbolic
links (both for files and directories) since Windows Vista. It also supports
placeholder files on Windows 8.1 and later.

``looklink`` is a simple tool to investigate reparse point properties from the
command line.

Installation
------------
No installation required. Download the binaries (32 and 64bit) from `SourceForge`_.
Use an archiver to unpack the archive, I recommend `7-Zip`_, and then place the
``.exe`` files into a folder listed in your ``PATH`` environment variable.

As for the utility class all you need is to place the files ``ReparsePoint.h``
and ``VerySimpleBuf.h`` into your project, ``#include`` the former and you
should be set to use ``CReparsePoint``.

Build instructions
------------------
If you prefer to use Visual Studio, a solution/project for Visual Studio 2005 is
included and you can generate any of the other supported projects yourself using
the `premake4`_ that comes with the WinDirStat source code. Use a tool such as
``sigcheck`` to verify the code signature on it, though.

The way you generate the projects is by calling ``premake4`` one of the
following ways::

    premake4 vs2005
    premake4 vs2008
    premake4 vs2010
    premake4 vs2012
    premake4 vs2013
    premake4 vs2015
    premake4 vs2017
    premake4 vs2019
    premake4 vs2022

The earlier Visual Studio versions may work, but no guarantees.

My prepared premake4.lua can take a number of arguments. Most notably it'll take

  * ``--xp`` to generate XP-compatible projects on newer VS versions.
  * ``--msvcrt`` to use the ``msvcrt.dll`` import Windows Vista WDK which makes
    the resulting binary statically import ``msvcrt.dll``, but in the version
    backwards compatible until Windows 2000 (and 2003 for x64).
    In order to make use of this, you have to set the environment variable
    ``WLHBASE`` to point to your WDK (e.g. ``WLHBASE=C:\WINDDK\6001.18002``).
  * ``--release`` will generate the release solution (no ``Debug`` configuration)
    and is used by the ``relbuild.cmd`` script. This option implies ``--xp`` and
    ``--msvcrt`` at the time of this writing.

Requirements
------------
The program should run on Windows 2000 and later for x86-32, and on Windows 2003
and later for x86-64, aka x64 (this includes Windows XP x64, which used the same
code base as Windows 2003 Server). Itanium CPUs are not supported, but feel free
to build that yourself, if needed.

The ``CReparsePoint`` class should work starting on Windows 2000, too.

Usage
-----

Syntax::

    looklink [--nologo] [--verbose] [--noerror] <path>

* ``--nologo`` will prevent showing all the information about the tool iself.
* ``--verbose`` will add some details to the output, including the file ID on NTFS.
* ``--noerror`` will suppress the error when ``<path>`` doesn't point to a reparse point.

License
-------
The tool and the classes are placed into the PUBLIC DOMAIN/CC0, with the
exception of CVersionInfo (inside ``VersionInfo.hpp``), which is licensed under
the very liberal MIT license. Licensing information can usually be found at the
top of each source file. Anything not explicitly licensed in the source file can
be assumed to have been placed into the PUBLIC DOMAIN/CC0.

.. _premake4: https://sourceforge.net/projects/premake4-wds/files/
.. _SourceForge: https://sourceforge.net/projects/looklink/files/
.. _7-Zip: https://7-zip.org/
