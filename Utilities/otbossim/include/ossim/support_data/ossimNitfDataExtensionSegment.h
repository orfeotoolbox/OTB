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
// $Id: ossimNitfDataExtensionSegment.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfDataExtensionSegment_HEADER
#define ossimNitfDataExtensionSegment_HEADER

#include <iosfwd>
#include <ossim/base/ossimObject.h>
class ossimNitfTagInformation;

class OSSIMDLLEXPORT ossimNitfDataExtensionSegment : public ossimObject
{
public:
   friend std::ostream& operator <<(std::ostream &out,
                                    const ossimNitfDataExtensionSegment &data);
   ossimNitfDataExtensionSegment(){}
   virtual ~ossimNitfDataExtensionSegment(){}
   virtual void parseStream(std::istream &in)=0;
   virtual const ossimNitfTagInformation&  getTagInformation()const=0;
   
TYPE_DATA
};

#endif
