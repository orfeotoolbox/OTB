We provide a Core package and standalone optional packages to install over the Core for GNU/Linux x86_64. They include
all of the OTB applications along with command line launchers.
Since OTB 9.0, it is now possible to have a modular installation, you have to choose which package to download depending on your use case.

See the page :doc:`Modules` to pick the ones you need.

**Important note for RedHat / Fedora / Rocky users**

If you are using Fedora, Redhat8, please download the package **Linux_RedHat** which contains GDAL bindings in a different folder than the standard Linux package.
It is because the default system Python user site in RedHat, and distributions based on it, is lib/python3.8/site-packages and in ubuntu/debian it is lib/python3/dist-packages

Recommended Installation : One package containing all the modules 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
With OTB 9 the packaging is made by CMake to deliver a unique self extractible tar.gz to users. All you have to do is downloading the **OTB-9.0.0-Linux.tar.gz** package.

You can download the package from the website and extract it in your file manager, or from command line :

.. code-block:: bash

   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-9.0.0-Linux.tar.gz -o /Path/To/Downloads/OTB-9.0.0-Linux.tar.gz
   tar xf /Path/To/Downloads/OTB-9.0.0-Linux.tar.gz --one-top-level=/Path/To/OTB_install
   source /Path/To/OTB_install/otbenv.profile

Advanced Installation : Modular installation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Download
````````

In order to run OTB you will need the **OTB-Dependencies-9.0.tar.gz** package to run the Core **AND** optional packages that you can install afterwards.
Let's say you want to start using OTB only with the Core applications, and some months later you realize that you need to do more specific operations such as Learning. 
In that case you will just need to download the OTB-Learning package, and untar it where you installed the Core package. 
You can then use directly the applications packaged in the Learning package alongside the other Core apps.

.. code-block:: bash

   # Download mandatory packages to run OTB
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-9.0.0-Linux-Core.tar.gz -o OTB-9.0.0-Linux-Core.tar.gz
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-9.0.0-Linux-Dependencies.tar.gz -o OTB-9.0.0-Linux-Dependencies.tar.gz
   # Download optional packages
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-9.0.0-Linux-FeaturesExtraction.tar.gz -o OTB-9.0.0-Linux-FeaturesExtraction.tar.gz
   curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-9.0.0-Linux-Learning.tar.gz -o OTB-9.0.0-Linux-Learning.tar.gz
   ...

Installation
````````````

These packages are self-extractable tar.gz archives. You may uncompress the files with a
right-click => Extract to => create OTB-|release| folder and click OK, or from the command line as follows:

.. code-block:: bash

   # Install each tar gz in the same "top level" folder
   tar xvf OTB-9.0.0-Linux-Core.tar.gz --one-top-level="/Path/To/Install/OTB"
   tar xvf OTB-9.0.0-Linux-FeaturesExtraction.tar.gz --one-top-level="/Path/To/Install/OTB"
   # It is necessary to install the dependencies AFTER the other modules*
   tar xvf OTB-9.0.0-Linux-Dependencies.tar.gz --one-top-level="/Path/To/Install/OTB"
   ...
   source /Path/To/Install/OTB/otbenv.profile

Be careful to install the dependencies *AFTER* the modules because the paths in the cmake files are made modular only when you install
the dependencies : the resulting installation can be moved elsewhere on the disk. See the section "move installation below"

Installation folder description
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

If you want to use the otbcli launchers, you can initialize your
environment with ``source otbenv.profile``.

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
managers or at [https://cmake.org/]). Make sure you installed the necessary dependencies in the :doc:`First_Steps` page

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

Moving your installed OTB
~~~~~~~~~~~~~~~~~~~~~~~~~

With OTB 9 one can move the installation folder, but once it is done, there is a step to do to ensure the paths are correct.

.. code-block:: bash

   rm /Path/To/Moved/OTB/tools/install_done.txt
   source /Path/To/Moved/OTB/otbenv.profile
   # At this time a message will be displayed showing that this is a new installation, this is normal
