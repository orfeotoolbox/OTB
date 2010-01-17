//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL  see top level LICENSE.txt
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpfTagFactory.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/support_data/ossimNitfRpfTagFactory.h>
#include <ossim/support_data/ossimNitfEmbeddedRpfHeader.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimNitfRpfTagFactory, "ossimNitfRpfTagFactory", ossimNitfTagFactory);

ossimNitfRpfTagFactory*   ossimNitfRpfTagFactory::theInstance = NULL;

static const ossimString REGISTERED_RPF_HEADER_TAG = "RPFHDR";
static const ossimString REGISTERED_RPF_DES_TAG    = "RPFDES";

ossimNitfRpfTagFactory::ossimNitfRpfTagFactory()
{
   theInstance = this;
}

ossimNitfRpfTagFactory::~ossimNitfRpfTagFactory()
{
   theInstance = NULL;
}

ossimNitfRpfTagFactory* ossimNitfRpfTagFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimNitfRpfTagFactory;
   }
   
   return theInstance;
}


ossimRefPtr<ossimNitfRegisteredTag> ossimNitfRpfTagFactory::create(const ossimString &tagName)const
{
   ossimString temp(ossimString(tagName).trim().upcase());
   
   if(temp == REGISTERED_RPF_HEADER_TAG)
   {
      return new ossimNitfEmbeddedRpfHeader;
   }
   if(temp == REGISTERED_RPF_DES_TAG)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING  ossimNitfRpfTagFactory::create: Reader for REGISTERED_RPF_DES_TAG not implemented yet" << std::endl;
   }
   
   return NULL;
}
