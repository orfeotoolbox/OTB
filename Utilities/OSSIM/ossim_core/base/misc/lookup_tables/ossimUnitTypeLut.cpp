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
//*******************************************************************
//  $Id: ossimUnitTypeLut.cpp,v 1.5 2005/06/15 14:15:22 gpotts Exp $
#include "ossimUnitTypeLut.h"
static const int TABLE_SIZE = 13;
ossimUnitTypeLut::ossimUnitTypeLut()
  :ossimLookUpTable(TABLE_SIZE)
{
   theTable[0].theKey   = OSSIM_UNIT_UNKNOWN;
   theTable[0].theValue = "unknown";
   theTable[1].theKey   = OSSIM_METERS;
   theTable[1].theValue = "meters";
   theTable[2].theKey   = OSSIM_FEET;
   theTable[2].theValue = "feet";
   theTable[3].theKey   = OSSIM_US_SURVEY_FEET;
   theTable[3].theValue = "us_survey_feet";
   theTable[4].theKey   = OSSIM_DEGREES;
   theTable[4].theValue = "degrees";
   theTable[5].theKey   = OSSIM_RADIANS;
   theTable[5].theValue = "radians";
   theTable[6].theKey   = OSSIM_NAUTICAL_MILES;
   theTable[6].theValue = "nautical_miles";
   theTable[7].theKey   = OSSIM_SECONDS;
   theTable[7].theValue = "seconds";
   theTable[8].theKey   = OSSIM_MINUTES;
   theTable[8].theValue = "minutes";
   theTable[9].theKey   = OSSIM_PIXEL;
   theTable[9].theValue = "pixel";
   theTable[10].theKey   = OSSIM_MILES;
   theTable[10].theValue = "miles";
   theTable[11].theKey   = OSSIM_MILLIMETERS;
   theTable[11].theValue = "millimeters";
   theTable[12].theKey   = OSSIM_MICRONS;
   theTable[12].theValue = "microns";
}
