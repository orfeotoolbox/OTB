Image processing
================

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

The naming convention “im[x]b[y]” designates the yth band of the xth
input image.

The *BandMath* application embeds built-in operators and functions
listed in `muparser documentation <https://beltoforion.de/article.php?a=muparser&p=features&s=idDef1#idDef1>`_ thus
allowing a vast choice of possible operations.

Images with no-data values
--------------------------

Image files can contain a no-data value in their metadata. It represents
a special pixel value that should be treated as “no data available for
this pixel”. For instance, SRTM tiles use a particular no-data value of
-32768 (usually found on sea areas).

On multiband images, the no-data values are handled independently for
each band. The case of an image with no-data values defined only for a
subset of its bands is supported.

This metadata is now handled by OTB image readers and writer (using the
GDAL driver). The no-data value can be read from an image files and
stored in the image metadata dictionary. It can also be exported by
image writers. The OTB filters that produce a no-data value are able to
export this value so that the output file will store it.

An application has been created to manage the no-data value. The
application has the following features:

-  Build a mask corresponding to the no-data pixels in the input image:
   it gives you a binary image of the no-data pixels in your input
   image.

-  Change the no-data value of the input image: it will change all
   pixels that carry the old no-data value to the new one and update the
   metadata

-  Apply an external mask to the input image as no-data: all the pixels
   that corresponds have a null mask value are flagged as no-data in the
   output image.

For instance, the following command converts the no-data value of the
input image to the default value for DEM (which is -32768):

::

    otbcli_ManageNoData -in input_image.tif
                        -out output_image.tif
                        -mode changevalue
                        -mode.changevalue.newv -32768

The third mode “apply” can be useful if you apply a formula to the
entire image. This will likely change the values of pixels flagged as
no-data, but the no-data value in the image metadata does not change. If
you want to fix all no-data pixels to their original value, you can
extract the mask of the original image and apply it on the output image.
For instance:

::

    otbcli_ManageNoData -in input_image.tif
                        -out mask.tif
                        -mode buildmask

    otbcli_BandMath -il input_image.tif
                    -out filtered_image.tif
                    -exp "2*im1b1-4"

    otbcli_ManageNoData -in filtered_image.tif
                        -out output_image.tif
                        -mode apply
                        -mode.apply.mask mask.tif

You can also use this “apply” mode with an additional parameter
“mode.apply.ndval”. This parameter sets the output nodata value
of the input mask.

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

You can find more information about this application
`here <http://blog.orfeo-toolbox.org/preview/coming-next-large-scale-segmentation>`__.

Large-Scale Mean-Shift (LSMS) segmentation
------------------------------------------

LSMS is a segmentation workflow which allows to perform tile-wise
segmentation of very large image with theoretical guarantees of getting
identical results to those without tiling.

It has been developed by David Youssefi and Julien Michel during David
internship at CNES.

For more a complete description of the LSMS method, please refer to the
following publication, *J. Michel, D. Youssefi and M. Grizonnet, “Stable
Mean-Shift Algorithm and Its Application to the Segmentation of
Arbitrarily Large Remote Sensing Images,” in IEEE Transactions on
Geoscience and Remote Sensing, vol. 53, no. 2, pp. 952-964, Feb. 2015.*
The workflow consists in chaining 3 or 4 dedicated applications and
produces a GIS vector file with artifact-free polygons corresponding to
the segmented image, as well as mean and variance of the radiometry of
each band for each polygon.

Step 1: Mean-Shift Smoothing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first step of the workflow is to perform Mean-Shift smoothing with
the *MeanShiftSmoothing* application:

::

    otbcli_MeanShiftSmoothing -in input_image.tif
                              -fout filtered_range.tif 
                              -foutpos filtered_spatial.tif
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
neighboring pixels whose range distance is below the *ranger* parameter and
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
                                   -inseg segmentation.tif
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

    otbcli_LSMSVectorization -in input_image.tif
                             -inseg segmentation_merged.tif 
                             -out segmentation_merged.shp 
                             -tilesizex 256 
                             -tilesizey 256

This application will process the input images tile-wise to keep
resources usage low, with the guarantee of identical results. You can
set the tile size using the *tilesizex* and *tilesizey* parameters.
However unlike the *LSMSSegmentation* application, it does not require
to write any temporary file to disk.

All-in-one
~~~~~~~~~~

The *LargeScaleMeanShift* application is a composite application that chains
all the previous steps:

- Mean-Shift Smoothing
- Segmentation
- Small region merging
- Vectorization

Most of the settings from the previous applications are also exposed in this
composite application. The range and spatial radius used for the segmentation
step are half the values used for Mean-Shift smoothing, which are obtained from
LargeScaleMeanShift parameters. There are two output modes: vector (default)
and raster. When the raster output is chosen, last step (vectorization) is
skipped.

::

    otbcli_LargeScaleMeanShift -in input_image.tif
                               -spatialr 5
                               -ranger 30
                               -minsize 10
                               -mode.vector.out segmentation_merged.shp

There is a cleanup option that can be disabled in order to check intermediate
outputs of this composite application.
