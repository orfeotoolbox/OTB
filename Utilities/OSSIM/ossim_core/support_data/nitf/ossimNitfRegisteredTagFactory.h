//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level LICENSE.txt
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTagFactory.h,v 1.1 2004/09/28 19:37:23 gpotts Exp $
#ifndef ossimNitfRegisteredTagFactory_HEADER
#define ossimNitfRegisteredTagFactory_HEADER
#include <support_data/nitf/ossimNitfTagFactory.h>

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
