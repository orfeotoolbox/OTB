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
// Contains class declaration for ossimAdrData. 
//*******************************************************************
// $Id: ossimAdrData.h,v 1.5 2003/05/13 11:31:03 dburken Exp $

#ifndef ossimAdrData_HEADER
#define ossimAdrData_HEADER

#include "base/common/ossimConstants.h"

class OSSIMDLLEXPORT ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimAdrData
//*******************************************************************
class OSSIMDLLEXPORT ossimAdrData
{
public:
   ossimAdrData  ();
   ossimAdrData (const ossimKeywordlist& kwl);
   ~ossimAdrData (){};

   ossim_int32 getErrorStatus() const { return theErrorStatus; } 
   
   void print(ostream& os) const;

   friend ostream& operator<<(ostream& os, const ossimAdrData& ad);

   ossim_sint32      theLines;
   ossim_sint32      theSamples;
   ossim_float64     theUlLon;
   ossim_float64     theUlLat;
   ossim_float64     theUrLon;
   ossim_float64     theUrLat;
   ossim_float64     theLrLon;
   ossim_float64     theLrLat;
   ossim_float64     theLlLon;
   ossim_float64     theLlLat;
   ossim_float64     thePositionError;
   ossim_int32       theErrorStatus;
};
#endif
