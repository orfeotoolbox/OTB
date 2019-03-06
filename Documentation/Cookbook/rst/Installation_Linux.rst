We provide a binary package for GNU/Linux x86_64. This package includes
all of the OTB applications along with command line and graphical launchers.
It can be downloaded from `OTB's download page
<https://www.orfeo-toolbox.org/download>`__.

This package is a self-extractable archive. You may uncompress it with a
double-click on the file, or from the command line as follows:

.. parsed-literal::

   chmod +x OTB-|release|-Linux64.run
   ./OTB-|release|-Linux64.run

The self-extractable archive only needs common tools found on most Linux
distributions ("sed", "grep", "find", "cat", "printf", "ln", ...). However, be
aware that it requires tools such as "which" and "file" (they are not always
present, for instance when building a container).

Please note that the resulting installation is not meant to be moved,
you should uncompress the archive in its final location. Once the
archive is extracted, the directory structure consists of:

-  ``monteverdi.sh``: A launcher script for Monteverdi

-  ``mapla.sh``: A launcher script for Mapla

-  ``otbenv.profile``: A script to initialize the environment for OTB
   executables

-  ``bin``: A folder containing application launchers (otbcli.sh,
   otbgui.sh), Monteverdi and Mapla.

-  ``lib``: A folder containing all shared libraries and OTB
   applications.

-  ``include``: A folder containing all the necessary headers to compile OTB
   based projects.

-  ``share``: A folder containing common resources and copyright
   mentions.

-  ``tool``: A folder containing useful scripts to test the installation or
   to uninstall OTB libraries and headers while keeping all the dependencies.

The applications can be launched from the Mapla launcher. If you want to
use the otbcli and otbgui launchers, you can initialize your environment
with ``source otbenv.profile``.

The package can be used to compile other projects using OTB (binaries, libraries
and headers are included). If you want to build OTB from source using this
package, you should first uninstall the specific OTB files from the package to
leave only the dependencies (what we call an XDK). You can do it using the
supplied script ``tools/uninstall_otb.sh``.

System dependencies
~~~~~~~~~~~~~~~~~~~

In order to run the command line launchers, this package doesn’t require
any special library that is not present in most modern Linux
distributions. The graphical executable (otbgui launchers, Monteverdi
and Mapla) use the X11 libraries, which are widely used in a lot of
distributions:

::

    libx11-6 libxext6 libxau6 libxxf86vm1 libxdmcp6 libdrm2

Monteverdi also requires the standard graphics libraries **libgl1** and
**libglu1**. Make sure you have at least one version of them installed
in your system.

Caveat on OTB 6.0
~~~~~~~~~~~~~~~~~

In OTB 6.0 binaries, there is a small caveat for "expat" as the supplied binaries
depend on "libexpat.so", which is not contained in the package. It can be
supplied by most package managers (apt, yum, ...). If not already present, it is
necessary to install one of the following packages:

::

    libexpat-dev   libexpat1-dev

Python bindings
~~~~~~~~~~~~~~~

Since OTB 6.7.0 OTB bindings for Python 3.5 are distributed as a binary
package.
Please note that using a different Python version may not be compatible with
OTB wrappings. If the installation completes
without issue, information relating to your Python bindings will be provided. 

You must have Python NumPy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option 
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 3.5 will be enabled with the ``otbenv`` script.

Notes:
~~~~~~

- You must use monteverdi and mapla through ``mapla.sh`` and ``monteverdi.sh`` helper scripts in extracted directory.

- The helper scripts for monteverdi and mapla set required environment variables

- You might be tempted to move "OTB-|release|-Linux64" into another location say /usr/local/ after extraction. But avoid this action!

- To have "OTB-|release|-Linux64" installed in /usr/local or /opt execute "OTB-|release|-Linux64.run" in that directory.

- Multiple installation of OTB can exists in same system without one conflicting the other!

FAQ
~~~

Q: Unable to import otbApplication library with Python3
+++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

   ImportError: libpython3.5m.so.rh-python35-1.0: cannot open shared object file: No such file or directory

A: You need to add a symlink to libpython3.5m.so.rh-python35-1.0 to make it works. 

Here is the solution:

- Find the libpython3.5XX on your system : ``find /usr/lib -iname *libpython3.5*``
  (on Ubuntu 14.04, it is ``/usr/lib/x86_64-linux-gnu/libpython3.5m.so``)
- Create a symlink : ``ln -s path/to/lib/python3.5XX path/to/lib/libpython3.5m.so.rh-python35-1.0``
- Try to import otbApplication again

See this discussion on `OTB issue tracker <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/1540#note_67864>`_
