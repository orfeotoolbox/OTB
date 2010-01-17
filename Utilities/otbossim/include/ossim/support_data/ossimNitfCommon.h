//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class for global nitf methods.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfCommon.h 15410 2009-09-11 19:45:38Z dburken $
#ifndef ossimNitfCommon_HEADER
#define ossimNitfCommon_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <iostream>

class ossimDpt;
class ossimIrect;

/** class ossimNitfCommon for global utility methods */   
class OSSIM_DLL ossimNitfCommon
{
public:

   /** default constructor */
   ossimNitfCommon();

   /** destructor */
   ~ossimNitfCommon();

   /**
    * This method takes a value and a size(or width) and converts it to a
    * scientific notation sting in the format: ±0.999999E±9
    *
    * @note If the value is positive, currently there is no + added to the
    * front.
    *
    * @param aValue Value to convert to string.
    *
    * @param size total size of the string.  This must be at least 7 if
    * aValue is positive and at least 8 if aValue is negative; else,
    * no action is taken and an empty string is returned.
    */
   static ossimString convertToScientificString(const ossim_float64& aValue,
                                                ossim_uint32 size);

   /**
    * This method takes a value and converts to a string of size with
    * requested precision with a '0' fill.
    *
    * Format example: 00925.00
    * 
    * @param aValue Value to convert to string.
    *
    * @param precision The floating point precision.
    *
    * @param size total size of the string.
    */
   static ossimString convertToDoubleString(const ossim_float64& aValue,
                                            ossim_uint32 precision,
                                            ossim_uint32 size);

   /**
    * This method takes a value and converts to a string of size with
    * with a '0' fill.
    *
    * Format example: 00925

    * @param aValue Value to convert to string.
    *
    * @param size total size of the string.
    */
   static ossimString convertToUIntString(ossim_uint32 aValue,
                                          ossim_uint32 size);
   
   /**
    * Just like convertToUIntString only takes a signed value.
    * 
    * This method takes a value and converts to a string of size with
    * with a '0' fill.
    *
    * Format example: 00925

    * @param aValue Value to convert to string.
    *
    * @param size total size of the string.
    */
   static ossimString convertToIntString(ossim_int32 aValue,
                                         ossim_uint32 size);

   /**
    * Converts pt to a decimal degrees latitude longitude location string.
    *
    * Format example: +-dd.dddddd+-ddd.dddddd
    *
    * @param pt Point to convert where pt.x = longitude and pt.y = latitude.
    *
    * @param  precision The floating point precision. This will determine the
    * size of the string.
    */
   static ossimString convertToDdLatLonLocString(const ossimDpt& pt,
                                                 ossim_uint32 precision);

   /**
    * Sets a field with a given string, width, and IOS flags.
    *
    * @param fieldDestination
    *        The member NITF field to set.
    * @param src
    *        The source string.
    * @param width
    *        The number of characters in fieldDestination to copy from the
    *        source string.
    * @param ioflags
    *        Formatting flags for the destination string.
    * @param fill
    *        If the size of the string is smaller than the field size,
    *        this character specifies what the excess character values are.
    */
   static void setField(void* fieldDestination,
                        const ossimString& src,
                        std::streamsize width,
                        std::ios_base::fmtflags ioflags = std::ios::left,
                        char fill = ' ');
   /**
    * This will encode a 60 character IGEOLO field.
    */
   static ossimString encodeUtm(
                                ossim_uint32 zone,
                                const ossimDpt& ul,
                                const ossimDpt& ur,
                                const ossimDpt& lr,
                                const ossimDpt& ll);
   static ossimString encodeGeographicDms(
                                          const ossimDpt& ul,
                                          const ossimDpt& ur,
                                          const ossimDpt& lr,
                                          const ossimDpt& ll);
   static ossimString encodeGeographicDecimalDegrees(
                                                     const ossimDpt& ul,
                                                     const ossimDpt& ur,
                                                     const ossimDpt& lr,
                                                     const ossimDpt& ll);

   /**
    * @brief Get the nitf pixel type string from scalar type.
    *
    * @param scalarType The scalar type.
    *
    * @return Pixel type as string, e.g.: "INT", "SI", "R"
    */
   static ossimString getNitfPixelType(ossimScalarType scalarType);

   /**
    * @brief Get compression rate (COMRAT) which is bits perpixel per band.
    *
    * This corresponds to the nitf image header COMRAT field.  Return is in
    * the form of Nxyz "N" for numerically lossles, where "xyz"
    * indicates the expected achieved bit rate (in bits per pixel per band)
    * for the final layer of each tile. The decimal point is implicit and
    * assumed to be one digit from the right (i.e. xy.z).
    * 
    * @param rect Image rectangles.
    * @param bands
    * @param scalar
    * @param lengthInBytes Compressed length of image pixels.
    * @return Approximate number of bits per pixel for the compressed image as
    * a string which follows the nitf comrat field convention.  This will
    * return an empty string if comrat is bigger than three digits.
    */
   static ossimString getCompressionRate(const ossimIrect& rect,
                                         ossim_uint32 bands,
                                         ossimScalarType scalar,
                                         ossim_uint64 lengthInBytes);
   
   
}; // End of class ossimNitfCommon.
   
#endif /* #ifndef ossimNitfCommon_HEADER */
