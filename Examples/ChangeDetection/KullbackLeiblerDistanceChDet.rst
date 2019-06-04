This example illustrates the class
:doxygen:`KullbackLeiblerDistanceImageFilter` for detecting changes
between pairs of images. This filter computes the Kullback-Leibler
distance between probability density functions (pdfs).
In fact, the Kullback-Leibler distance is itself approximated through
a cumulant-based expansion, since the pdfs are approximated through an
Edgeworth series.

The Kullback-Leibler distance is evaluated by:

.. math::

   K_{\text{Edgeworth}}(X_1 | X_2) = \frac{1}{12} \frac{\kappa_{X_1; 3}^2}{\kappa_{X_1; 2}^2}
       + \frac{1}{2} \left( \log \frac{\kappa_{X_2; 2}}{\kappa_{X_1; 2}}
                            -1+\frac{1}{\kappa_{X_2; 2}}
                            \left( \kappa_{X_1; 1} - \kappa_{X_2; 1} +  \kappa_{X_1; 2}^{1/2} \right)^2
                       \right)
       - \left( \kappa_{X_2; 3} \frac{a_1}{6} + \kappa_{X_2; 4} \frac{a_2}{24}
           + \kappa_{X_2; 3}^2 \frac{a_3}{72} \right)
       - \frac{1}{2} \frac{ \kappa_{X_2; 3}^2}{36}
           \left(
               c_6 - 6 \frac{c_4}{\kappa_{X_1; 2}} + 9 \frac{c_2}{\kappa_{X_2; 2}^2}
           \right)
       - 10 \frac{\kappa_{X_1; 3} \kappa_{X_2; 3}
                       \left( \kappa_{X_1; 1} - \kappa_{X_2; 1} \right)
                       \left( \kappa_{X_1; 2} - \kappa_{X_2; 2} \right)}{\kappa_{X_2; 2}^6} \qquad

where:

.. math::

   a_1 = c_3 - 3 \frac{\alpha}{\kappa_{X_2; 2}}
   a_2 = c_4 - 6 \frac{c_2}{\kappa_{X_2; 2}} + \frac{3}{\kappa_{X_2; 2}^2}
   a_3 = c_6 - 15\frac{c_4}{\kappa_{X_2; 2}} + 45\frac{c_2}{\kappa_{X_2; 2}^2} - \frac{15}{\kappa_{X_2; 2}^3}
   c_2 = \alpha^2 + \beta^2
   c_3 = \alpha^3 + 3 \alpha \beta^2
   c_4 = \alpha^4 + 6 \alpha^2 \beta^2 + 3 \beta^4
   c_6 = \alpha^6 + 15\alpha^4 \beta^2 + 45 \alpha^2 \beta^4 + 15 \beta^6
   \alpha = \frac{\kappa_{X_1; 1} - \kappa_{X_2; 1}}{\kappa_{X_2; 2}}
   \beta = \frac{ \kappa_{X_1; 2}^{1/2} }{\kappa_{X_2; 2}}.

:math:`\kappa_{X_i; 1}`, :math:`\kappa_{X_i; 2}`, :math:`\kappa_{X_i; 3}` and :math:`\kappa_{X_i; 4}`
are the cumulants up to order 4 of the random variable :math:`X_i`.
These correspond to 2 Radarsat fine
mode acquisitions before and after a lava flow resulting from a
volcanic eruption.

The program itself is very similar to the ratio of means detector,
implemented in :doxygen:`MeanRatioImageFilter`.
Nevertheless the corresponding header file has to be used instead.

.. |image1| image:: /Input/GomaAvant.png

.. |image2| image:: /Input/GomaApres.png

.. |image3| image:: /Output/KLdistanceChDet.png

.. _Figure1:

+--------------------------+-------------------------+-------------------------+
|        |image1|          |         |image2|        |         |image3|        |
+--------------------------+-------------------------+-------------------------+

    Result of the Kullback-Leibler change detector

