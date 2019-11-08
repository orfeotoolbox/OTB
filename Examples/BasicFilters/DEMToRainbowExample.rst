In some situation, it is desirable to represent a gray scale image in color for easier
interpretation. This is particularly the case if pixel values in the image are used
to represent some data such as elevation, deformation map,
interferogram. In this case, it is important to ensure that similar
values will get similar colors. You can notice how this requirement
differs from the previous case.

The following example illustrates the use of the :doxygen:`DEMToImageGenerator`
class combined with the `ScalarToRainbowRGBPixelFunctor`. You can refer to the
source code for the DEM conversion to image, we will focus on the color
conversion part here.

.. |image1| image:: /Output/DEMToRainbowImageGenerator.png

.. |image2| image:: /Output/DEMToHotImageGenerator.png

.. |image3| image:: /Output/DEMToReliefImageGenerator.png

.. _Figure1:

+--------------------------+-------------------------+-------------------------+
|        |image1|          |         |image2|        |         |image3|        |
+--------------------------+-------------------------+-------------------------+
