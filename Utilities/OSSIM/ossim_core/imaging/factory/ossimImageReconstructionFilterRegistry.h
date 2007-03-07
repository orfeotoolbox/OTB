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
//
//*************************************************************************
// $Id: ossimImageReconstructionFilterRegistry.h,v 1.1 2004/01/21 14:57:25 gpotts Exp $
#ifndef ossimImageReconstructionFilterRegistry_HEADER
#define ossimImageReconstructionFilterRegistry_HEADER
#include "ossimImageSourceFactoryBase.h"

class ossimImageReconstructionFilterRegistry : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimImageReconstructionFilterRegistry();
   static ossimImageReconstructionFilterRegistry* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   void registerFactory(ossimImageSourceFactoryBase* factory);
   void unregisterFactory(ossimImageSourceFactoryBase* factory);
   bool findFactory(ossimImageSourceFactoryBase* factory)const;
   
protected:
   ossimImageReconstructionFilterRegistry(); // hide
   ossimImageReconstructionFilterRegistry(const ossimImageReconstructionFilterRegistry&){}//hide
   void operator = (ossimImageReconstructionFilterRegistry&){}// hide
   
   static ossimImageReconstructionFilterRegistry* theInstance;
   std::vector<ossimImageSourceFactoryBase*> theFactoryList;
TYPE_DATA
};

#endif
