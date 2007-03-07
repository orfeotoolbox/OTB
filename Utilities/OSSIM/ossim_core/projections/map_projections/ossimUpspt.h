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
// Author:  Garrett Potts(gpotts@imagelinks.com)
//
// Description:
//
// This is the Universal Polar Stereographic (UPS) point.  Allows
// easy onversion between different coordinates.
//*******************************************************************
//  $Id: ossimUpspt.h,v 1.2 2001/08/15 03:05:49 okramer Exp $

#ifndef ossimUpspt_HEADER
#define ossimUpspt_HEADER

class ossimUtmpt;
class ossimGpt;
class ossimEcefPoint;
class ossimDatum;

class ossimUpspt
{
public:
   ossimUpspt(const ossimGpt &aPt);
   ossimUpspt(const ossimEcefPoint &aPt);

   char hemisphere()const{return theHemisphere;}
   double easting()const {return theEasting;}
   double northing()const{return theNorthing;}
   const ossimDatum* datum()const{return theDatum;}
   
private:
   char    theHemisphere;
   double  theEasting;
   double  theNorthing;
   ossimDatum  *theDatum;

   void convertFromGeodetic(const ossimGpt &aPt);
};

#endif
