//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level LICENSE.txt
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagFactory.h,v 1.2 2004/09/28 15:35:30 gpotts Exp $
#ifndef ossimNitfTagFactory_HEADER
#define ossimNitfTagFactory_HEADER
#include <base/common/ossimObject.h>
#include <base/common/ossimRefPtr.h>

class ossimNitfRegisteredTag;

class ossimNitfTagFactory : public ossimObject
{
public:
   ossimNitfTagFactory();
   virtual ~ossimNitfTagFactory();
   virtual ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const=0;
private:
   /*!
    * Hide this.
    */
   ossimNitfTagFactory(const ossimNitfTagFactory &rhs){}

   /*!
    * Hide this.
    */ 
   ossimNitfTagFactory& operator =(const ossimNitfTagFactory &rhs){return *this;}
TYPE_DATA
};
#endif
