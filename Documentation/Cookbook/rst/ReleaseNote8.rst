What is new with OTB 8.0
========================

OSSIM is used for geometric **sensor modelling** and **metadata
parsing**. It has been a dependency of the OTB since its
beginning. Then adapter classes have been added to hide OSSIM headers
from OTB public API. With the version 8 of OTB, it is time to remove
this dependency, whose development cycle is difficult to follow. Until
OTB v7, only a small portion of OSSIM was used anyway.

A new Metadata framework
------------------------

Reading and writing the metadata used to be handled by OSSIM. With OTB
8.0, GDAL is in charge for this task. Like with OSSIM, this framework
has a mechanism to parse the metadata read by GDAL and make them
accessible to the applications. The list of supported sensors didn’t
change from the previous versions. Sensors not in this list can still
be used with OTB, but the metadata won’t be accessible.

Formosat
^^^^^^^^

Fomosat products are based on the DIMAP file format. The user can
provide either the DIMAP file or the tiff file as input of an OTB
application.

Ikonos
^^^^^^

Ikonos products use the TIFF file format. To use an Ikonos product,
the user should provide one of them as input for an OTB application.

Pleiades
^^^^^^^^

Pleiades products are based on the DIMAP file format. The user can
provide either the DIMAP file or the tiff/jpeg2000 file as input of an
OTB application. We recommend the use of the tiff/jp2000 file. Indeed,
when reading the DIMAP file we noticed that the image is shifted by
0.5 pixel (this is a documented bug in GDAL).

QuickBird
^^^^^^^^^

QuickBird products use the TIFF file format. To use a QuickBird
product, the user should provide one of them as input for an OTB
application.

Spot 5, 6 & 7
^^^^^^^^^^^^^

Spot products are based on the DIMAP file format. The user can provide
either the DIMAP file or the tiff/jpeg2000 file as input of an OTB
application.

WorldView 2
^^^^^^^^^^^

WordView 2 products use the TIFF file format. To use a WordView 2
product, the user should provide one of them as input for an OTB
application.

CosmoSkyMed
^^^^^^^^^^^

CosmoSkyMed products contain either HDF5 files or Tiff files. One Tiff
file per polarization, or one HDF5 file containing all the data. When
reading a HDF dataset, one needs to select the right subdataset using
the Extended Filename ``&sdataidx=<(int)idx>``. For example, in some
CosmoSkyMed products, the first subDataset is a quicklook, and the
actual product is the second subdataset:

.. code-block:: bash

    -in “<PRODUCT_NAME>.h5?&sdataidx=2”

Radarsat 2
^^^^^^^^^^

Radarsat2 products uses the TIFF file format. To use a Radarsat
product, the user should provide one of them as input for an OTB
application.

Sentinel 1
^^^^^^^^^^

Sentinel 1 products use the TIFF file format. To use a Sentinel 1
product, the user should provide one of them as input for an OTB
application.

TerraSarX
^^^^^^^^^

TerraSarX products contains COS files. To use a TerraSarX product, the
user should provide one of them as input for an OTB application.

A new organization for the Sensor Models
----------------------------------------

OSSIM oversaw the forward and inverse transforms. It would use either
an RPC model, a SAR model, or a sensor specific model.

We now use GDAL’s RPC model in OTB. After running some tests, we
estimated that it was more accurate than OSSIM’s. Some localization
gaps of the order of 0.01 pixel are to be expected.

We implemented a new SAR model for OTB, based on OSSIM’s. The unique,
significant, difference is the way it deals with time. OSSIM uses a
double precision to record times. The time needs to be accurate at the
microsecond, and a double doesn’t allow such accuracy. When working on
one line, this is not a problem, but the accumulation of errors on
multiple lines generates a significant error which can lead to the
selection of the wrong line. To fix this, we used another time library
that is more precise. This means the new version of OTB provides
results slightly better: a few lines will have a different value.

A new way to handle DEM
-----------------------

