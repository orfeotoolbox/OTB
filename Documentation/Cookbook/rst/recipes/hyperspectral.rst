Hyperspectral image processing
==============================


.. figure:: ../Art/HyperspectralImages/cuprite_rgb.png

   Cuprite image considered, here bands 16, 100 and 180 are displayed.



Unmixing
--------

::

    otbcli_EndmemberNumberEstimation  -in inputImage
                                      -algo vd 
                                      -algo.vd.far 1e-5


::

    otbcli_VertexComponentAnalysis  -in inputImage
                                    -ne 19 
                                    -outendm endmembers.tif

::

    otbcli_HyperspectralUnmixing  -in inputImage 
                                  -ie endmembers.tif 
                                  -out unmixedImage.tif
                                  -ua ucls

.. figure:: ../Art/HyperspectralImages/hyperspectralUnmixing_rgb.png

   Resulting unmixed image, here the first three bands are displayed.


Anomaly detection
-----------------
