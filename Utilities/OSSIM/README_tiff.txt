During the OSSIM integration operation, the include of <tiffio.h>, <tiff.h> and <jpeglib.h> have
been replaced by <itk_tiff.h> and <itkjpeg/8/jpeglib.h> respectively.

The ossim library is linked again ${ITK_TIFF_LIBRARY} and itkjpeg8. 

This allows us to avoid looking for the right tiff and jpeg headers by ourselves, since it is already done
by ITK.


The OTB Development Team.
