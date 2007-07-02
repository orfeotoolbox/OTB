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
// $Id: ossimNitfTextHeader.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfTextHeader_HEADER
#define ossimNitfTextHeader_HEADER

#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimNitfTextHeader : public ossimObject
{
public:
   ossimNitfTextHeader(){}
   virtual ~ossimNitfTextHeader(){}
   
   virtual void parseStream(std::istream &in)=0;
   
TYPE_DATA
};

#endif
