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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimImageViewTransformFactory.cpp,v 1.3 2002/08/27 17:30:11 gpotts Exp $
#include "ossimImageViewTransformFactory.h"
#include "projections/ossimImageViewProjectionTransform.h"
#include "projections/ossimImageViewAffineTransform.h"
#include "base/data_types/ossimKeywordlist.h"

RTTI_DEF1(ossimImageViewTransformFactory, "ossimImageViewTransformFactory", ossimObject)

ossimImageViewTransformFactory* ossimImageViewTransformFactory::theInstance = 0;

ossimImageViewTransformFactory* ossimImageViewTransformFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageViewTransformFactory;
   }

   return theInstance;
}

ossimImageViewTransform* ossimImageViewTransformFactory::createTransform(const ossimKeywordlist& kwl,
                                                                         const char* prefix)
{
   ossimImageViewTransform* result=NULL;
   
   const char *lookup = kwl.find(prefix, "type");

   if(lookup)
   {
      if(ossimString(lookup) == STATIC_TYPE_NAME(ossimImageViewProjectionTransform))
      {
         result = new ossimImageViewProjectionTransform;
      }
      if(ossimString(lookup) == STATIC_TYPE_NAME(ossimImageViewAffineTransform))
      {
         result = new ossimImageViewAffineTransform;
      }
   }
   if(result)
   {
      result->loadState(kwl, prefix);
   }
   
   return result;
}
   
