Image processing and information extraction
===========================================

Simple calculus with channels
-----------------------------

The *BandMath* application provides a simple and efficient way to
perform band operations. The command line application and the
corresponding Monteverdi module (shown in the section [Band:sub:`m`\ ath
module]) are based on the same standards. It computes a band wise
operation according to a user defined mathematical expression. The
following code computes the absolute difference between first bands of
two images.

::

    otbcli_BandMath -il input_image_1 input_image_2
                    -exp "abs(im1b1 - im2b1)"
                    -out output_image

The naming convention “im[x]b[y]” designates the y\ :sup:`th` band of the x\ :sup:`th` input image.

The *BandMath* application embeds built-in operators and functions
listed in `muparser documentation <http://muparser.sourceforge.net/mup_features.html#idDef2>`_ thus
allowing a vast choice of possible operations.

Segmentation
------------

Segmenting objects across a very high resolution scene and with a
controlled quality is a difficult task for which no method has reached a
sufficient level of performance to be considered as operational.

Even if we leave aside the question of segmentation quality and consider
that we have a method performing reasonably well on our data and objects
of interest, the task of scaling up segmentation to real very high
resolution data is itself challenging. First, we can not load the whole
data into memory, and there is a need for on the flow processing which
does not cope well with traditional segmentation algorithms. Second, the
result of the segmentation process itself is difficult to represent and
manipulate efficiently.

The experience of segmenting large remote sensing images is packed into
a single *Segmentation* in **OTB Applications** .

You can find more information about this application in this `blog <http://blog.orfeo-toolbox.org/preview/coming-next-large-scale-segmentation>`_ .

Large-Scale Mean-Shift (LSMS) segmentation
------------------------------------------

LSMS is a segmentation workflow which allows to perform tile-wise
segmentation of very large image with theoretical guarantees of getting
identical results to those without tiling. It has been developed by
David Youssefi and Julien Michel during David internship at CNES and is
to be published soon.

The workflow consists in chaining 3 or 4 dedicated applications and
produces a GIS vector file with artifact-free polygons corresponding to
the segmented image, as well as mean and variance of the radiometry of
each band for each polygon.

Step 1: Mean-Shift Smoothing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first step of the workflow is to perform Mean-Shift smoothing with
the *MeanShiftSmoothing* application:

::

    otbcli_MeanShiftSmoothing -in input_image 
                              -fout filtered_range.tif 
                              -foutpos filtered_spat.tif 
                              -ranger 30 
                              -spatialr 5 
                              -maxiter 10 
                              -modesearch 0

Note that the *modesearch* option should be disabled, and that the
*foutpos* parameter is optional: it can be activated if you want to
perform the segmentation based on both spatial and range modes.

This application will smooth large images by streaming them, and
deactivating the *modesearch* will guarantee that the results will not
depend on the streaming scheme. Please also note that the *maxiter* is
used to set the margin to ensure these identical results, and as such
increasing the *maxiter* may have an additional impact on processing
time.

Step 2: Segmentation
~~~~~~~~~~~~~~~~~~~~

The next step is to produce an initial segmentation based on the
smoothed images produced by the *MeanShiftSmoothing* application. To do
so, the *LSMSSegmentation* will process them by tiles whose dimensions
are defined by the *tilesizex* and *tilesizey* parameters, and by
writing intermediate images to disk, thus keeping the memory consumption
very low throughout the process. The segmentation will group together
adjacent pixels whose range distance is below the *ranger* parameter and
(optionally) spatial distance is below the *spatialr* parameter.

::

    otbcli_LSMSSegmentation -in filtered_range.tif
                            -inpos filtered_spatial.tif
                            -out  segmentation.tif uint32 
                            -ranger 30 
                            -spatialr 5 
                            -minsize 0 
                            -tilesizex 256 
                            -tilesizey 256

Note that the final segmentation image may contains a very large number
of segments, and the *uint32* image type should therefore be used to
ensure that there will be enough labels to index those segments. The
*minsize* parameter will filter segments whose size in pixels is below
its value, and their labels will be set to 0 (nodata).

Please note that the output segmented image may look patchy, as if there
were tiling artifacts: this is because segments are numbered
sequentially with respect to the order in which tiles are processed. You
will see after the result of the vectorization step that there are no
artifacts in the results.

The *LSMSSegmentation* application will write as many intermediate files
as tiles needed during processing. As such, it may require twice as free
disk space as the final size of the final image. The *cleanup* option
(active by default) will clear the intermediate files during the
processing as soon as they are not needed anymore. By default, files
will be written to the current directory. The *tmpdir* option allows to
specify a different directory for these intermediate files.

Step 3 (optional): Merging small regions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The *LSMSSegmentation* application allows to filter out small segments.
In the output segmented image, those segments will be removed and
replaced by the background label (0). Another solution to deal with the
small regions is to merge them with the closest big enough adjacent
region in terms of radiometry. This is handled by the
*LSMSSmallRegionsMerging* application, which will output a segmented
image where small regions have been merged. Again, the *uint32* image
type is advised for this output image.

::

    otbcli_LSMSSmallRegionsMerging -in filtered_range.tif
                                   -inseg segementation.tif
                                   -out segmentation_merged.tif uint32 
                                   -minsize 10 
                                   -tilesizex 256 
                                   -tilesizey 256

The *minsize* parameter allows to specify the threshold on the size of
the regions to be merged. Like the *LSMSSegmentation* application, this
application will process the input images tile-wise to keep resources
usage low, with the guarantee of identical results. You can set the tile
size using the *tilesizex* and *tilesizey* parameters. However unlike
the *LSMSSegmentation* application, it does not require to write any
temporary file to disk.

