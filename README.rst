====================================
 ``CReparsePoint`` and ``looklink``
====================================
:Author: Oliver Schneider

About
-----
``CReparsePoint`` is a C++ utility class without dependencies to the
STL, MFC, ATL or some such. It allows you to retrieve all the data
stored inside reparse points, the mechanism underlying junction points
since Windows 2000 and symbolic links (both for files and directories)
since Windows Vista.

``looklink`` is a simple tool to investigate reparse point properties
from the command line.

Installation
------------
No installation required. Download the binaries (32 and 64bit) from
`my website`_. Use an archiver to unpack the archive, I recommend 7zip,
and then place the ``.exe`` files into a folder listed in your ``PATH``
environment variable.

As for the utility class all you need is to place the files
``ReparsePoint.h`` and ``VerySimpleBuf.h`` into your project,
``#include`` the former and you should be set to use ``CReparsePoint``.

Requirements
------------
The program should run on Windows 2000 and later. Itanium CPUs are not
supported, but feel free to build that yourself, if needed.

The ``CReparsePoint`` class should work starting on Windows 2000, too.

Usage
-----

Syntax::

    looklink [--nologo] [--verbose] <path>

* ``--nologo`` will prevent showing all the information about the tool iself.
* ``--verbose`` will add some details to the output, including the file ID on NTFS.

License
-------
The tools and the classes are placed into the PUBLIC DOMAIN (CC0).

.. _my website: https://assarbad.net/stuff/looklink.zip
