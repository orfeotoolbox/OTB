From raw image to calibrated product
====================================

This section presents various pre-processing tasks that are presented in
a classical order to obtain a calibrated, pan-sharpened image.

Optical radiometric calibration
-------------------------------

In remote sensing imagery, pixel values are called DN (for Digital
Numbers) and can not be physically interpreted and compared: they are
influenced by various factors such as the amount of light flowing trough
the sensor, the gain of the detectors and the analogic to numeric
converter.

Depending on the season, the light and atmospheric conditions, the
position of the sun or the sensor internal parameters, these DN can
drastically change for a given pixel (apart from any ground change
effects). Moreover, these effects are not uniform over the spectrum: for
instance aerosol amount and type has usually more impact on the blue
channel.

Therefore, it is necessary to calibrate the pixel values before any
physical interpretation is made out of them. In particular, this
processing is mandatory before any comparison of pixel spectrum between
several images (from the same sensor), and to train a classifier without
dependence to the atmospheric conditions at the acquisition time.

Calibrated values are called surface reflectivity, which is a ratio
denoting the fraction of light that is reflected by the underlying
surface in the given spectral range. As such, its values lie in the
range :math:`[0,1]`. For convenience, images are often stored in
thousandth of reflectivity, so that they can be encoded with an integer
type. Two levels of calibration are usually distinguished:

-  The first level is called *Top Of Atmosphere (TOA)* reflectivity. It
   takes into account the sensor gain, sensor spectral response and the
   solar illumination.

-  The second level is called *Top Of Canopy (TOC)* reflectivity. In
   addition to sensor gain and solar illumination, it takes into account
   the optical thickness of the atmosphere, the atmospheric pressure,
   the water vapor amount, the ozone amount, as well as the composition
   and amount of aerosol gasses.

This transformation can be done either with or with . Sensor-related
parameters such as gain, date, spectral sensitivity and sensor position
are seamlessly read from the image metadata. Atmospheric parameters can
be tuned by the user. Supported sensors are :

-  Pleiades,

-  SPOT5,

-  QuickBird,

-  Ikonos,

-  WorldView-1,

-  WorldView-2,

-  Formosat.

Optical calibration with 
~~~~~~~~~~~~~~~~~~~~~~~~~

The application allows to perform optical calibration. The mandatory
parameters are the input and output images. All other parameters are
optional. By default the level of calibration is set to TOA (Top Of
Atmosphere). The output images are expressed in thousandth of
reflectivity using a 16 bits unsigned integer type.

A basic TOA calibration task can be performed with the following command
:

::

    otbcli_OpticalCalibration -in  input_image -out output_image

A basic TOC calibration task can be performed with the following command
:

::

    otbcli_OpticalCalibration -in  input_image -out output_image -level toc

Optical calibration with 
~~~~~~~~~~~~~~~~~~~~~~~~~

These transformations can also be done in .

The 6S model needs atmospheric parameters to be able to compute
radiative terms to estimate the atmospheric contributions on the input
signal. Default parameters are available in the module. For atmospheric
parameters, it is possible to indicate AERONET file. The AERONET
(AErosol RObotic NETwork) program is a federation of ground-based remote
sensing aerosol networks established by NASA and PHOTONS (Univ. of Lille
1, CNES, and CNRS-INSU) and is greatly expanded by collaborators from
national agencies, institutes, universities, individual scientists, and
partners. The program provides accessible public domain database of
aerosol optical, mircrophysical and radiative properties.

The module produces four outputs:

-  Luminance image.

-  TOA reflectance image.

-  TOC reflectance image.

-  Difference TOA-TOC image, which allows to get the estimation of
   atmospheric contribution.

|image| [fig:opticalcalibration]

|image| [fig:opticalcalibrationoutput]

Pan-sharpening
--------------

Because of physical constrains on the sensor design, it is difficult to
achieve high spatial and spectral resolution at the same time : a better
spatial resolution means a smaller detector, which in turns means lesser
optical flow on the detector surface. On the contrary, spectral bands
are obtained through filters applied on the detector surface, that
lowers the optical flow, so that it is necessary to increase the
detector size to achieve an acceptable signal to noise ratio.

For these reasons, many high resolution satellite payload are composed
of two sets of detectors, which in turns delivers two different kind of
images :

-  The multi-spectral (XS) image, composed of 3 to 8 spectral bands
   containing usually blue, green, red and near infra-red bands at a
   given resolution (usually from 2.8 meters to 2 meters).

