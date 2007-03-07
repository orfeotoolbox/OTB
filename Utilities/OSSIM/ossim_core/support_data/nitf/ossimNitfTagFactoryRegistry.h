//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagFactoryRegistry.h,v 1.2 2004/09/28 15:35:30 gpotts Exp $
#ifndef ossimNitfTagFactoryRegistry_HEADER
#define ossimNitfTagFactoryRegistry_HEADER
#include <vector>
#include <support_data/nitf/ossimNitfRegisteredTag.h>
class ossimString;
class ossimNitfTagFactory;

class ossimNitfTagFactoryRegistry
{
public:
   virtual ~ossimNitfTagFactoryRegistry();
   void registerFactory(ossimNitfTagFactory* aFactory);
   void unregisterFactory(ossimNitfTagFactory* aFactory);
   
   static ossimNitfTagFactoryRegistry* instance();
   
   ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const;
   bool exists(ossimNitfTagFactory* factory)const;
   
protected:
   ossimNitfTagFactoryRegistry();

private:
   ossimNitfTagFactoryRegistry(const ossimNitfTagFactoryRegistry &){} // hide
   const ossimNitfTagFactoryRegistry & operator =(const ossimNitfTagFactoryRegistry &){return *this;} // hide

   void initializeDefaults();

   static ossimNitfTagFactoryRegistry* theInstance;
   
   std::vector<ossimNitfTagFactory*> theFactoryList;
};

#endif
