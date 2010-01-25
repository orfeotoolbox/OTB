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
// $Id: ossimNitfDataExtensionSegment.h 15766 2009-10-20 12:37:09Z gpotts $
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
   virtual void parseStream(std::istream &in)=0;
   virtual const ossimNitfTagInformation&  getTagInformation()const=0;

protected:
   virtual ~ossimNitfDataExtensionSegment(){}

TYPE_DATA
};

#endif
