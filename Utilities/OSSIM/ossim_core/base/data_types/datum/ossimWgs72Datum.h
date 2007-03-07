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
// Wgs72Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs72Ellipsoid and set the initial defaults for
// that are specific to a Wgs72Datum
//*******************************************************************
//  $ID$
#ifndef ossimWgs72Datum_HEADER
#define ossimWgs72Datum_HEADER
#include "ossimThreeParamDatum.h"

class OSSIMDLLEXPORT ossimWgs72Datum : public ossimThreeParamDatum
{
public:
   ossimWgs72Datum();

   virtual ~ossimWgs72Datum(){}
   virtual ossimGpt    shift(const ossimGpt    &aPt)const;
   virtual ossimGpt    shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt    shiftFromWgs84(const ossimGpt &aPt)const;

   
};

#endif
