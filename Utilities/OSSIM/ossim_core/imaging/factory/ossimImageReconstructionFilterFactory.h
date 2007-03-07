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
// $Id: ossimImageReconstructionFilterFactory.h,v 1.1 2004/01/21 14:57:25 gpotts Exp $
#ifndef ossimImageReconstructionFilterFactory_HEADER
#define ossimImageReconstructionFilterFactory_HEADER
#include "ossimImageSourceFactoryBase.h"

class ossimImageReconstructionFilterFactory : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimImageReconstructionFilterFactory();
   static ossimImageReconstructionFilterFactory* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   
protected:
   ossimImageReconstructionFilterFactory(); // hide
   ossimImageReconstructionFilterFactory(const ossimImageReconstructionFilterFactory&){}//hide
   void operator = (ossimImageReconstructionFilterFactory&){}// hide

   static ossimImageReconstructionFilterFactory* theInstance;
   
TYPE_DATA
};
#endif
