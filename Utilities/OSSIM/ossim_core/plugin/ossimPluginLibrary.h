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
//
//*************************************************************************
// $Id: ossimPluginLibrary.h,v 1.3 2004/05/19 18:37:37 dburken Exp $
#ifndef ossimPluginLibrary_HEADER
#define ossimPluginLibrary_HEADER
#include "plugin/ossimSharedObjectBridge.h"
#include "ossimDynamicLibrary.h"

class OSSIMDLLEXPORT ossimPluginLibrary : public ossimDynamicLibrary
{
public:
   ossimPluginLibrary();
   ossimPluginLibrary(const ossimString& name);
   virtual ~ossimPluginLibrary();
   void initialize();
   void finalize();
   ossimString getDescription()const;
   void getClassNames(std::vector<ossimString>& classNames)const;
   
protected:
   ossimSharedObjectInfo* theInfo;

TYPE_DATA
};

#endif 
