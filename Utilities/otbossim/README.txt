1. During the OSSIM integration operation, the include of <tiffio.h>, <tiff.h> and <jpeglib.h> have
been replaced by <itk_tiff.h> and <itkjpeg/8/jpeglib.h> respectively.

The ossim library is linked again gdal and itkjpeg8. 

This allows us to avoid looking for the right jpeg headers by ourselves, since it is already done
by ITK.

2. During generating otb::MapProjection<ossimLlxyProjection>, pb with polymorphism :
   virtual void computeMetersPerPixel(		double deltaDegreesPerPixelLat,
                                            double deltaDegreesPerPixelLon,
                                            ossimDpt &metersPerPixel)
   The method is redefined in the file "ossimLlxyProjection.h"


The OTB Development Team.
