//*******************************************************************
//
// LICENSE: LGPL  see top level LICENSE.txt
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpfTagFactory.cpp 16997 2010-04-12 18:53:48Z dburken $
#include <ossim/support_data/ossimNitfRpfTagFactory.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/base/ossimNotify.h>

RTTI_DEF1(ossimNitfRpfTagFactory, "ossimNitfRpfTagFactory", ossimNitfTagFactory);

ossimNitfRpfTagFactory* ossimNitfRpfTagFactory::theInstance = 0;

static const ossimString REGISTERED_RPF_HEADER_TAG = "RPFHDR";
static const ossimString REGISTERED_RPF_DES_TAG    = "RPFDES";

ossimNitfRpfTagFactory::ossimNitfRpfTagFactory()
{
   theInstance = this;
}

ossimNitfRpfTagFactory::~ossimNitfRpfTagFactory()
{
   theInstance = 0;
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
      return new ossimRpfHeader;
   }
   if(temp == REGISTERED_RPF_DES_TAG)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING  ossimNitfRpfTagFactory::create: Reader for REGISTERED_RPF_DES_TAG not implemented yet" << std::endl;
   }
   
   return 0;
}
