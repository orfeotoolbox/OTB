This example demonstrates the use of the :doxygen:`MeanShiftSegmentationFilter`
class which implements filtering and clustering using the mean shift algorithm.
For a given pixel, the mean shift will build a set of neighboring pixels within
a given spatial radius and a color range. The spatial and color center of this
set is then computed and the algorithm iterates with this new spatial and color
center. The Mean Shift can be used for edge-preserving smoothing, or for
clustering.

.. |image1| image:: /Input/ROI_QB_MUL_1.png

.. |image2| image:: /Output/MSClusteredOutput-pretty.png

.. |image3| image:: /Output/MSLabeledOutput-pretty.png

.. _Figure1:

+--------------------------+-------------------------+-------------------------+
|        |image1|          |         |image2|        |         |image3|        |
+--------------------------+-------------------------+-------------------------+

    Original image, image filtered by mean shift after clustering, and labeled image.
