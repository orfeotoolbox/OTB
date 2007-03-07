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
// $Id: ossimImageReconstructionFilterFactory.cpp,v 1.3 2004/08/30 11:23:26 gpotts Exp $
#include "ossimImageReconstructionFilterFactory.h"
#include "base/common/ossimTrace.h"
#include "imaging/reconstruction/ossimMeanMedianFilter.h"
#include "base/common/ossimKeywordNames.h"

RTTI_DEF1(ossimImageReconstructionFilterFactory, "ossimImageReconstructionFilterFactory",  ossimImageSourceFactoryBase);

ossimImageReconstructionFilterFactory* ossimImageReconstructionFilterFactory::theInstance = 0;

ossimImageReconstructionFilterFactory::~ossimImageReconstructionFilterFactory()
{
   theInstance = NULL;
}

ossimImageReconstructionFilterFactory* ossimImageReconstructionFilterFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageReconstructionFilterFactory;
   }

   return theInstance;
}

ossimImageReconstructionFilterFactory::ossimImageReconstructionFilterFactory()
{
   theInstance = this;
}

ossimObject* ossimImageReconstructionFilterFactory::createObject(const ossimString& name)const
{
   if(STATIC_TYPE_NAME(ossimMeanMedianFilter) == name)
   {
      return new ossimMeanMedianFilter;
   }

   return 0;
}

ossimObject* ossimImageReconstructionFilterFactory::createObject(const ossimKeywordlist& kwl,
                                                                 const char* prefix)const
{
   const char* typeKw = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(typeKw)
   {
      ossimObject* obj = createObject(ossimString(typeKw).trim());

      if(obj)
      {
         obj->loadState(kwl,
                        prefix);
         
         return obj;
      }
   }
   
   return 0;
}

void ossimImageReconstructionFilterFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimMeanMedianFilter));
}
