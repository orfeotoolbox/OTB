//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Volume Header Label
//               (VOL) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedVol.h 10262 2007-01-14 18:58:38Z dburken $

#ifndef ossimDtedVol_H
#define ossimDtedVol_H
#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>

class OSSIM_DLL ossimDtedVol : public ossimErrorStatusInterface
{
public:
   ossimDtedVol(const ossimFilename& dted_file,
                ossim_int32 offset);
   ossimDtedVol(std::istream& in,
                ossim_int32 offset);

   enum
   {
      VOL_LENGTH            = 80,
      VOL_ONE_LABEL_1       =  4,
      VOL_REEL_NUMBER       =  5,
      VOL_REEL_ACCESS       = 11,
      VOL_SPACE             = 12,
      VOL_ACCOUNT_NUMBER    = 38,
      VOL_BLANKS            = 52,
      VOL_ONE_LABEL_2       = 80,
      FIELD1_SIZE           = 3,
      FIELD2_SIZE           = 1,
      FIELD3_SIZE           = 6,
      FIELD4_SIZE           = 1,
      FIELD5_SIZE           = 26,
      FIELD6_SIZE           = 14,
      FIELD7_SIZE           = 28,
      FIELD8_SIZE           = 1
   };
   
   // The Recoginition Sentinel signifies if the VOL record exists.
   ossimString getRecoginitionSentinel() const;
   ossimString getReelNumber()           const;
   ossimString getAccountNumber()        const;
   ossim_int32 startOffset()             const;
   ossim_int32 stopOffset()              const;
   
   friend OSSIM_DLL std::ostream& operator<<( std::ostream& os,
                                              const ossimDtedVol& vol);
   
   void parse(std::istream& in);

private:
   // Do not allow...
   ossimDtedVol(const ossimDtedVol& source);
   const ossimDtedVol& operator=(const ossimDtedVol& rhs);

   char theRecSen[FIELD1_SIZE+1];
   char theField2[FIELD2_SIZE+1];
   char theReelNumber[FIELD3_SIZE+1];
   char theField4[FIELD4_SIZE+1];
   char theField5[FIELD5_SIZE+1];
   char theAccountNumber[FIELD6_SIZE+1];
   char theField7[FIELD7_SIZE+1];
   char theField8[FIELD8_SIZE+1];
  
   ossim_int32 theStartOffset;
   ossim_int32 theStopOffset;
};

#endif
