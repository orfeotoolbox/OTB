**Before extracting the package, make sure that you have been through the** :doc:`First_Steps` page

Windows binary packages are available for Windows 10 or higher. They can
be downloaded from `otb download page <https://www.orfeo-toolbox.org/download>`_ .

Extract the zip archive and use one of the launchers, they contain all applications
and their launchers (both command line and graphical launchers are provided):

-  ``otbenv.bat``: A script to initialize the environment for OTB
   executables

-  ``bin``: A folder containing application launchers (otbcli.bat) and the DLLs.

-  ``lib``: A folder containing application DLLs.

-  ``include``: A folder containing all the necessary headers to compile OTB
   based projects.

-  ``tool``: A folder containing useful scripts to test the installation or
   to uninstall OTB libraries and headers while keeping all the dependencies.

If you want to use the otbcli launcher, you can initialize a command
prompt with ``otbenv.bat``.

The package can be used to compile other projects using OTB (binaries, libraries
and headers are included). If you want to build OTB from source using this
package, you should first uninstall the specific OTB files from the package to
leave only the dependencies (what we call an XDK). You can do it using the
supplied script ``tools/uninstall_otb.bat``.

Python bindings
~~~~~~~~~~~~~~~

Since OTB 8.0.0, the bindings for Python 3.7 are distributed.
Please note that using a different Python version may not be compatible with
OTB wrappings. If the installation completes
without issue, information relating to your Python bindings will be provided.

You must have Python numpy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 3.7 will be enabled with the ``otbenv`` script.

