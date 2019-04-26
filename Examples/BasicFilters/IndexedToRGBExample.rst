Some algorithms produce an indexed image as output. In such images,
each pixel is given a value according to the region number it belongs to.
This value starting at 0 or 1 is usually an integer value.
Often, such images are produced by segmentation or classification algorithms.

If such regions are easy to manipulate -- it is easier and faster to compare two integers
than a RGB value -- it is different when it comes to displaying the results.

Here we present a convient way to convert such indexed image to a color image. In
such conversion, it is important to ensure that neighboring regions, which are
likely to have consecutive number have easily dicernable colors. This is done
randomly using a hash function by ``ScalarToRGBPixelFunctor``.

.. |image1| image:: /Output/buildingExtractionIndexed_scaled.png

.. |image2| image:: /Output/buildingExtractionRGB.png

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    The original indexed image (left) and the conversion to color image.
