//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageLut.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfImageLut_HEADER
#define ossimNitfImageLut_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>

//using namespace std;

class ossimNitfImageLut : public ossimObject
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNitfImageLut &data);
   ossimNitfImageLut();
   virtual ~ossimNitfImageLut();
   virtual void parseStream(std::istream &in)=0;
   virtual std::ostream& print(std::ostream& out)const=0;
   virtual ossim_uint32 getNumberOfEntries()const=0;
   virtual void setNumberOfEntries(ossim_uint32 numberOfEntries)=0;
   virtual ossim_uint8 getValue(ossim_uint32 idx)const=0;

TYPE_DATA;
};
#endif