Step 4: Vectorization
~~~~~~~~~~~~~~~~~~~~~

The last step of the LSMS workflow consists in the vectorization of the
segmented image into a GIS vector file. This vector file will contain
one polygon per segment, and each of these polygons will hold additional
attributes denoting the label of the original segment, the size of the
segment in pixels, and the mean and variance of each band over the
segment. The projection of the output GIS vector file will be the same
as the projection from the input image (if input image has no
projection, so does the output GIS file).

::

    otbcli_LSMSVectorization -in input_image 
                             -inseg segmentation_merged.tif 
                             -out segmentation_merged.shp 
                             -tilesizex 256 
                             -tilesizey 256

This application will process the input images tile-wise to keep
resources usage low, with the guarantee of identical results. You can
set the tile size using the *tilesizex* and *tilesizey* parameters.
However unlike the *LSMSSegmentation* application, it does not require
to write any temporary file to disk.

Dempster Shafer based Classifier Fusion
---------------------------------------

This framework is dedicated to perform cartographic validation starting
from the result of a detection (for example a road extraction), enhance
the results fiability by using a classifier fusion algorithm. Using a
set of descriptor, the processing chain validates or invalidates the
input geometrical features.

Fuzzy Model (requisite)
~~~~~~~~~~~~~~~~~~~~~~~

The *DSFuzzyModelEstimation* application performs the fuzzy model
estimation (once by use case: descriptor set / Belief support /
Plausibility support). It has the following input parameters :

-  ``-psin`` a vector data of positive samples enriched according to the
   “Compute Descriptors” part

-  ``-nsin`` a vector data of negative samples enriched according to the
   “Compute Descriptors” part

-  ``-belsup`` a support for the Belief computation

-  ``-plasup`` a support for the Plausibility computation

-  ``-desclist`` an initialization model (xml file) or a descriptor name
   list (listing the descriptors to be included in the model)

The application can be used like this:

::

    otbcli_DSFuzzyModelEstimation -psin     PosSamples.shp
                                  -nsin     NegSamples.shp
                                  -belsup   "ROADSA"
                                  -plasup   "NONDVI" "ROADSA" "NOBUIL"
                                  -desclist "NONDVI" "ROADSA" "NOBUIL"
                                  -out      FuzzyModel.xml

The output file ``FuzzyModel.xml`` contains the optimal model to perform
informations fusion.

First Step: Compute Descriptors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first step in the classifier fusion based validation is to compute,
for each studied polyline, the choosen descriptors. In this context, the
*ComputePolylineFeatureFromImage* application can be used for a large
range of descriptors. It has the following inputs :

-  ``-in`` an image (of the sudied scene) corresponding to the choosen
   descriptor (NDVI, building Mask…)

-  ``-vd`` a vector data containing polyline of interest

-  ``-expr`` a formula (“b1 >0.4”, “b1 == 0”) where b1 is the standard
   name of input image first band

-  ``-field`` a field name corresponding to the descriptor codename
   (NONDVI, ROADSA...)

The output is a vector data containing polylines with a new field
containing the descriptor value. In order to add the “NONDVI” descriptor
to an input vector data (“inVD.shp”) corresponding to the percentage of
pixels along a polyline that verifies the formula “NDVI >0.4” :

::

    otbcli_ComputePolylineFeatureFromImage -in   NDVI.TIF
                                           -vd  inVD.shp
                                           -expr  "b1 > 0.4"
                                           -field "NONDVI"
                                           -out   VD_NONDVI.shp

``NDVI.TIF`` is the NDVI mono band image of the studied scene. This step
must be repeated for each choosen descriptor:

::

    otbcli_ComputePolylineFeatureFromImage -in   roadSpectralAngle.TIF
                                           -vd  VD_NONDVI.shp
                                           -expr  "b1 > 0.24"
                                           -field "ROADSA"
                                           -out   VD_NONDVI_ROADSA.shp

::

    otbcli_ComputePolylineFeatureFromImage -in   Buildings.TIF
                                           -vd  VD_NONDVI_ROADSA.shp
                                           -expr  "b1 == 0"
                                           -field "NOBUILDING"
                                           -out   VD_NONDVI_ROADSA_NOBUIL.shp

Both ``NDVI.TIF`` and ``roadSpectralAngle.TIF`` can be produced using
**Monteverdi** feature extraction capabilities, and ``Buildings.TIF``
can be generated using **Monteverdi** rasterization module. From now on,
``VD_NONDVI_ROADSA_NOBUIL.shp`` contains three descriptor fields. It
will be used in the following part.

Second Step: Feature Validation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The final application (*VectorDataDSValidation* ) will validate or
unvalidate the studied samples using `the Dempster-Shafer
theory <http://en.wikipedia.org/wiki/Dempster%E2%80%93Shafer_theory>`_ 
. Its inputs are :

-  ``-in`` an enriched vector data “VD\_NONDVI\_ROADSA\_NOBUIL.shp”

-  ``-belsup`` a support for the Belief computation

-  ``-plasup`` a support for the Plausibility computation

-  ``-descmod`` a fuzzy model FuzzyModel.xml

The output is a vector data containing only the validated samples.

::

    otbcli_VectorDataDSValidation -in      extractedRoads_enriched.shp
                                  -descmod FuzzyModel.xml
                                  -out     validatedSamples.shp

