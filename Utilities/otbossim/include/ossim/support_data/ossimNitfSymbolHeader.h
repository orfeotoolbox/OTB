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
// $Id: ossimNitfSymbolHeader.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfSymbolHeader_HEADER
#define ossimNitfSymbolHeader_HEADER

#include <iosfwd>
#include <ossim/base/ossimObject.h>

class ossimDrect;

class OSSIMDLLEXPORT ossimNitfSymbolHeader : public ossimObject
{
public:
   ossimNitfSymbolHeader(){}
   virtual ~ossimNitfSymbolHeader(){}

   virtual void        parseStream(std::istream &in)=0;
   virtual ossim_int32 getDisplayLevel()const=0;
   virtual ossimDrect  getImageRect()const=0;

TYPE_DATA
};

#endif
