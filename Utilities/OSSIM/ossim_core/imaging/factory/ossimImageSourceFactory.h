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
// $Id: ossimImageSourceFactory.h,v 1.5 2004/01/30 23:30:24 dburken Exp $
#ifndef ossimImageSourceFactory_HEADER
#define ossimImageSourceFactory_HEADER
#include "ossimImageSourceFactoryBase.h"

class ossimImageSourceFactory : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimImageSourceFactory();
   static ossimImageSourceFactory* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   // Hide from use.
   ossimImageSourceFactory();
   ossimImageSourceFactory(const ossimImageSourceFactory&);
   const ossimImageSourceFactory& operator=(ossimImageSourceFactory&);

   static ossimImageSourceFactory* theInstance;
TYPE_DATA
};
#endif
