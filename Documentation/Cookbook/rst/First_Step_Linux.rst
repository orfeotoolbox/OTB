On this page, we describe a simple use case for OTB that uses only the Core package.

System dependencies to run the standalone package
`````````````````````````````````````````````````

OTB 9 has only a command line interface, which is composed of C++ written applications. The toolbox also provides python bindings compiled with Python 3.8. 
You need to have some dependencies on your system to use OTB in both cases.

Debian / Ubuntu
+++++++++++++++
.. code-block:: bash

  # Required packages to extract OTB from the archive and recompile the python bindings
  apt update && apt install -y --no-install-recommends g++ swig cmake make python3 python3-dev python3-numpy

Simple OTB use case : Compute NDVI with the CLI interface
`````````````````````````````````````````````````````````

Download OTB
++++++++++++

.. code-block:: bash

    curl https://www.orfeo-toolbox.org/download/packages/OTB-Core-Dependencies-9.0.tar.gz -o /Path/Download/OTB-Core-Dependencies-9.0.tar.gz
    curl https://www.orfeo-toolbox.org/download/packages/OTB-Core-9.0.tar.gz -o /Path/Download/OTB-Core-9.0.tar.gz

Installation
++++++++++++

.. code-block:: bash

   # Install otb
   cd /Path/Download
   tar xvf OTB-Core-9.0.tar.gz --one-top-level="/Path/To/Install/OTB"
   tar xvf OTB-Core-Dependencies-9.0.tar.gz --one-top-level="/Path/To/Install/OTB"

Download an example image
+++++++++++++++++++++++++

.. code-block:: bash

    curl https://www.orfeo-toolbox.org/download/data_example/xt_PHR.tif -o /Path/You/Want/xt_PHR.tif

Compute NDVI using the CLI interface
+++++++++++++++++++++++++++++++++++

.. code-block:: bash

    otbcli_BandMath –il /Path/You/Want/xt_PHR.tif –out ndvi.tif –exp “(im1b4-im1b1)/ (im1b4+im1b1)”

Use Python API to compute NDVI
++++++++++++++++++++++++++++++++

To use the Python API you have first to recompile the bindings for your version of Python. By default they are compiled for Python 3.8

.. code-block:: bash

   # Recompile python bindings
   cd /Path/To/OTB-9.0
   source otbenv.profile
   sh recompile-bindings.sh

Then you can launch :

.. code-block:: bash

  python3
  import otbApplication as otb
  appbandmath = otb.Registry.CreateApplication("BandMath")
  appbandmath.SetParameterInputImage("/Path/You/Want/xt_PHR.tif")
  appbandmath.SetParameterString("exp",“(im1b4-im1b1)/ (im1b4+im1b1)”)
  appbandmath.SetParameterOutputImage("/Path/You/Want/ndvi.tif")
  appbandmath.ExecuteAndWriteOutput()
