In order to run the OTB applications, you have to make sure you installed some necessary dependencies on your system if you plan to use the Python interface
or the graphical version of the apps

System dependencies to run the standalone package
`````````````````````````````````````````````````

OTB 9 has only a command line interface, which exposes python bindings compiled with Python 3.8. You need to have some dependencies on your system
to use OTB with your python version.

Debian / Ubuntu
+++++++++++++++
.. code-block:: bash

  # Required packages to extract OTB from the archive and recompile the python bindings
  apt update && apt install -y --no-install-recommends g++ swig cmake make python3 python3-dev python3-numpy

Download OTB
`````````````

OTB9 is composed of a Core package and optional modules. Depending on your needs, you can only use the Core package and its dependencies.
On this simple use case, we will use only the Core package.

.. code-block:: bash
    curl otb-depends-core.tar.gz -o /Path/You/Want/otb-deps.tar.gz
    curl otb-core.tar.gz -o /Path/You/Want/otb-core.tar.gz

Installation
++++++++++++

.. code-block:: bash

   # Install otb
   cd /Path/You/Want/
   for f in *.tar.gz; do tar xvf "$f" --one-top-level="OTB-9.0"; done
   # Recompile python bindings
   cd OTB-9.0
   source otbenv.profile
   sh recompile-bindings.sh

Simple OTB use case : Compute NDVI
```````````````````````````````````

Download an example image
+++++++++++++++++++++++++

.. code-block:: bash

    curl https://www.orfeo-toolbox.org/download/data_example/xt_PHR.tif -o /Path/You/Want/xt_PHR.tif

Compute NDVI from red and Nir bands
+++++++++++++++++++++++++++++++++++

.. code-block:: bash

    otbcli_BandMath –il /Path/You/Want/xt_PHR.tif –out ndvi.tif –exp “(im1b4-im1b1)/ (im1b4+im1b1)”
