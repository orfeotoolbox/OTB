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
// $Id: ossimNitfImageBand.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfImageBand_HEADER
#define ossimNitfImageBand_HEADER
#include <iosfwd>
#include <ossim/support_data/ossimNitfImageLut.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>

class ossimNitfImageBand : public ossimObject
{
public:
   ossimNitfImageBand();
   virtual ~ossimNitfImageBand();
   virtual void parseStream(std::istream& in)=0;
   virtual void writeStream(std::ostream& out)=0;

   /**
    * @param bandNumber Zero based band number.
    */
   virtual std::ostream& print(std::ostream& out,
                               ossim_uint32 bandNumber=0)const=0;

   virtual ossim_uint32 getNumberOfLuts()const=0;
   virtual const ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx)const=0;
   virtual ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx)=0;

TYPE_DATA;
};

#endif