-  The panchromatic (PAN) image, which is a grayscale image acquired by
   a detector covering a wider part of the light spectrum, which allows
   to increase the optical flow and thus to reduce pixel size.
   Therefore, resolution of the panchromatic image is usually around 4
   times lower than the resolution of the multi-spectral image (from 46
   centimeters to 70 centimeters).

It is very frequent that those two images are delivered side by side by
data providers. Such a dataset is called a bundle. A very common remote
sensing processing is to fuse the panchromatic image with the
multi-spectral one so as to get an image combining the spatial
resolution of the panchromatic image with the spectral richness of the
multi-spectral image. This operation is called pan-sharpening.

This fusion operation requires two different steps :

#. The multi-spectral (XS) image is zoomed and registered to the
   panchromatic image,

#. A pixel-by-pixel fusion operator is applied to the co-registered
   pixels of the multi-spectral and panchromatic image to obtain the
   fused pixels.

Using either or modules from , it is possible to perform both steps in a
row, or step-by-step fusion, as described in the above sections.

Pan-sharpening with 
~~~~~~~~~~~~~~~~~~~~

The application allows to perform both steps in a row. Seamless sensor
modelling is used to perform zooming and registration of the
multi-spectral image on the panchromatic image. In the case of a
Pléiades bundle, a different approach is used : an affine transform is
used to zoom the multi-spectral image and apply a residual translation.
This translation is computed based on metadata about the geometric
processing of the bundle. This zooming and registration of the
multi-spectral image over the panchromatic image can also be performed
by the application.

After the registration step, a simple pan-sharpening is applied,
according to the following formula:

.. math:: PXS(i,j) = \frac{PAN(i,j)}{PAN_{smooth}(i,j)} \cdot XS(i,j)

Where :math:`i` and :math:`j` are pixels indices, :math:`PAN` is the
panchromatic image, :math:`XS` is the multi-spectral image and
:math:`PAN_{smooth}` is the panchromatic image smoothed with a kernel to
fit the multi-spectral image scale.

Here is a simple example of how to use the application:

::

    otbcli_BundleToPerfectSensor -inp pan_image -inxs xs_image -out output_image

There are also optional parameters that can be useful for this tool:

-  The ``-elev`` option allows to specify the elevation, either with a
   DEM formatted for OTB (``-elev.dem`` option, see section [ssec:dem])
   or with an average elevation (``-elev.default`` option). Since
   registration and zooming of the multi-spectral image is performed
   using sensor-models, it may happen that the registration is not
   perfect in case of landscape with high elevation variation. Using a
   DEM in this case allows to get better registration.

-  The ``-lmSpacing`` option allows to specify the step of the
   registration grid between the multi-spectral image and panchromatic
   image. This is expressed in amount of panchromatic pixels. A lower
   value gives a more precise registration but implies more computation
   with the sensor models, and thus increase the computation time.
   Default value is 10 pixels, which gives sufficient precision in most
   of the cases.

-  The ``-mode`` option allows to select the registration mode for the
   multi-spectral image. The ``default`` mode uses the sensor model of
   each image to create a generic “MS to Pan” transform. The ``phr``
   mode uses a simple affine transform (which doesn’t need an elevation
   source nor a registration grid).

Pan-sharpening is a quite heavy processing requiring a lot of system
resource. The ``-ram`` option allows you to limit the amount of memory
available for the computation, and to avoid overloading your computer.
Increasing the available amount of RAM may also result in better
computation time, seems it optimises the use of the system resources.
Default value is 256 Mb.

Pan-sharpening with 
~~~~~~~~~~~~~~~~~~~~

allows to perform step-by-step fusion. The followings screenshots
highlight operations needed to perform Pan-Sharpening.

-  Open panchromatic and multispectral images in monteverdi using the
   module or using the ``-il`` option of the executable.

-  The module is used to zoomed and registered the multispectral on the
   panchromatic image. As a result, we get a multispectral dataset with
   the same geographic extension and the same resolution as the
   panchromatic image, cf  [fig:qbmulsuper].

   |image| |image| [fig:qbmulsuper]

-  Now the module can be used using the panchromatic and the
   multispectral images as inputs. It produces a multispectral image
   with the same resolution and geographic extension (cf
    [fig:pansharpen]).

   |image| [fig:pansharpen]

Please also note that since registration and zooming of the
multi-spectral image with the panchromatic image relies on sensor
modelling, this tool will work only for images whose sensor models is
available in (see section [ssec:ortho] for a detailed list). It will
also work with ortho-ready products in cartographic projection.

