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
//  $Id: ossimImageViewTransformFactory.h,v 1.1 2001/10/26 13:49:04 gpotts Exp $
#ifndef ossimImageViewTransformFactory_HEADER
#define ossimImageViewTransformFactory_HEADER
#include "base/common/ossimObject.h"

class ossimImageViewTransform;
class ossimKeywordlist;

class ossimImageViewTransformFactory : public ossimObject
{
public:
   static ossimImageViewTransformFactory* instance();
   virtual ossimImageViewTransform* createTransform(const ossimKeywordlist& kwl,
                                                    const char* prefix = 0);
   
protected:
   ossimImageViewTransformFactory(){}//hide
   ossimImageViewTransformFactory(const ossimImageViewTransformFactory&){}
   void operator =(const ossimImageViewTransformFactory&){}

   static ossimImageViewTransformFactory* theInstance;

TYPE_DATA
};

#endif
