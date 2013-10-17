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
// $Id: ossimNitfDataExtensionSegment.cpp 22418 2013-09-26 15:01:12Z gpotts $

#include <ostream>

#include <ossim/support_data/ossimNitfDataExtensionSegment.h>
RTTI_DEF2(ossimNitfDataExtensionSegment,
          "ossimNitfDataExtensionSegment",
          ossimObject, ossimPropertyInterface)


const ossimString ossimNitfDataExtensionSegment::DE_KW = "de";
const ossimString ossimNitfDataExtensionSegment::DESVER_KW = "desver";
const ossimString ossimNitfDataExtensionSegment::DESOFLW_KW = "desoflw";
const ossimString ossimNitfDataExtensionSegment::DESITEM_KW = "desitem";
const ossimString ossimNitfDataExtensionSegment::DESSHL_KW = "desshl";
const ossimString ossimNitfDataExtensionSegment::DESSHF_KW = "desshf";
const ossimString ossimNitfDataExtensionSegment::DESDATA_KW = "desdata";
const ossimString ossimNitfDataExtensionSegment::DECLAS_KW = "declas";
const ossimString ossimNitfDataExtensionSegment::DESCODE_KW = "descode";
const ossimString ossimNitfDataExtensionSegment::DESCTLH_KW = "desctlh";
const ossimString ossimNitfDataExtensionSegment::DESREL_KW = "desrel";
const ossimString ossimNitfDataExtensionSegment::DESCAUT_KW = "descaut";
const ossimString ossimNitfDataExtensionSegment::DESCTLN_KW = "desctln";

std::ostream& operator <<(std::ostream &out,
                          const ossimNitfDataExtensionSegment &data)
{
   data.print(out);
   
   return out;
}

void ossimNitfDataExtensionSegment::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(DE_KW);
   propertyNames.push_back(DESVER_KW);
   propertyNames.push_back(DESOFLW_KW);
   propertyNames.push_back(DESITEM_KW);
   propertyNames.push_back(DESSHL_KW);
   propertyNames.push_back(DESSHF_KW);
   propertyNames.push_back(DESDATA_KW);
   propertyNames.push_back(DECLAS_KW);
   propertyNames.push_back(DESCODE_KW);
   propertyNames.push_back(DESCTLH_KW);
   propertyNames.push_back(DESREL_KW);
   propertyNames.push_back(DESCAUT_KW);
   propertyNames.push_back(DESCTLN_KW);
}
