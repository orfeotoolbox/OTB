//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Container class for J2K Image and tile size (SIZ) record.
//
// See document BPJ2K01.00 Table 7-6 Image and tile size (15444-1 Annex A5.1)
// 
//----------------------------------------------------------------------------
// $Id: ossimJ2kSizRecord.h,v 1.5 2005/10/13 21:24:47 dburken Exp $
#ifndef ossimJ2kSizRecord_HEADER
#define ossimJ2kSizRecord_HEADER

#include <iosfwd>
#include <string>

#include <ossim/base/ossimConstants.h>

class OSSIM_DLL ossimJ2kSizRecord
{
public:
   
   /** default constructor */
   ossimJ2kSizRecord();

   /** destructor */
   ~ossimJ2kSizRecord();

   /**
    * Parse method.  Performs byte swapping as needed.
    *
    * @param in Stream to parse.
    *
    * @note SIZ Marker (0xff51) is not read.
    */
   void parseStream(std::istream& in);

   /** @return scalar type based on bit depth and signed bit from theSsiz. */
   ossimScalarType getScalarType() const;

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;

   /** operator<< */
   friend OSSIM_DLL std::ostream& operator<<(
      std::ostream& out, const ossimJ2kSizRecord& obj);

   /** segmet marker 0xff51 (big endian) */
   ossim_uint16 theMarker;

   /** length of segment minus marker */
   ossim_uint16 theLsiz;

   /** profile */
   ossim_uint16 theRsiz;

   /** width of reference grid */
   ossim_uint32 theXsiz;

   /** height of reference grid */
   ossim_uint32 theYsiz;

   /**
    * Horizontal offset from the orgin of reference grid to the left side
    * of image.
    */
   ossim_uint32 theXOsiz;

   /**
    * Vertical offset from the orgin of reference grid to the top
    * of image.
    */
   ossim_uint32 theYOsiz;

   /** width of one reference tile */
   ossim_uint32 theXTsiz;
   
   /** height of one reference tile */   
   ossim_uint32 theYTsiz;

   /**
    * Horizontal offset from the orgin of reference grid to the left edge
    * of first tile.
    */
   ossim_uint32 theXTOsiz;

   /**
    * Vertical offset from the orgin of reference grid to the top
    * edge of first tile.
    */
   ossim_uint32 theYTOsiz;

   /** number of component in the image */
   ossim_uint16 theCsiz;

   /**
    * sign bit and bit depth of data
    * unsigned = 0xxx xxxx (msb == 0)
    * signed   = 1xxx xxxx (msb == 1)
    * bit depth = x000 0000 + 1 (first seven bits plus one)
    */
   ossim_uint8  theSsiz;

   /**
    * Horizontal separation of a sample of the component with respect to the
    * reference grid.
    */
   ossim_uint8  theXRsiz;

   /**
    * Vertical separation of a sample of the component with respect to the
    * reference grid.
    */
   ossim_uint8  theYRsiz;
};

#endif /* End of "#ifndef ossimJ2kSizRecord_HEADER" */
