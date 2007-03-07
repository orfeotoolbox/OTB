//*****************************************************************************
// FILE: ossimElevCellHandlerFactory.h
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// DESCRIPTION:
//   Contains declaration of class ossimElevCellHandlerFactory. This factory
//   class instantiates the proper ossimElevCellHandler given a file name.
//
// SOFTWARE HISTORY:
//>
//   26Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#ifndef ossimElevCellHandlerFactory_HEADER
#define ossimElevCellHandlerFactory_HEADER

#include "base/common/ossimFactoryBaseTemplate.h"

class ossimElevCellHandler;
class ossimGpt;

/*!****************************************************************************
 *
 * CLASS:  ossimElevCellHandlerFactory
 *
 *****************************************************************************/
class ossimElevCellHandlerFactory :
   public ossimFactoryBase <ossimElevCellHandler>
{
 public:
   static ossimElevCellHandlerFactory* instance();
   
   virtual ossimElevCellHandler* create(const ossimString&) const;
   virtual ossimElevCellHandler* create(const ossimKeywordlist& kwl,
                                        const char* prefix) const;
   
   virtual list<ossimString> getList() const;
   
 protected:
   ossimElevCellHandlerFactory();

   static ossimElevCellHandlerFactory*  theInstance;
};

#endif
