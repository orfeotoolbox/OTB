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
// $Id: ossimNitfLabelHeader.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfLabelHeader_HEADER
#define ossimNitfLabelHeader_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>

class ossimDrect;

class OSSIMDLLEXPORT ossimNitfLabelHeader : public ossimObject
{
public:
   ossimNitfLabelHeader(){}
   virtual ~ossimNitfLabelHeader(){}
   
   virtual void        parseStream(std::istream &in)=0;
   virtual ossim_int32 getDisplayLevel()const=0;
   virtual ossimDrect  getImageRect()const=0;
   
TYPE_DATA
};

#endif
