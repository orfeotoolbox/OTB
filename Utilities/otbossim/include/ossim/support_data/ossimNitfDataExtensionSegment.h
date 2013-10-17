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
// $Id: ossimNitfDataExtensionSegment.h 22418 2013-09-26 15:01:12Z gpotts $
#ifndef ossimNitfDataExtensionSegment_HEADER
#define ossimNitfDataExtensionSegment_HEADER

#include <iosfwd>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimProperty.h>

#include <vector>

class ossimNitfTagInformation;

class OSSIMDLLEXPORT ossimNitfDataExtensionSegment : public ossimPropertyInterface, public ossimObject
{
public:
   friend std::ostream& operator <<(std::ostream &out,
                                    const ossimNitfDataExtensionSegment &data);
   ossimNitfDataExtensionSegment(){}
   virtual ~ossimNitfDataExtensionSegment(){}
   virtual void parseStream(std::istream &in, ossim_uint64 dataLength)=0;
   virtual const std::vector<ossimNitfTagInformation>&  getTagList()const=0;

   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   static const ossimString DE_KW;
   static const ossimString DESVER_KW;
   static const ossimString DESOFLW_KW;
   static const ossimString DESITEM_KW;
   static const ossimString DESSHL_KW;
   static const ossimString DESSHF_KW;
   static const ossimString DESDATA_KW;
   static const ossimString DECLAS_KW;
   static const ossimString DESCODE_KW;
   static const ossimString DESCTLH_KW;
   static const ossimString DESREL_KW;
   static const ossimString DESCAUT_KW;
   static const ossimString DESCTLN_KW;

TYPE_DATA
};

#endif
