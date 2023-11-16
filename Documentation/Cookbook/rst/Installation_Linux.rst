We provide a Core package and standalone optional packages to install over the Core for GNU/Linux x86_64. These packages includes
all of the OTB applications along with command line launchers.
Since OTB 9.0, the installation is modular, you have to choose what package to download depending on your use case.

This is the list of the different modules available to download :

+-----------------------+-------------------------------------------------------------------------------------+-----------------------+
| **Module Name**       | **Description**                                                                     | **Criticity**         |
+=======================+=====================================================================================+=======================+
| Core and Dependencies | Core applications,modules and the required dependencies to run OTB                  | **Mandatory**         |
+-----------------------+-------------------------------------------------------------------------------------+-----------------------+
| ImageProcessing       | Contains apps related to MuParser and MuParserX                                     | Optional              |
+-----------------------+-------------------------------------------------------------------------------------+-----------------------+
| Learning              | Learning apps using OpenCV, SVM, Shark                                              | Optional              |
+-----------------------+-------------------------------------------------------------------------------------+-----------------------+
| SAR                   | Apps and modules used for Radar processing                                          | Optional              |
+-----------------------+-------------------------------------------------------------------------------------+-----------------------+
| StereoProcessing      | Apps and modules for stereo operations                                              | Optional              |
+-----------------------+-------------------------------------------------------------------------------------+-----------------------+
| Hyperspectral         | Apps and modules to perform calculations on hyperspectral images                    | Optional              |
+-----------------------+-------------------------------------------------------------------------------------+-----------------------+

Download packages
~~~~~~~~~~~~~~~~~

**Important note**

If you are using Fedora/Redhat8, please download the package Linux_RH8-Core which contains GDAL bindings in a different folder than the Core package.
It is because the default system Python user site in Redhat8 is lib/python3.8/site-packages and in ubuntu/debian it is lib/python3/dist-packages

Recommended Installation : Core + Optional packages 
```````````````````````````````````````````````````

In most cases, you will need the **OTB-All-Dependencies-9.0.tar.gz** package in order to run Core **AND** optional packages that you can install afterwards.
Let's say you want to start using OTB only with the Core applications, and some months later you realize that you need to do more specific operations such as Learning. 
In that case you will just need to download the OTB-Learning package, and untar it where you installed the Core package. 
You can then use directly the applications packaged in the Learning package alongside the other Core apps.
If you want your installation to be the same as with OTB < 9, you can download all the optional packages.

.. code-block:: bash

   # Download mandatory packages to run OTB
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-Core-9.0.tar.gz
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-All-Dependencies-9.0.tar.gz
   # Download optional package
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-Learning-9.0.tar.gz
   ...


Core Only
`````````

In this use case, you'll only be able to run Core applications, you **CANNOT** install any optional package over it as the necessary dependencies are not installed.
it is possible to make your installation available to optional modules by downloading the **All-Dependencies** package and install it in the current OTB core install.
This installation will prompt you to replace the existing dependencies, just click OK.  

.. code-block:: bash

   # Download mandatory packages to run OTB
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-Core-9.0.tar.gz
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-Core-Dependencies-9.0.tar.gz


Installation
~~~~~~~~~~~~

This package is a self-extractable tar.gz archive. You may uncompress the files with a
right-click => Extract to => create OTB-|release| folder and click OK, or from the command line as follows:

.. code-block:: bash

   # Install each tar gz in the same "one top level" folder
   tar xvf OTB-Core-9.0.tar.gz --one-top-level="/Path/To/Install/OTB"
   tar xvf OTB-All-Dependencies-9.0.tar.gz --one-top-level="/Path/To/Install/OTB"
   ...

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

**Our recommendation is to always recompile the python bindings when you install OTB**

Since OTB 8.0.0 OTB bindings for Python 3.8 are distributed as a binary
package. Please note that using a different Python version may not be compatible with
OTB wrappings directly after installation. If the installation completes
without issue, information relating to your Python bindings will be provided. 

You must have Python NumPy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option 
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 3.8 will be enabled with the ``otbenv`` script.

Recompiling Python bindings
```````````````````````````

If you are using another version of Python 3 than 3.8, but still want to use OTB Python bindings, it is possible
to compile the python bindings again with your version of Python. CMake is required (it is available in most package
managers or at [https://cmake.org/]). Make sure you installed the necessary dependencies in the :doc:`First Step` page

At the root of the OTB installation run :

.. code-block:: bash

    source otbenv.profile 
    sh recompile_bindings.sh

You should now be able to import ``otbApplication`` through Python !

Create an healthy Python environment for OTB
````````````````````````````````````````````

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
```````
   - Multiple installation of OTB can exists in same system without one conflicting the other!
