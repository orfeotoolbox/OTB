On this page, we describe a simple use case for OTB that uses only the Core and FeaturesExtraction package.

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

Download and install OTB
++++++++++++++++++++++++

.. code-block:: bash

    bash -c "$(curl -fsSL https://orfeo-toolbox.org/packages/install-otb.sh)" -s 9.1.0 $HOME/otb

Download an example image
+++++++++++++++++++++++++

.. code-block:: bash

    curl https://www.orfeo-toolbox.org/packages/examples/phr_xs.tif -o $HOME/Data/phr_xs.tif

Compute NDVI using the CLI interface
++++++++++++++++++++++++++++++++++++

.. code-block:: bash

    source $HOME/otb/otbenv.profile
    otbcli_BandMath –il $HOME/Data/phr_xs.tif –out $HOME/Data/ndvi.tif –exp “(im1b4-im1b1)/ (im1b4+im1b1)”

Use Python API to compute NDVI
++++++++++++++++++++++++++++++

To use the Python API you have first to recompile the bindings for your version of Python if this version is different than 3.10.

.. code-block:: bash

   # Recompile python bindings
   cd $HOME/otb
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
