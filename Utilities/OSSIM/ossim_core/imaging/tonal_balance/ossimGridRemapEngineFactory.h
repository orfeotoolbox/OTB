//*****************************************************************************
// FILE: ossimGridRemapEngineFactory.h
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
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimGridRemapEngineFactory
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGridRemapEngineFactory.h,v 1.2 2001/12/05 00:04:31 okramer Exp $

#ifndef ossimGridRemapEngineFactory_HEADER
#define ossimGridRemapEngineFactory_HEADER

class ossimGridRemapEngine;

/*!****************************************************************************
 *
 * CLASS:  ossimGridRemapEngineFactory (SIMPLETON)
 *
 *****************************************************************************/
class ossimGridRemapEngineFactory
{
public:
   /*!
    * Attempts to create an instance of the Product given a Specifier. Returns
    * successfully constructed product or NULL.
    */
   static ossimGridRemapEngine* create(const char* spec);
   
protected:
   ossimGridRemapEngineFactory() {}
};

#endif
