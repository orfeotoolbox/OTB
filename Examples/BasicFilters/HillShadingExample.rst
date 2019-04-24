Visualization of digital elevation models (DEM) is often more intuitive by
simulating a lighting source and generating the corresponding shadows. This
principle is called hill shading.

Using :doxygen:`HillShadingFilter` and the DEM image generated
using the :doxygen:`DEMToImageGenerator`, you can easily obtain a representation
of the DEM. Better yet, using the :doxygen-itk:`ScalarToRGBColormapImageFilter`
combined with the ``ReliefColormapFunctor`` you can easily generate the
classic elevation maps.

This example will focus on the shading itself.

.. |image1| image:: /Output/HillShadingExample.png

.. |image2| image:: /Output/HillShadingColorExample.png

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    Hill shading obtained from SRTM data (left) and combined with the color representation (right)
