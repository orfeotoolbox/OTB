The :doxygen:`BandMathImageFilter` is based on the mathematical parser library muParser.
The built in functions and operators list is available at:
http://muparser.sourceforge.net/mup_features.html.

In order to use this filter, at least one input image should be
set. An associated variable name can be specified or not by using
the corresponding ``SetNthInput`` method. For the nth input image, if
no associated variable name has been specified, a default variable
name is given by concatenating the letter "b" (for band) and the
corresponding input index.

The next step is to set the expression according to the variable
names. For example, in the default case with three input images the
following expression is valid: ``(b1+b2)*b3``.

.. |image1| image:: /Input/qb_ExtractRoad_pretty.png

.. |image2| image:: /Output/qb_BandMath-pretty.jpg

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    NDVI of a Quickbird image computed with BandMathImageFilter

