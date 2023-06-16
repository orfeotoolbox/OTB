**Before extracting the package, make sure that you have been through the** :doc:`First_Step` page

We provide a binary package for GNU/Linux x86_64. This package includes
all of the OTB applications along with command line launchers.
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

-  ``otbenv.profile``: A script to initialize the environment for OTB
   executables

-  ``bin``: A folder containing application launcher (otbcli.sh)

-  ``lib``: A folder containing all shared libraries and OTB
   applications.

-  ``include``: A folder containing all the necessary headers to compile OTB
   based projects.

-  ``share``: A folder containing common resources and copyright
   mentions.

-  ``tool``: A folder containing useful scripts to test the installation or
   to uninstall OTB libraries and headers while keeping all the dependencies.

If you want to use the otbcli launchers, you can initialize your
environment with ``source otbenv.profile``.

The package can be used to compile other projects using OTB (binaries, libraries
and headers are included). If you want to build OTB from source using this
package, you should first uninstall the specific OTB files from the package to
leave only the dependencies (what we call an XDK). You can do it using the
supplied script ``tools/uninstall_otb.sh``.

Python bindings
~~~~~~~~~~~~~~~

Since OTB 8.0.0 OTB bindings for Python 3.8 are distributed as a binary
package. (From OTB 6.7 to 7.4, bindings are provided for Python 3.5)
Please note that using a different Python version may not be compatible with
OTB wrappings. If the installation completes
without issue, information relating to your Python bindings will be provided. 

You must have Python NumPy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option 
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 3.8 will be enabled with the ``otbenv`` script.

Recompiling Python bindings
+++++++++++++++++++++++++++

If you are using another version of Python 3 than 3.8, but still want to use OTB Python bindings, it is possible
to compile the python bindings again with your version of Python. CMake is required (it is available in most package
managers or at [https://cmake.org/]). Make sure you installed the necessary dependencies in the **First Step** page

At the root of the OTB installation run :

.. parsed-literal::

    source otbenv.profile 
    ctest -S share/otb/swig/build_wrapping.cmake -VV

You should now be able to import ``otbApplication`` through Python !

Create an healthy Python environment for OTB
++++++++++++++++++++++++++++++++++++++++++++

We strongly recommend to use a virtual env to **avoid conflicts between OTB and GDAL when you develop python scripts that uses other dependencies like rasterio, scikit...**

.. code-block:: bash

   # Source your OTB environment
   . <your installation directory>/otbenv.profile
   # Create a virtual env and install some libraries
   python -m venv otb_venv
   . otb_venv/bin/activate
   pip install --upgrade pip
   pip install scikit-image scikit-learn geopandas 
   # Rastero depends on GDAL and need to be compiled on the flight with current OTB's own GDAL
   pip install rasterio --no-binary :all:
   # Use your libraries within Python
   python
   > import rasterio
   > import otbApplication as otb


Notes:
~~~~~~

- You might be tempted to move "OTB-|release|-Linux64" into another location say /usr/local/ after extraction. But avoid this action!

- To have "OTB-|release|-Linux64" installed in /usr/local or /opt execute "OTB-|release|-Linux64.run" in that directory.

- Multiple installation of OTB can exists in same system without one conflicting the other!

FAQ
~~~

Q: Unable to import otbApplication library with Python3.8
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

   ImportError: libpython3.8m.so.rh-python38-1.0: cannot open shared object file: No such file or directory

A: You need to add a symlink to libpython3.8m.so.rh-python38-1.0 to make it work. 

Here is the solution:

- Find the libpython3.8XX on your system : ``find /usr/lib -iname *libpython3.8*``
  (on Ubuntu 20.04, it is ``/usr/lib/x86_64-linux-gnu/libpython3.8m.so``)
- Create a symlink : ``ln -s path/to/lib/python3.8XX path/to/lib/libpython3.8m.so.rh-python38-1.0``
- Try to import otbApplication again

See this discussion on `OTB issue tracker <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/1540#note_67864>`_
