This example illustrates the class :doxygen:`MeanDifferenceImageFilter` for detecting changes
between pairs of images. This filter computes the mean intensity in
the neighborhood of each pixel of the pair of images to be compared
and uses the difference of means as a change indicator.
These correspond to the near
infrared band of two Spot acquisitions before and during a flood.

.. |image1| image:: /Input/SpotBefore.png

.. |image2| image:: /Input/SpotAfter.png

.. |image3| image:: /Output/DiffChDet.png

.. _Figure1:

+--------------------------+-------------------------+-------------------------+
|        |image1|          |         |image2|        |         |image3|        |
+--------------------------+-------------------------+-------------------------+

    Spot images for change detection. Left: Before the flood. Middle: during the
    flood. Right: Result of the mean difference change detector.
