Frequently Asked Questions
==========================

What's in OTB?
--------------

-  Image access: optimized read/write access for most of remote sensing
   image formats, meta-data access, simple visualization;

-  Sensor geometry: sensor models, cartographic projections;

-  Radiometry: atmospheric corrections, vegetation indices;

-  Filtering: blurring, denoising, enhancement;

-  Fusion: image pansharpening;

-  Feature extraction: interest points, alignments, lines;

-  Image segmentation: region growing, watershed, level sets;

-  Classification: K-means, SVM, Markov random fields;

-  Change detection.

-  Object based image analysis.

-  Geospatial analysis.

For a full list of applications see the chapter :ref:`apprefdoc`.
For an introduction to the C++ API see the
`Software Guide <https://www.orfeo-toolbox.org/SoftwareGuide/>`_.
And for an exhaustive description of the C++ API see the
`Doxygen <https://www.orfeo-toolbox.org/doxygen/>`_.


License
-------

What is OTB's license?
~~~~~~~~~~~~~~~~~~~~~~

OTB is distributed under the permissive open source license Apache
v2.0 - aka Apache Software License (ASL) v2.0: http://www.apache.org/licenses/LICENSE-2.0

Am I forced to distribute my code based on OTB?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

No. The license gives you the option to distribute your application if
you want to. You do not have to exercise this option in the license.

Am I forced to contribute my code based on OTB into the official repo?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

No.

If I wanted to distribute an application using OTB what license would I need to use?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The license of your choice. The OTB license only requires you to include
a copy of the Apache license and to provide a clear attribution to the
OTB project in any distribution including a piece of OTB software.

I am a commercial user. Is there any restriction on the use of OTB?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

No. The OTB license only requires you to include a copy of the Apache
license and to provide a clear attribution to the OTB project in any
distribution including a piece of OTB software.


Using OTB
---------

What is the image size limitation of OTB ?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The maximum physical space a user can allocate depends on her platform.
Therefore, image allocation in OTB is restricted by image dimension,
size, pixel type and number of bands.

Fortunately, thanks to the streaming mechanism implemented within OTB’s
pipeline (actually ITK’s), this limitation can be bypassed. The use of
the at the end of the pipeline, will seamlessly break the large,
problematic data into small pieces whose allocation is possible. These
pieces are processed one after the other, so that there is not
allocation problem anymore. We are often working with images of
:math:`25000 \times 25000` pixels.

For the streaming to work, all the filters in the pipeline must be
streaming capable (this is the case for most of the filters in OTB). The
output image format also need to be streamable (not PNG or JPEG, but
TIFF or ENVI formats, for instance).

The class manage the steaming process following two strategies: by tile
or by strip. Different size configuration for these two strategies are
available into the interface. The default mode use the information about
how the file is streamed on the disk and will try to minimize the memory
consumption along the pipeline. More information can be found into the
documentation of the class.

Problems using OTB python wrapping along with other software
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you use OTB standalone binaries, there should not be any dependency conflict
with other libraries installed on your system. OTB will always try to grab
supplied libraries in the standalone package.

However, when using Python wrappings, there can be conflicts if you import
*otbApplication* along with other software that share common dependencies with
OTB. For instance, if you want to use OTB Applications and Fiona in a Python
script, they both rely on GDAL library. As the libraries loaded by Python must
be unique, the first library *SomeLib* loaded will be used by any other binary
depending on it. Thus, the order of the imports has an effect. In some cases,
symbol problems have been observed in libcrypto, and the solution was to import
OTB Applications before importing Fiona.
