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
// Author: Garrett Potts  <gpotts@imagelinks.com>
//
//*************************************************************************
// $Id: ossimImageSourceFactoryBase.h,v 1.4 2004/08/31 19:15:57 gpotts Exp $
#ifndef ossimImageSourceFactoryBase_HEADER
#define ossimImageSourceFactoryBase_HEADER
#include "base/factory/ossimObjectFactory.h"
class ossimImageSource;

class OSSIMDLLEXPORT ossimImageSourceFactoryBase : public ossimObjectFactory
{
public:
   /*!
    * Convenient conversion method.  Gurantees an ossimImageSource is returned.  Returns
    * NULL otherwise
    */
   virtual ossimImageSource* createImageSource(const ossimString& name)const;
   virtual ossimImageSource* createImageSource(const ossimKeywordlist& kwl,
                                               const char* prefix=0)const;
   
protected:
   // Hide from use.
   ossimImageSourceFactoryBase();
   ossimImageSourceFactoryBase(const ossimImageSourceFactoryBase&);
   const ossimImageSourceFactoryBase& operator=(const ossimImageSourceFactoryBase&);
};

#endif
