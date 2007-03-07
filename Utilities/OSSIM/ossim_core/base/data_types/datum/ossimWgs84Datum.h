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
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// ossimWgs84Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs84Ellipsoid and set the initial defaults for
// that are specific to a ossimWgs84Datum
//*******************************************************************
//  $Id: ossimWgs84Datum.h,v 1.4 2002/09/01 19:06:34 gpotts Exp $

#ifndef ossimWgs84Datum_HEADER
#define ossimWgs84Datum_HEADER
#include "ossimThreeParamDatum.h"

class OSSIMDLLEXPORT ossimWgs84Datum : public ossimThreeParamDatum
{
public:
   ossimWgs84Datum();
   virtual ~ossimWgs84Datum(){}
   
   virtual ossimGpt    shift(const ossimGpt    &aPt)const;

   virtual ossimGpt    shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt    shiftFromWgs84(const ossimGpt&aPt)const;

protected:
   
};

#endif
