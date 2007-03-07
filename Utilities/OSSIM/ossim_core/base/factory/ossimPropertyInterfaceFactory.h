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
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimPropertyInterfaceFactory.h,v 1.6 2002/11/25 19:16:28 gpotts Exp $
#ifndef ossimPropertyInterfaceFactory_HEADER
#define ossimPropertyInterfaceFactory_HEADER


#include "base/common/ossimRtti.h"
#include "base/factory/ossimObjectFactory.h"
class OSSIMDLLEXPORT ossimObject;
class OSSIMDLLEXPORT ossimPropertyInterface;

class OSSIMDLLEXPORT ossimPropertyInterfaceFactory : public ossimObjectFactory
{
public:
   ossimPropertyInterfaceFactory()
      :ossimObjectFactory()
      {}
   virtual ~ossimPropertyInterfaceFactory()
      {}

   virtual ossimPropertyInterface* createInterface( const ossimObject* obj ) const = 0;

   
TYPE_DATA
};

#endif