Digital Elevation Model management
----------------------------------

A Digital Elevation Model (DEM) is a georeferenced image (or collection
of images) where each pixel corresponds to a local elevation. DEM are
useful for tasks involving sensor to ground and ground to sensor
coordinate transforms, like during ortho-rectification (see
section [ssec:ortho]). These transforms need to find the intersection
between the line of sight of the sensor and the earth geoid. If a simple
spheroid is used as the earth model, potentially high localisation
errors can be made in areas where elevation is high or perturbed. Of
course, DEM accuracy and resolution have a great impact on the precision
of these transforms.

Two main available DEM, free of charges, and with worldwide cover, are
both delivered as 1 degree by 1 degree tiles:

-  `The Shuttle Radar topographic Mission
   (SRTM) <http://www2.jpl.nasa.gov/srtm/>`__ is a 90 meters resolution
   DEM, obtained by radar interferometry during a campaign of the
   Endeavour space shuttle from NASA in 2000.

-  The `Advanced Spaceborne Thermal Emission and Reflection Radiometer
   (ASTER) <http://www.ersdac.or.jp/GDEM/E/2.html>`__ is a 30 meters
   resolution DEM obtained by stereoscopic processing of the archive of
   the ASTER instrument.

The relies on capabilities for sensor modelling and DEM handling. Tiles
of a given DEM are supposed to be located within a single directory.
General elevation support is also supported from GeoTIFF files.

Whenever an application or module requires a DEM, the option
**elev.dem** allows set the DEM directory. This directory must contains
the DEM tiles, either in DTED or SRTM format, either as GeoTIFF files.
Subdirectories are not supported.

