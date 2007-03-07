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
//*******************************************************************
//  $Id: ossimImageHandlerFactoryBase.h,v 1.3 2005/05/20 20:25:19 dburken Exp $

#ifndef ossimImageHandlerFactoryBase_HEADER
#define ossimImageHandlerFactoryBase_HEADER

#include "base/data_types/ossimString.h"
#include "base/factory/ossimObjectFactory.h"

class ossimImageHandler;
class ossimFilename;
class ossimKeywordlist;

class OSSIM_DLL ossimImageHandlerFactoryBase : public ossimObjectFactory
{
public:
   virtual ossimImageHandler* open(const ossimFilename& fileName)const = 0;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const = 0;

TYPE_DATA
};

#endif
