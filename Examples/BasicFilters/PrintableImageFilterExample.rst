Most of the time, satellite images have more than three spectral bands. As we
are only able to see three colors (red, green and blue), we have to find a way
to represent these images using only three bands. This is called creating a
color composition.

Of course, any color composition will not be able to render all the information
available in the original image. As a consequence, sometimes, creating more than
one color composition will be necessary.

If you want to obtain an image with natural colors, you have to match the
wavelength captured by the satellite with those captured by your eye: thus
matching the red band with the red color, etc.

Some satellites (SPOT 5 is an example) do not acquire all the visible
spectral bands: the blue can be missing and replaced by some other wavelength of
interest for a specific application.  In these situations, another mapping has
to be created. That's why, the vegetation often appears in red in satellite
images.

The band order in the image products can be also quite tricky. It could be in
the wavelength order, as it is the case for Quickbird (1: Blue, 2: Green, 3:
Red, 4: NIR), in this case, you have to be careful to reverse the order if you
want a natural display. It could also be reverse to facilitate direct viewing,
as for SPOT5 (1: NIR, 2: Red, 3: Green, 4: SWIR) but in this situations you have
to be careful when you process the image.

.. |image1| image:: /Output/PrintableExampleOutput1.jpg

.. |image2| image:: /Output/PrintableExampleOutput2.jpg

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

On the left, a classic SPOT5 combination: XS3 in red, XS2 in green and XS1 in blue. On the right another composition: XS3 in red, XS4 in green and XS2 in blue.
