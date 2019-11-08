This example illustrates the use of the :doxygen:`LeeImageFilter`.
This filter belongs to the family of the edge-preserving smoothing
filters which are usually used for speckle reduction in radar
images. The LeeFilter aplies a linear regression
which minimizes the mean-square error in the frame of a
multiplicative speckle model.

.. |image1| image:: /Input/GomaSmall.png

.. |image2| image:: /Output/GomaSmallLeeFiltered.png

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    Result of applying the Lee filter to a SAR image.

