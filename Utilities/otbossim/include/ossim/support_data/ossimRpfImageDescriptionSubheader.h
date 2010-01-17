//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfImageDescriptionSubheader.h 14241 2009-04-07 19:59:23Z dburken $
#ifndef ossimRpfImageDescriptionSubheader_HEADER
#define ossimRpfImageDescriptionSubheader_HEADER

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfImageDescriptionSubheader
{
public:
   friend std::ostream& operator <<(
      std::ostream& out, const ossimRpfImageDescriptionSubheader& data);

   ossimRpfImageDescriptionSubheader();
   ~ossimRpfImageDescriptionSubheader(){}

   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);
   
   ossim_uint32 getStartOffset()const{return theStartOffset;}
   ossim_uint32 getEndOffset()const{return theEndOffset;}

   bool isSubframeMaskTableOffsetNull()const
   {
      return (theSubframeMaskTableOffset == OSSIM_RPF_ULONG_NULL);
   }
   bool isTransparencyMaskTableOffsetNull()const
   {
      return (theTransparencyMaskTableOffset == OSSIM_RPF_ULONG_NULL);
   }
   ossim_uint16 getNumberOfSpectralGroups()const
   {
      return theNumberOfSpectralGroups;
   }
   ossim_uint16 getNumberOfSubframeTables()const
   {
      return theNumberOfSubframeTables;
   }
   ossim_uint16 getNumberOfSpectralBandTables()const
   {
      return theNumberOfSpectralBandTables;
   }
   ossim_uint16 getNumberOfSpectralBandLinesPerImageRow()const
   {
      return theNumberOfSpectralBandLinesPerImageRow;
   }
   ossim_uint16 getNumberOfSubframesHorizontal()const
   {
      return theNumberOfSubframesHorizontal;
   }
   ossim_uint16 getNumberOfSubframesVertical()const
   {
      return theNumberOfSubframesVertical;
   }
   ossim_uint32  getNumberOfOutputColumnsPerSubframe()const
   {
      return theNumberOfOutputColumnsPerSubframe;
   }
   ossim_uint32  getNumberOfOutputRowsPerSubframe()const
   {
      return theNumberOfOutputRowsPerSubframe;
   }
   ossim_uint32  getSubframeMaskTableOffset()const
   {
      return theSubframeMaskTableOffset;
   }
   ossim_uint32  getTransparencyMaskTableOffset()const
   {
      return theTransparencyMaskTableOffset;
   }

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
   
private:
   void clearFields();
   // work variables to store useful information.
   ossim_uint32 theStartOffset;
   ossim_uint32 theEndOffset;

   ossim_uint16 theNumberOfSpectralGroups;
   ossim_uint16 theNumberOfSubframeTables;
   ossim_uint16 theNumberOfSpectralBandTables;
   ossim_uint16 theNumberOfSpectralBandLinesPerImageRow;

   /*!
    * Each frame file could be divided into sub frames.
    * This identifies the number of subframes in the
    * horizontal direction.
    */
   ossim_uint16 theNumberOfSubframesHorizontal;

   /*!
    * Each frame file could be divided into sub frames.
    * This identifies the number of subframes in the
    * vertical direction.
    */
   ossim_uint16 theNumberOfSubframesVertical;
   ossim_uint32 theNumberOfOutputColumnsPerSubframe;
   ossim_uint32 theNumberOfOutputRowsPerSubframe;
   ossim_uint32 theSubframeMaskTableOffset;
   ossim_uint32 theTransparencyMaskTableOffset;
};

#endif
