//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
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
// Author:  David Burken <dburken@imagelinks.com>
//
// Description: Nitf support class.  Copied from ossimNitfImageBandV2_0.
// 
//********************************************************************
// $Id: ossimNitfImageBandV2_1.cpp,v 1.5 2004/11/11 14:23:33 gpotts Exp $
#include <sstream>
#include <support_data/nitf/ossimNitfImageBandV2_1.h>
#include <base/data_types/ossimString.h>


ossimNitfImageBandV2_1::ossimNitfImageBandV2_1()
{
}

ossimNitfImageBandV2_1::~ossimNitfImageBandV2_1()
{
}

void ossimNitfImageBandV2_1::setBandRepresentation(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << value.c_str();

   memcpy(theBandRepresentation, out.str().c_str(), 2);
}
