This example illustrates the class
:doxygen:`MultivariateAlterationDetectorImageFilter`,
which implements the Multivariate Alteration Change Detector
algorithm. This algorihtm allows
performing change detection from a pair multi-band images, including
images with different number of bands or modalities. Its output is
a multi-band image of change maps, each one being unccorrelated
with the remaining. The number of bands of the output image is the
minimum number of bands between the two input images.

The algorithm works as follows. It tries to find two linear
combinations of bands (one for each input images) which maximize
correlation, and subtract these two linear combinitation, leading
to the first change map. Then, it looks for a second set of linear
combinations which are orthogonal to the first ones, a which
maximize correlation, and use it as the second change map. This
process is iterated until no more orthogonal linear combinations
can be found.

This algorithms has numerous advantages, such as radiometry scaling
and shifting invariance and absence of parameters, but it can not
be used on a pair of single band images (in this case the output is
simply the difference between the two images).

.. |image1| image:: /Output/mad-input2.png

.. |image2| image:: /Output/mad-output.png

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    Result of the Multivariate Alteration Detector results on SPOT5 data before and after flooding.