Once again, OSSIM is replaced by GDAL for this task. Many OTB
application use an elevation model as input, usually with the
parameter “-elev”. This parameter accepts any raster file supported by
GDAL, or a directory containing such files. In this second case, all
rasters from the input directory will be opened by GDAL, so it could
be a good idea to use a VRT. It is the same for the geoid file, but it
is important to note that in previous OTB versions (using OSSIM) it
was common to use the egm96.grd file as geoid. This file cannot be
opened by GDAL. However, it is still possible to use it with a little
trick described in `the documentation
<https://www.orfeo-toolbox.org/CookBook-8.0/Supported_Sensors.html#a-note-on-the-egm96-grd-file>`_.

Status of the tests
-------------------

The continuous integration platform executed all the tests on the
release 8.0.0-alpha2, and they all exited with success.

Beside the automatic, non-regression tests, we also run functional
tests with products and application commonly used. We were
particularly cautious in testing applications related to sensor models
and DEM, like the orthorectification. To perform the tests, we used
Pleiades products to perform some treatments (calibration and
orthorectification). We also used multiple TerraSAR-X and Sentinel-1
products to perform various treatments (deburst, calibration and
orthorectification) with different parameters. This test helped us
identify some variations with the previous version: the results are
more accurate, but they take more computation time. We will describe
these differences in the following sections.

Numerical gaps with the previous version
----------------------------------------

Optical Calibration
^^^^^^^^^^^^^^^^^^^

No difference is to be expected, except for Pleiade products. Indeed,
we improved the calibration for the Pleiade products by changing the
position of the pixel used to take the sun angles from top center to
center. See this thread on the forum for more details. This leads to a
numerical gap of the order of 10e-4 in absolute.

Optical Orthorectification
^^^^^^^^^^^^^^^^^^^^^^^^^^

We studied the disparity between an orthorectification generated with
OTB 7.4 and an orthorectification generated with OTB 8.0 with a
Pleiades product. The analyses have shown a disparity of 0.01 pixel
along the row and 0.02 pixel along the columns. These disparities are
negligible compared to Pleiade’s localization precision.

SAR Deburst
^^^^^^^^^^^

Our analyses have shown that with OTB 8.0, the problem of line
selection due to nanosecond error accumulation is solved. Thus, the
lines that were miss selected by OSSIM are now correctly
selected. This means that a couple of lines will be different with the
new version of OTB. Except for those lines, no difference was
assessed.

SAR Calibration
^^^^^^^^^^^^^^^

This application doesn’t produce numerical gaps. However, if the
calibration is directly processed after deburst, the lines mis
selected during the deburst are still different.

SAR Orthorectification
^^^^^^^^^^^^^^^^^^^^^^

We studied the disparity between an orthorectification generated with
OTB 7.4 and an orthorectification generated with OTB 8.0 with a
Sentinel1 SLC product. The analyses have shown a disparity of 0.07
pixel along the row and 0.025 pixel along the columns. This is within
the precision of the measurements.

Difference of computation time
------------------------------

Calibration
^^^^^^^^^^^

We noticed a gain of processing time (about 15% for optical
calibration and about 66% for SAR calibration). This is due to a
better use of the calibration LUT.

Orthorectification
^^^^^^^^^^^^^^^^^^

There is a loss of processing time (about 40%) for this
application. We are currently investigating the cause. We think it
comes from the new time library.

Removed functionalities
-----------------------

DownloardSRTMTiles
^^^^^^^^^^^^^^^^^^

The DownloadSRTMTiles application was not functional anymore because
the official API used to retrieve the tiles is not usable without
authentication anymore. After some discussions, the decision was taken
to remove this application from OTB. The task performed by this
application can easily be done by a simple python script, there is no
need for an OTB application.

What is missing for the release of OTB 8.0?
-------------------------------------------

The version OTB-8.0.0-alpha2 was released on November 8th, 2021.  We
need more testing to release the final version 8.0. Please, don’t
hesitate to use OTB 8.0 alpha2, and tell us if you have some issues
with the application you usually use.

In the meantime, we will work on the optimization of the computation
time for Orthorectification. We will also run some more tests.

The release of OTB 8.0 will be largely discussed during the OTB User
Days that will be held in Toulouse, France from November 29th to
December 1st.
