//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level LICENSE.txt
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTagFactory.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfRegisteredTagFactory_HEADER
#define ossimNitfRegisteredTagFactory_HEADER
#include <ossim/support_data/ossimNitfTagFactory.h>

class ossimNitfRegisteredTagFactory : public ossimNitfTagFactory
{
public:
   ossimNitfRegisteredTagFactory();
   virtual ~ossimNitfRegisteredTagFactory();
   static ossimNitfRegisteredTagFactory* instance();
   
   virtual ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const;

protected:
   static ossimNitfRegisteredTagFactory* theInstance;
TYPE_DATA   
};

#endif
