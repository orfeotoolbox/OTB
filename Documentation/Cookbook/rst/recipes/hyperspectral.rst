Hyperspectral image processing
==============================


Unmixing
--------

.. figure:: ../Art/HyperspectralImages/cuprite_rgb.png

   Cuprite image considered, here bands 16, 100 and 180 are displayed.



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

.. figure:: ../Art/HyperspectralImages/pavia.png
   :width: 70%

   Hyperspectral image of the Pavia university.

::

    otbcli_DimensionalityReduction  -in ${PAVIA}
                                    -out reducedData.tif
                                    -method pca
                                    -nbcomp 10

::

    otbcli_LocalRxDetection  -in reducedData.tif
                             -out RxScore.tif
                             -ir 1
                             -er 5

::

    otbcli_BandMath  -il RxScore.tif
                     -out anomalyMap.tif
                     -exp "im1b1>100"



.. |image_1| image:: ../Art/HyperspectralImages/rx_score.png

.. |image_2| image:: ../Art/HyperspectralImages/rx_detection.png


.. _Figure1:

+---------------------------+---------------------------+
|        |image_1|          |         |image_2|         |
+---------------------------+---------------------------+

   Left: Computed Rx score, right: detected anomalies (in red)