Depending on the reference of the elevation, you also need to use a
geoid to manage elevation accurately. For this, you need to specify a
path to a file which contains the geoid. Geoid corresponds to the
equipotential surface that would coincide with the mean ocean surface of
the Earth (see `) <http://en.wikipedia.org/wiki/Geoid>`__. We provide
one geoid in the OTB-Data repository available
`here <http://hg.orfeo-toolbox.org/OTB-Data/file/4722d9e672c6/Input/DEM/egm96.grd>`__.

In all applications, the option **elev.geoid** allows to manage the path
to the geoid. Finally, it is also possible to use an average elevation
in case no DEM is available by using the **elev.default** option.

Ortho-rectification and map projections
---------------------------------------

There are several level of products available on the remote sensing
imagery market. The most basic level often provide the geometry of
acquisition (sometimes called the raw geometry). In this case, pixel
coordinates can not be directly used as geographical positions. For most
sensors (but not for all), the different lines corresponds to different
acquisition times and thus different sensor positions, and different
rows correspond to different cells of the detector.

The mapping of a raw image so as to be registered to a cartographic grid
is called ortho-rectification, and consist in inverting the following
effects (at least):

-  In most cases, lines are orthogonal to the sensor trajectory, which
   is not exactly (and in some case not at all) following a north-south
   axis,

-  Depending on the sensor, the line of sight may be different from a
   Nadir (ground position of the sensor), and thus a projective warping
   may appear,

-  The variation of height in the landscape may result in severe warping
   of the image.

Moreover, depending on the area of the world the image has been acquired
on, different map projections should be used.

The ortho-rectification process is as follows: once an appropriate map
projection has been defined, a localisation grid is computed to map
pixels from the raw image to the ortho-rectified one. Pixels from the
raw image are then interpolated according to this grid in order to fill
the ortho-rectified pixels.

Ortho-rectification can be performed either with or . Sensor parameters
and image meta-data are seamlessly read from the image files without
needing any user interaction, provided that all auxiliary files are
available. The sensor for which supports ortho-rectification of raw
products are the following:

-  Pleiades,

-  SPOT5,

-  Ikonos,

-  Quickbird,

-  GeoEye,

-  WorldView.

In addition, GeoTiff and other file format with geographical information
are seamlessly read by , and the ortho-rectification tools can be used
to re-sample these images in another map projection.

Beware of “ortho-ready” products
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are some image products, called “ortho-ready”, that should be
processed carefully. They are actual products in raw geometry, but their
metadata also contains projection data :

-  a map projection

-  a physical origin

-  a physical spacing

-  and sometimes an orientation angle

The purpose of this projection information is to give an approximate map
projection to a raw product. It allows you to display the raw image in a
GIS viewer at the (almost) right location, without having to reproject
it. Obviously, this map projection is not as accurate as the sensor
parameters of the raw geometry. In addition, the impact of the elevation
model can’t be observed if the map projection is used. In order to
perform an ortho-rectification on this type of product, the map
projection has to be hidden from .

You can see if a product is an “ortho-ready” product by using tools such
as ``gdalinfo`` or (see [app:ReadImageInfo]), and check if the product
verifies the 2 following conditions :

-  The product is in raw geometry : you should expect the presence of
   RPC coefficients and a non-empty OSSIM keywordlist.

-  The product has a map projection : you should see a projection name
   with physical origin and spacing.

In that case, you can hide the map projection from the by using
*extended* filenames. Instead of using the plain input image path, you
append a specific key at the end :

::

    "path_to_image?&skipcarto=true"

The double quote can be necessary for a successful parsing. More details
about the extended filenames can be found
`here <http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName>`__, and
also in the .

Ortho-rectification with 
~~~~~~~~~~~~~~~~~~~~~~~~~

The application allows to perform ortho-rectification and map
re-projection. The simplest way to use it is the following command:

::

    otbcli_OrthoRectification -io.in input_image -io.out output_image

In this case, the tool will automatically estimates all the necessary
parameters:

-  The map projection is set to UTM (a worldwide map projection) and the
   UTM zone is automatically estimated,

-  The ground sampling distance of the output image is computed to fit
   the image resolution,

-  The region of interest (upper-left corner and size of the image) is
   estimated so as to contain the whole input image extent.

In order to use a Digital Elevation Model (see section [ssec:dem]) for
better localisation performances, one can pass the directory containing
the DEM tiles to the application:

::

    otbcli_OrthoRectification -io.in input_image
                              -io.out output_image
                              -elev.dem dem_dir

If one wants to use a different map projection, the *-map* option may be
used (example with *lambert93* map projection):

::


    otbcli_OrthoRectification -io.in input_image
                              -io.out output_image
                              -elev.dem dem_dir
                              -map lambert93

Map projections handled by the application are the following (please
note that the ellipsoid is always WGS84):

-  | UTM : ``-map utm``
   | The UTM zone and hemisphere can be set by the options
   ``-map.utm.zone`` and ``-map.utm.northhem``.

-  Lambert 2 etendu: ``-map lambert2``

-  Lambert 93: ``-map lambert93``

-  | TransMercator: ``-map transmercator``
   | The related parameters (false easting, false northing and scale
   factor) can be set by the options
   ``-map.transmercator.falseeasting``,
   ``-map.transmercator.falsenorthing`` and ``-map.transmercator.scale``

-  WGS : ``-map wgs``

-  | Any map projection system with an EPSG code : ``-map epsg``
   | The EPSG code is set with the option ``-map.epsg.code``

The group ``outputs`` contains parameters to set the origin, size and
spacing of the output image. For instance, the ground spacing can be
specified as follows:

::


    otbcli_OrthoRectification -io.in input_image
                              -io.out output_image
                              -elev.dem dem_dir
                              -map lambert93
                              -outputs.spacingx spx
                              -outputs.spacingy spy

Please note that since the y axis of the image is bottom oriented, the y
spacing should be negative to avoid switching north and south direction.

A user-defined region of interest to ortho-rectify can be specified as
follows:

::


    otbcli_OrthoRectification -io.in input_image
                              -io.out output_image
                              -elev.dem dem_dir
                              -map lambert93
                              -outputs.spacingx spx
                              -outputs.spacingy spy
                              -outputs.ulx ul_x_coord
                              -outputs.uly ul_y_coord
                              -outputs.sizex x_size
                              -outputs.sizey y_size

Where the ``-outputs.ulx`` and ``-outputs.uly`` options allow to specify
the coordinates of the upper-left corner of the output image. The
``-outputs.sizex`` and ``-outputs.sizey`` options allow to specify the
size of the output image.

A few more interesting options are available:

-  The ``-opt.rpc`` option allows to use an estimated RPC model instead
   of the rigorous SPOT5 model, which speeds-up the processing,

-  The ``-opt.gridspacing`` option allows to define the spacing of the
   localisation grid used for ortho-rectification. A coarser grid
   results in speeding-up the processing, but with potential loss of
   accuracy. A standard value would be 10 times the ground spacing of
   the output image.

-  The ``-interpolator`` option allows to change the interpolation
   algorithm between nearest neighbor, linear and bicubic. Default is
   nearest neighbor interpolation, but bicubic should be fine in most
   cases.

-  The ``-opt.ram`` option allows to specify the amount of memory
   available for the processing (in Mb). Default is 256 Mb. Increasing
   this value to fit the available memory on your computer might
   speed-up the processing.

Residual registration
---------------------

Image registration is a fundamental problem in image processing. The aim
is to align two or more images of the same scene often taken at
different times, from different viewpoints, or by different sensors. It
is a basic step for orthorectification, image stitching, image fusion,
change detection…But this process is also critical for stereo
reconstruction process to be able to obtain an accurate estimation of
epipolar geometry.

Sensor model is generally not sufficient to provide image registrations.
Indeed, several sources of geometric distortion can be contained in
optical remote sensing images including earth rotation, platform
movement, non linearity…

They result in geometric errors on scene level, image level and pixel
level. It is critical to rectify the errors before a thematic map is
generated, especially when the remote sensing data need to be integrated
together with other GIS data.

This figure illustrates the generic workflow in the case of image series
registration:

[scale=0.15] (-1,-12) rectangle (75,17); in 5,...,1 (,) rectangle
+(4,4); (InputSeries) at (4,-1) Input series; (9,5) – +(3,0); (12.2,3)
rectangle +(6,4); (SensorModel) at (15,5) Sensor Model; (1,-10)
rectangle +(4,4); (DEM) at (5,-11) DEM; (3,-5.5) – ++(0,3) – ++(12,0) –
++(0,5); (18.5,5) – +(3,0); in 5,...,1 (,) rectangle +(4,4);
(GeoRefSeries) at (28,-1) Geo-referenced Series;

(25.5,8.5) – +(0,3);

(22,12) rectangle +(8.5,4); (HomPoExtr) at (27,14) Homologous Points;

(21.5,14) – +(-2.5,0);

(11,12) rectangle +(8,4); (BBAdj) at (15.5,14) Bundle-block Adjustement;

(15,11.5) – +(0,-4);

(30,5) – +(3,0); (33.2,2.5) rectangle +(6,4.5); (FineRegistration) at
(36,4.9) Fine Registration;

(39.5,5) – +(3,0); in 5,...,1 (,) rectangle +(4,4); (RegistSeries) at
(47,-1) Registered Series; (36,2) – ++(0,-10) – ++(-30,0);

(52,5) – +(3,0); (55.2,2.5) rectangle +(6,4.5); (CartoProjection) at
(57.5,4.9) Map Projection;

(61.5,5) – +(3,0); in 5,...,1 (,) rectangle +(4,4); (CartoSeries) at
(68,-1) Cartographic Series;

We will now illustrate this process by applying this workflow to
register two images. This process can be easily extended to perform
image series registration.

The aim of this example is to describe how to register a Level 1
QuickBird image over an orthorectify Pleiades image over the area of
Toulouse, France.

|image| |image| [fig:InputImagesRegistration]

Extract metadata from the image reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We first dump geometry metadata of the image we want to refine in a text
file. In OTB, we use the extension *.geom* for this type of file. As you
will see the application which will estimate a refine geometry only
needs as input this metadata and a set of homologous points. The
refinement application will create a new *.geom* file containing refined
geometry parameters which can be used after for reprojection for
example.

The use of external *.geom* file is available in OTB since release
:math:`3.16`. See
`here <http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName>`__ for
more information.

::


    otbcli_ReadImageInfo   -in slave_image
                           -outkwl TheGeom.geom

Extract homologous points from images
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The main idea of the residual registration is to estimate an second
transformation (after the application of sensors model).

The homologous point application use interest point detection method to
get a set of point which match in both images.

The basic idea is to use this set of homologous points and estimate with
them a residual transformation between the two images.

There is a wide variety of keypoint detector in the literature. They
allow to detect and describe local features in images. These algorithms
provide for each interesting point a “feature description”. This
descriptor has the property to be invariant to image translation,
scaling, and rotation, partially invariant to illumination changes and
robust to local geometric distortion. keypoints. Features extracted from
the input images are then matched against each other. These
correspondences are then used to create the homologous points.

`SIFT <http://en.wikipedia.org/wiki/Scale-invariant_feature_transform>`__
or `SURF <http://en.wikipedia.org/wiki/SURF>`__ keypoints can be
computed in the application. The band on which keypoints are computed
can be set independently for both images.

The application offers two modes :

-  the first is the full mode where keypoints are extracted from the
   full extent of both images (please note that in this mode large image
   file are not supported).

-  The second mode, called *geobins*, allows to set-up spatial binning
   so as to get fewer points spread across the entire image. In this
   mode, the corresponding spatial bin in the second image is estimated
   using geographical transform or sensor modeling, and is padded
   according to the user defined precision.

Moreover, in both modes the application can filter matches whose
co-localization in the first image exceed this precision. Last, the
elevation parameters allow to deal more precisely with sensor modelling
in case of sensor geometry data. The *outvector* option allows to create
a vector file with segments corresponding to the localization error
between the matches.

Finally, with the *2wgs84* option, you can match two sensor geometry
images or a sensor geometry image with an ortho-rectified reference. In
all cases, you get a list of ground control points spread all over your
image.

::



    otbcli_HomologousPointsExtraction   -in1 slave_image
                                        -in2 reference_image
                                        -algorithm surf
                                        -mode geobins
                                        -mode.geobins.binstep 512
                                        -mode.geobins.binsize 512
                                        -mfilter 1
                                        -precision 20
                                        -2wgs84 1
                                        -out homologous_points.txt
                                        -outvector points.shp
                                        -elev.dem dem_path/SRTM4-HGT/
                                        -elev.geoid OTB-Data/Input/DEM/egm96.grd

Note that for a proper use of the application, elevation must be
correctly set (including DEM and geoid file).

Geometry refinement using homologous points
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now that we can use this set of tie points to estimate a residual
transformation.For this we use the dedicated application called
**RefineSensorModel**. This application make use of OSSIM capabilities
to align the sensor model.

It reads the input geometry metadata file (*.geom*) which contains the
sensor model information that we want to refine and the text file
(homologous\_points.txt) containing the list of ground control point. It
performs a least-square fit of the sensor model adjustable parameters to
these tie points and produces an updated geometry file as output (the
extension which is always use is *.geom*)

The application can provide as well an optional ground control points
based statistics file and a vector file containing residues that you can
display in a GIS software.

Please note again that for a proper use of the application, elevation
must be correctly set (including DEM and geoid file). The map parameters
allows to choose a map projection in which the accuracy will be
estimated (in meters).

Accuracy values are provided as output of the application (computed
using tie points location) and allow also to control the precision of
the estimated model.

::


    otbcli_RefineSensorModel   -elev.dem dem_path/SRTM4-HGT/
                               -elev.geoid OTB-Data/Input/DEM/egm96.grd
                               -ingeom slave_image.geom
                               -outgeom refined_slave_image.geom
                               -inpoints homologous_points.txt
                               -outstat stats.txt
                               -outvector refined_slave_image.shp

Orthorecrtify image using the affine geometry
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now we will show how we can use this new sensor model. In our case we’ll
use this sensor model to orthorectify the image over the Pléiades
reference. offers since version 3.16 the possibility to use
hrefhttp://wiki.orfeo-toolbox.org/index.php/ExtendedFileNameextend image
path to use different metadata file as input. That’s what we are going
to use there to orthorectify the QuickBird image using the *.geom* file
obtained by the **RefineSensorModel** applications. over the first one
using for the second image estimated sensor model which take into
account the original sensor model of the slave and which also fit to the
set of tie points.

::


    otbcli_OrthoRectification   -io.in slave_image?&geom=TheRefinedGeom.geom
                                -io.out ortho_slave_image
                                -elev.dem dem_path/SRTM4-HGT/
                                -elev.geoid OTB-Data/Input/DEM/egm96.grd
                         

As a result, if you’ve got enough homologous points in images and
control that the residual error between the set of tie points and the
estimated sensor model is small, you must achieve a good registration
now between the 2 rectified images. Normally far better than ’only’
performing separate orthorectification over the 2 images.

This methodology can be adapt and apply in several cases, for example :

-  register stereo pair of images and estimate accurate epipolar
   geometry

-  registration prior to change detection

.. |image| image:: ../Art/MonteverdiImages/monteverdi_optical_calibration.png
.. |image| image:: ../Art/MonteverdiImages/monteverdi_optical_calibration_outputs.png
.. |image| image:: ../Art/MonteverdiImages/monteverdi_QB_PAN_ROI.png
.. |image| image:: ../Art/MonteverdiImages/monteverdi_QB_MUL_Superimpose.png
.. |image| image:: ../Art/MonteverdiImages/monteverdi_QB_XS_pan-sharpened.png
.. |image| image:: ../Art/MonteverdiImages/registration_pleiades_ql.png
.. |image| image:: ../Art/MonteverdiImages/registration_quickbird_ql.png
