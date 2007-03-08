//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageLutV2_0.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfImageLutV2_0_HEADER
#define ossimNitfImageLutV2_0_HEADER
#include <ossim/support_data/ossimNitfImageLut.h>

#include <vector>

class ossimNitfImageLutV2_0 : public ossimNitfImageLut
{
public:
   ossimNitfImageLutV2_0();
   virtual ~ossimNitfImageLutV2_0();
   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream& out)const;
   virtual ossim_uint32 getNumberOfEntries()const;
   virtual void setNumberOfEntries(ossim_uint32 numberOfEntries);
   virtual ossim_uint8 getValue(ossim_uint32 idx)const;
   
protected:
   std::vector<ossim_uint8> theData;
   ossim_uint32 theNumberOfEntries;
};
#endif
