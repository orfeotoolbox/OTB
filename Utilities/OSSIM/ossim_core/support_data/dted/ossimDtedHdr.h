//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the File Header Label
//               (HDR) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedHdr.h,v 1.9 2005/10/17 19:24:25 gpotts Exp $

#ifndef DtedHdr_H
#define DtedHdr_H
#include <iostream>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/data_types/ossimFilename.h>

class ossimDtedHdr : public ossimErrorStatusInterface
{
public:
   ossimDtedHdr(const ossimFilename& dted_file, ossim_int32 offset);
   ossimDtedHdr(std::istream& in, ossim_int32 offset);

   enum
   {
      HDR_LENGTH            = 80,
      HDR_ONE_LABEL_1       =  4,
      HDR_FILENAME          =  5,
      HDR_UNIVAC            = 22,
      HDR_REEL              = 28,
      HDR_FILE_NUM          = 32,
      HDR_GENERATION_NUM    = 36,
      HDR_VERSION_NUM       = 40,
      HDR_CREATION_DATE     = 42,
      HDR_EXPIRATION_DATE   = 48,
      HDR_ACCESS            = 54,
      HDR_BLOCK_COUNT       = 55,
      HDR_QUALIFIER         = 61,
      HDR_BLANKS            = 74,
      FIELD1_SIZE           = 3,
      FIELD2_SIZE           = 1,
      FIELD3_SIZE           = 17,
      FIELD4_SIZE           = 6,
      FIELD5_SIZE           = 4,
      FIELD6_SIZE           = 4,
      FIELD7_SIZE           = 4,
      FIELD8_SIZE           = 2,
      FIELD9_SIZE           = 6,
      FIELD10_SIZE          = 6,
      FIELD11_SIZE          = 1,
      FIELD12_SIZE          = 6,
      FIELD13_SIZE          = 13,
      FIELD14_SIZE          = 7
   };

   // The Recoginition Sentinel signifies if the HDR record exists.
   ossimString recoginitionSentinel() const;

   ossimString fileName()     const;
   ossimString version()      const;
   ossimString creationDate() const;
   ossim_int32 startOffset()  const;
   ossim_int32 stopOffset()   const;

   friend std::ostream& operator<<( std::ostream& os, const ossimDtedHdr& hdr);

   void parse(std::istream& in);

private:
   // Do not allow...
   ossimDtedHdr(const ossimDtedHdr& source);
   const ossimDtedHdr& operator=(const ossimDtedHdr& rhs);
   
   char theRecSen[FIELD1_SIZE+1];
   char theField2[FIELD2_SIZE+1];
   char theFilename[FIELD3_SIZE+1];
   char theField4[FIELD4_SIZE+1];
   char theField5[FIELD5_SIZE+1];
   char theField6[FIELD6_SIZE+1];
   char theVersion[FIELD7_SIZE+1];
   char theCreationDate[FIELD8_SIZE+1];
   char theField9[FIELD9_SIZE+1];
   char theField10[FIELD10_SIZE+1];
   char theField11[FIELD11_SIZE+1];
   char theField12[FIELD12_SIZE+1];
   char theField13[FIELD13_SIZE+1];
   char theField14[FIELD14_SIZE+1];
   
   ossim_int32 theStartOffset;
   ossim_int32 theStopOffset;
};

#endif
