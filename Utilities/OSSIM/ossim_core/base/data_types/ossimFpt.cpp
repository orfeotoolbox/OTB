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
// Description:
//
// Contains class definitions for ossimFpt.
// 
//*******************************************************************
//  $Id: ossimFpt.cpp,v 1.3 2001/10/27 20:16:11 gpotts Exp $

#include <iostream>
#include <iomanip>

#include "ossimFpt.h"
#include "ossimIpt.h"
#include "ossimDpt.h"

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimFpt::ossimFpt(const ossimIpt& pt)
   :
      x(pt.x), y(pt.y)
{
   if(pt.hasNans())
   {
      makeNan();
   }
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimFpt::ossimFpt(const ossimDpt& pt)
   :
      x(pt.x), y(pt.y)
{
   if(pt.hasNans())
   {
      makeNan();
   }
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimFpt& ossimFpt::operator=(const ossimDpt& pt)
{
   x = pt.x;
   y = pt.y;
   
   if(pt.hasNans())
   {
      makeNan();
   }
   return *this;
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimFpt& ossimFpt::operator=(const ossimIpt& pt)
{
   x = pt.x;
   y = pt.y;
   if(pt.hasNans())
   {
      makeNan();
   }
   
   return *this;
}

//*******************************************************************
// Public Method:
//*******************************************************************
void ossimFpt::print(std::ostream& os) const
{
   os << std::setiosflags(std::ios::fixed) << std::setprecision(15)
      << "( " << x << " " << y << " )";
}

//*******************************************************************
// friend function:
//*******************************************************************
std::ostream& operator<<(std::ostream& os, const ossimFpt& pt)
{
   pt.print(os);
   
   return os;
}
