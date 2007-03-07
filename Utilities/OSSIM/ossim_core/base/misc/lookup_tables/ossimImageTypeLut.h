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
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class declaration for ImageTypeLut.  Currently has mapping of
// ouput image writes from enumeration to string and string to
// enumeration.
//
//*******************************************************************
//  $Id: ossimImageTypeLut.h,v 1.3 2002/11/25 19:16:28 gpotts Exp $

#ifndef ImageTypeLUT_HEADER
#define ImageTypeLUT_HEADER

#include "ossimLookUpTable.h"

class OSSIMDLLEXPORT ossimImageTypeLut : public ossimLookUpTable
{
public:

   ossimImageTypeLut();

   virtual ~ossimImageTypeLut(){}

   virtual ossimKeyword getKeyword() const { return IMAGE_TYPE_KW; }

   static const ossimKeyword IMAGE_TYPE_KW;
};
#endif
