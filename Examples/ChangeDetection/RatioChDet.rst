This example illustrates the class
:doxygen:`MeanRatioImageFilter` for detecting changes
between pairs of images. This filter computes the mean intensity in
the neighborhood of each pixel of the pair of images to be compared
and uses the ratio of means as a change indicator. This change
indicator is then normalized between 0 and 1 by using the classical

.. math::

    r = 1 - min\{\frac{\mu_A}{\mu_B},\frac{\mu_B}{\mu_A} \}

where :math:`\mu_A` and :math:`\mu_B` are the local means.

.. |image1| image:: /Input/GomaAvant.png

.. |image2| image:: /Input/GomaApres.png

.. |image3| image:: /Output/RatioChDet.png

.. _Figure1:

+--------------------------+-------------------------+-------------------------+
|        |image1|          |         |image2|        |         |image3|        |
+--------------------------+-------------------------+-------------------------+

    Radarsat fine mode acquisitions before (left) and after (middle) a lava flow
    resulting from a volcanic eruption. On the right is the result of the change
    detection by ratio of local means.

