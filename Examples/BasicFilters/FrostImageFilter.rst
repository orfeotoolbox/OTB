This example illustrates the use of the :doxygen:`FrostImageFilter`.
This filter belongs to the family of the edge-preserving smoothing
filters which are usually used for speckle reduction in radar
images.

This filter uses a negative exponential convolution kernel.
The output of the filter for pixel p is:

.. math::

   \hat I_{s}=\sum_{p\in\eta_{p}} m_{p}I_{p}

   m_{p}=\frac{KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})}{\sum_{p\in\eta_{p}} KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})}

   d_{s, p}=\sqrt{(i-i_{p})^2+(j-j_{p})^2}

where:

* :math:`K`: the decrease coefficient
* :math:`(i, j)`: the coordinates of the pixel inside the region defined by :math:`\eta_{s}`
* :math:`(i_{p}, j_{p})`: the coordinates of the pixels belonging to :math:`\eta_{p} \subset \eta_{s}`
* :math:`C_{s}`: the variation coefficient computed over :math:`\eta_{p}`

.. |image1| image:: /Input/GomaSmall.png

.. |image2| image:: /Output/GomaSmallFrostFiltered.png

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    Result of applying the FrostImageFilter to a SAR image.
