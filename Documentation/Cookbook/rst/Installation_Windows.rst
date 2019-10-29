Windows binary packages are available for Windows 7 or higher. They can
be downloaded from `otb download page <https://www.orfeo-toolbox.org/download>`_ .

Pick the correct version (32 bit or 64 bit) depending on your system.

Extract the archive and use one of the launchers, they contain all applications
and their launchers (both command line and graphical launchers are provided):

-  ``monteverdi.bat``: A launcher script for Monteverdi

-  ``mapla.bat``: A launcher script for Mapla

-  ``otbenv.bat``: A script to initialize the environment for OTB
   executables

-  ``bin``: A folder containing application launchers (otbcli.bat,
   otbgui.bat) and the DLLs.

-  ``lib``: A folder containing application DLLs.

-  ``include``: A folder containing all the necessary headers to compile OTB
   based projects.

-  ``tool``: A folder containing useful scripts to test the installation or
   to uninstall OTB libraries and headers while keeping all the dependencies.

The applications can be launched from the Mapla launcher. If you want to
use the otbcli and otbgui launchers, you can initialize a command prompt
with ``otbenv.bat``.

The package can be used to compile other projects using OTB (binaries, libraries
and headers are included). If you want to build OTB from source using this
package, you should first uninstall the specific OTB files from the package to
leave only the dependencies (what we call an XDK). You can do it using the
supplied script ``tools/uninstall_otb.bat``.

In the package you also have a template project for Visual 2015
``OTB Project.zip``. This template can be placed in your user Visual 2015 template
directory : ``%USERPROFILE%\Documents\Visual Studio 2015\Templates\ProjectTemplates``.
The script ``start_devenv.bat`` allows to copy the template in that folder and
start Visual Studio.

Python bindings
~~~~~~~~~~~~~~~

Since OTB 6.7.0 OTB bindings for Python 3.5 are distributed as a binary
package.
Please note that using a different Python version may not be compatible with
OTB wrappings. If the installation completes
without issue, information relating to your Python bindings will be provided. 

You must have Python numpy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option 
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 3.5 will be enabled with the ``otbenv`` script.

Notes:
~~~~~~

- You must have "Visual C++ Redistributable for Visual Studio 2015" installed for using this package.
  It can be downloaded freely from `microsoft <https://www.microsoft.com/en-us/download/details.aspx?id=48145>`_


