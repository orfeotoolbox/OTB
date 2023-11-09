Installing OTB
~~~~~~~~~~~~~~

We provide a Core package and standalone optional packages to install over the Core for GNU/Linux x86_64. These packages includes
all of the OTB applications along with command line launchers.
It can be downloaded from `OTB's download page
<https://www.orfeo-toolbox.org/download>`__.

Since OTB 9.0, the installation is modular, you have to choose what package to download depending on your use case.

This is the list of the different modules available :

+-----------------------------------------------------------------------------------------------------------------------------------------+
| **Module Name**           | **Description**                                                                     | **Criticity**         |
+-----------------------------------------------------------------------------------------------------------------------------------------+
| OTB_Core and Dependencies | Contains all Core applications and modules + the required dependencies to run OTB   | **Mandatory**         |
+-----------------------------------------------------------------------------------------------------------------------------------------+
| OTB_ImageProcessing       | Test                                                                                | Optional              |
+-----------------------------------------------------------------------------------------------------------------------------------------+
| OTB_Learning              | Test                                                                                | Optional              |
+-----------------------------------------------------------------------------------------------------------------------------------------+
| OTB_SAR                   | Test                                                                                | Optional              |
+-----------------------------------------------------------------------------------------------------------------------------------------+
| OTB_StereoProcessing      | Test                                                                                | Optional              |
+-----------------------------------------------------------------------------------------------------------------------------------------+

Depending on your use case, you have to choose what package to download :

Default choice : Core + Optional packages 
++++++++++++++++++++++++++++++++++++++++++

In most cases, you should take the mandatory **OTB-Core-9.0.tar.gz** package, along with the **OTB-All-Dependencies-9.0.tar.gz** one, which permits you
to install optional packages afterwards. Let's say you want to start using OTB only with the Core applications, and some months later
you realize that you need to do more specific operations such as Learning. 
In that case you will just need to download the OTB-Learning package, and unzip it where you installed the Core package. 
You can then use directly the applications packaged in the Learning package alongside the other Core apps.

Core Only
+++++++++

In that case, you are sure you only need the core applications of OTB, resulting in a lighter install because there are less dependencies needed.
You only have to download **OTB-Core-9.0.tar.gz** and **OTB-Core-Dependencies-9.0.tar.gz**


This package is a self-extractable tar.gz archive. You may uncompress the files with a
right-click => Extract to => create OTB-|release| folder and click OK, or from the command line as follows:

.. code-block:: bash

   for f in *.tar.gz; do tar xvf "$f" --one-top-level="/Path/To/Install/OTB"; done

Please note that the resulting installation can be moved elsewhere on the disk. Just move the folder
elsewhere and source the otbenv.profile.
Once the archive is extracted, the directory structure consists of:

-  ``otbenv.profile``: A script to initialize the environment for OTB
   executables

- ``recompile_bindings.sh`` : A script to recompile the python bindings with your system's Python

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
    sh recompile_bindings.sh

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
   - Multiple installation of OTB can exists in same system without one conflicting the other!

FAQ
~~~

Q: Unable to import otbApplication library with Python3.8
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

   ImportError: libpython3.8m.so.rh-python38-1.0: cannot open shared object file: No such file or directory

A: You need to add a symlink to libpython3.8m.so.rh-python38-1.0 to make it work. 
The recommended solution is to **recompile the bindings with your systems' python version**, because this symbolic link will work only
if you have Python 3.8 in your system which is the case on Ubuntu 20.04.

Here is the solution for the symbolic link:

- Find the libpython3.8XX on your system : ``find /usr/lib -iname *libpython3.8*``
  (on Ubuntu 20.04, it is ``/usr/lib/x86_64-linux-gnu/libpython3.8m.so``)
- Create a symlink : ``ln -s path/to/lib/python3.8XX path/to/lib/libpython3.8m.so.rh-python38-1.0``
- Try to import otbApplication again

See this discussion on `OTB issue tracker <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/1540#note_67864>`_
