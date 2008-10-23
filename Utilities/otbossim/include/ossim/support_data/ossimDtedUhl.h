//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero
// 
// Description:  This class gives access to the User Header Label
//               (UHL) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedUhl.h 10262 2007-01-14 18:58:38Z dburken $
#ifndef ossimDtedUhl_H
#define ossimDtedUhl_H
#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>

class OSSIM_DLL ossimDtedUhl : public ossimErrorStatusInterface
{
public:
   ossimDtedUhl(const ossimFilename& dted_file, ossim_int32 offset);
   ossimDtedUhl(std::istream& in, ossim_int32 offset);

   enum
   {
      UHL_LENGTH        = 80,
      UHL_LON_ORIGIN    =  5,
      UHL_LAT_ORIGIN    = 13,
      UHL_LON_INTERVAL  = 21,
      UHL_LAT_INTERVAL  = 25,
      UHL_ABSOLUTE_LE   = 29,
      UHL_SECURITY_CODE = 33,
      UHL_REFERENCE_NUM = 33,
      UHL_NUM_LON_LINES = 48,
      UHL_NUM_LAT_LINES = 52,
      UHL_MULTIPLE_ACC  = 56,
      UHL_RESERVED      = 57,
      FIELD1_SIZE       = 3,
      FIELD2_SIZE       = 1,
      FIELD3_SIZE       = 8,
      FIELD4_SIZE       = 8,
      FIELD5_SIZE       = 4,
      FIELD6_SIZE       = 4,
      FIELD7_SIZE       = 4,
      FIELD8_SIZE       = 3,
      FIELD9_SIZE       = 12,
      FIELD10_SIZE      = 4,
      FIELD11_SIZE      = 4,
      FIELD12_SIZE      = 1,
      FIELD13_SIZE      = 24
   };
   
   // The Recoginition Sentinel signifies if the UHL record exists.
   ossimString recoginitionSentinel() const;

   double      lonOrigin()        const;
   double      latOrigin()        const;
   double      lonInterval()      const;
   double      latInterval()      const;
   double      absoluteLE()       const;
   ossimString securityCode()     const;
   ossim_int32 numLonLines()      const;
   ossim_int32 numLatPoints()     const;
   ossim_int32 mulitpleAccuracy() const;
   ossim_int32 startOffset()      const;
   ossim_int32 stopOffset()       const;
   
   friend OSSIM_DLL std::ostream& operator<<( std::ostream& os,
                                              const ossimDtedUhl& uhl);

   void parse(std::istream& in);

private:
   // Do not allow...
   ossimDtedUhl(const ossimDtedUhl& source);
   const ossimDtedUhl& operator=(const ossimDtedUhl& rhs);

   double degreesFromString(const char* str) const;
   double spacingFromString(const char* str) const;
   
   char theRecSen[FIELD1_SIZE+1];
   char theField2[FIELD2_SIZE+1];
   char theLonOrigin[FIELD3_SIZE+1];
   char theLatOrigin[FIELD4_SIZE+1];
   char theLonInterval[FIELD5_SIZE+1];
   char theLatInterval[FIELD6_SIZE+1];
   char theAbsoluteLE[FIELD7_SIZE+1];
   char theSecurityCode[FIELD8_SIZE+1];
   char theField9[FIELD9_SIZE+1];
   char theNumLonLines[FIELD10_SIZE+1];
   char theNumLatPoints[FIELD11_SIZE+1];
   char theMultipleAccuracy[FIELD12_SIZE+1];
   
   ossim_int32 theStartOffset;
   ossim_int32 theStopOffset;
};

#endif
