//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Accuracy Description
//               (ACC) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedAcc.h,v 1.9 2005/10/17 19:24:25 gpotts Exp $

#ifndef ossimDtedAcc_H
#define ossimDtedAcc_H
#include <iostream>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/data_types/ossimFilename.h>


class ossimDtedAcc : public ossimErrorStatusInterface
{
public:
   ossimDtedAcc(const ossimFilename& dted_file, ossim_int32 offset);
   ossimDtedAcc(std::istream& in, ossim_int32 offset);

   enum
   {
      ACC_LENGTH = 2700,
      ACC_ABSOLUTE_CE = 4,
      ACC_ABSOLUTE_LE = 8,
      ACC_RELATIVE_CE = 12,
      ACC_RELATIVE_LE = 16,
      ACC_RESERVED_1 = 20,
      ACC_RESERVED_2 = 24,
      ACC_RESERVED_3 = 25,
      ACC_MULTIPLE_FLAG = 56,
      ACC_RESERVED = 58,
      FIELD1_SIZE           = 3,
      FIELD2_SIZE           = 4,
      FIELD3_SIZE           = 4,
      FIELD4_SIZE           = 4,
      FIELD5_SIZE           = 4,
      FIELD6_SIZE           = 4,
      FIELD7_SIZE           = 1,
      FIELD8_SIZE           = 31,
      FIELD9_SIZE           = 2,
      FIELD10_SIZE          = 4,
      FIELD11_SIZE          = 4,
      FIELD12_SIZE          = 4,
      FIELD13_SIZE          = 4,
      FIELD14_SIZE          = 2,
      FIELD15_SIZE          = 9,
      FIELD16_SIZE          = 10,
      FIELD17_SIZE          = 18,
      FIELD18_SIZE          = 69
   };

   // The Recoginition Sentinel signifies if the ACC record exists.

   ossim_int32 absCE() const;
   ossim_int32 absLE() const;
   ossim_int32 relCE() const;
   ossim_int32 relLE() const;
   
   ossim_int32 startOffset() const;
   ossim_int32 stopOffset()  const;
   
   friend std::ostream& operator<<( std::ostream& os, const ossimDtedAcc& acc);
   void parse(std::istream& in);

private:
   // Do not allow...
   ossimDtedAcc(const ossimDtedAcc& source);
   const ossimDtedAcc& operator=(const ossimDtedAcc& rhs);

   
   char theRecSen[FIELD1_SIZE+1];
   char theAbsoluteCE[FIELD2_SIZE+1];
   char theAbsoluteLE[FIELD3_SIZE+1];
   char theRelativeCE[FIELD4_SIZE+1];
   char theRelativeLE[FIELD5_SIZE+1];
   char theField6[FIELD6_SIZE+1];
   char theField7[FIELD7_SIZE+1];
   char theField8[FIELD8_SIZE+1];
   char theField9[FIELD9_SIZE+1];
   char theField10[FIELD10_SIZE+1];
   char theField11[FIELD11_SIZE+1];
   char theField12[FIELD12_SIZE+1];
   char theField13[FIELD13_SIZE+1];
   char theField14[FIELD14_SIZE+1];
   char theField15[FIELD15_SIZE+1];
   char theField16[FIELD16_SIZE+1];
   char theField17[FIELD17_SIZE+1];
   
   ossim_int32 theStartOffset;
   ossim_int32 theStopOffset;
};

#endif
