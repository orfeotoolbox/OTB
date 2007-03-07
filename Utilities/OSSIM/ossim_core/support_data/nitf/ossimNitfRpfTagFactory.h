//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL  see top level LICENSE.txt
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpfTagFactory.h,v 1.3 2004/09/28 15:35:30 gpotts Exp $
#ifndef ossimNitfRpfTagFactory_HEADER
#define ossimNitfRpfTagFactory_HEADER
#include <support_data/nitf/ossimNitfTagFactory.h>
class ossimNitfRegisteredTag;

class ossimNitfRpfTagFactory : public ossimNitfTagFactory
{
public:
   virtual ~ossimNitfRpfTagFactory();
   static ossimNitfRpfTagFactory* instance();
   virtual ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const;
   
protected:
   ossimNitfRpfTagFactory();

private:
   static ossimNitfRpfTagFactory* theInstance;
   /*!
    * Hide this.
    */
   ossimNitfRpfTagFactory(const ossimNitfRpfTagFactory &rhs){}

   /*!
    * Hide this.
    */ 
   ossimNitfRpfTagFactory& operator =(const ossimNitfRpfTagFactory &rhs){return *this;}
TYPE_DATA
};
#endif
