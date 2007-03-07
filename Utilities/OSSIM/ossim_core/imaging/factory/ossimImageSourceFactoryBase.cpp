//*******************************************************************
// Copyright (C) 2004 ImageLinks Inc. All rights reserved.
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
// Author: David Burken <dburken@imagelinks.com>
//
//*************************************************************************
// $Id: ossimImageSourceFactoryBase.cpp,v 1.2 2004/08/26 19:14:35 gpotts Exp $

#include <imaging/factory/ossimImageSourceFactoryBase.h>
#include <imaging/ossimImageSource.h>
ossimImageSourceFactoryBase::ossimImageSourceFactoryBase()
   : ossimObjectFactory()
{}

ossimImageSourceFactoryBase::ossimImageSourceFactoryBase(const ossimImageSourceFactoryBase&)
   : ossimObjectFactory()
{}

const ossimImageSourceFactoryBase& ossimImageSourceFactoryBase::operator=(const ossimImageSourceFactoryBase&)
{
   return *this;
}

ossimImageSource* ossimImageSourceFactoryBase::createImageSource(const ossimString& name)const
{
   ossimObject* result =createObject(name);
   
   if(PTR_CAST(ossimImageSource, result))
   {
      return (ossimImageSource*)result;
   }

   if(result)
   {
      delete result;
      result = NULL;
   }

   return (ossimImageSource*)NULL;
}

ossimImageSource* ossimImageSourceFactoryBase::createImageSource(const ossimKeywordlist& kwl,
                                                                 const char* prefix)const
{
   ossimObject* result =createObject(kwl, prefix);
   
   if(PTR_CAST(ossimImageSource, result))
   {
      return (ossimImageSource*)result;
   }

   if(result)
   {
      delete result;
      result = NULL;
   }

   return (ossimImageSource*)NULL;
}
