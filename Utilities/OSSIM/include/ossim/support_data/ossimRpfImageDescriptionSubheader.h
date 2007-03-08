//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfImageDescriptionSubheader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfImageDescriptionSubheader_HEADER
#define ossimRpfImageDescriptionSubheader_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfImageDescriptionSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfImageDescriptionSubheader& data);
   ossimRpfImageDescriptionSubheader();
   virtual ~ossimRpfImageDescriptionSubheader(){}

   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   
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
   virtual void print(ostream& out)const;
   
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
