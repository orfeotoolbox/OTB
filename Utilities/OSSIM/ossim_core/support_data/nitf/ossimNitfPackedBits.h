//*******************************************************************
// Copyright (C) 2004 Garrett Potts.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for ossimNitfTileSource.
//*******************************************************************
//  $Id: ossimNitfPackedBits.h,v 1.2 2004/11/11 14:23:34 gpotts Exp $
#ifndef ossimNitfPackedBits_HEADER
#define ossimNitfPackedBits_HEADER
#include <base/common/ossimConstants.h>
/**
 * This class will allow access to individual packed pixels given an offset and
 * return as an ossim_uint32 type.
 */
class OSSIM_DLL ossimNitfPackedBits
{
public:
   ossimNitfPackedBits(ossim_uint8* packedBits, ossim_uint16 bits);
   /**
    * Extract out the value at offset indicated by idx and convert the
    * the vale to an ossim_uint32.  This class only supports packed bits
    * less than 32.
    */
   ossim_uint32 getValueAsUint32(ossim_uint32 idx)const;
   
protected:
   ossim_uint8* thePackedBits;
   ossim_uint16 theBits;
   ossim_uint16 theBytes;
   ossim_uint16 theBitsLeft;
   
};

#endif
