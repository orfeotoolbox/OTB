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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// This is the Universal Polar Stereographic (UPS) point.  Allows
// easy onversion between different coordinates.
//*******************************************************************
//  $Id: ossimUpspt.cpp,v 1.2 2001/08/15 03:05:49 okramer Exp $
#include "ossimUpspt.h"
#include "ossimUtmpt.h"
#include "ossimUps.h"

#include "base/data_types/ossimGpt.h"
#include "base/data_types/ossimEcefPoint.h"
#include "base/data_types/ellipse/ossimEllipsoid.h"
#include "base/data_types/datum/ossimDatum.h"

ossimUpspt::ossimUpspt(const ossimGpt &aPt)
{
   convertFromGeodetic(aPt);
}

ossimUpspt::ossimUpspt(const ossimEcefPoint &aPt)
{
   convertFromGeodetic(ossimGpt(aPt));
}


void ossimUpspt::convertFromGeodetic(const ossimGpt &aPt)
{
   const ossimDatum *aDatum = aPt.datum();

   if(aDatum)
   {
      Set_UPS_Parameters(aDatum->ellipsoid()->a(),
                         aDatum->ellipsoid()->b());
      
      Convert_Geodetic_To_UPS(aPt.latr(),
                              aPt.lonr(),
                              &theHemisphere,
                              &theEasting,
                              &theNorthing);
      theDatum = const_cast<ossimDatum*>(aDatum);
   }
   else
   {
      //ERROR: should never happen
   }
}
