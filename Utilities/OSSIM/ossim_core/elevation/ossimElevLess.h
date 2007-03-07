//*****************************************************************************
// FILE: ossimElevLess.h
//
// Copyright (C) 2002 ImageLinks, Inc.
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
// Function object (functor) to for stl's sort method for sorting
// a vector of ossimElevSource* by best post spacing.
//*****************************************************************************
//  $Id: ossimElevLess.h,v 1.3 2005/11/18 20:24:49 gpotts Exp $

#ifndef ossimElevLess_HEADER
#define ossimElevLess_HEADER

#include <functional>

#include "ossimElevSource.h"

class ossimElevLess :
           public std::binary_function<const ossimRefPtr<ossimElevSource>,
                  const ossimRefPtr<ossimElevSource>, bool>
{
public:
   bool operator() (const ossimRefPtr<ossimElevSource> a, const ossimRefPtr< ossimElevSource> b) const
   {
      return (a->getMeanSpacingMeters() < b->getMeanSpacingMeters());
   }
};

#endif
