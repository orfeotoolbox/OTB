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
//*******************************************************************
//  $Id: ossimCadrgProjection.h,v 1.9 2005/09/09 19:23:58 gpotts Exp $
#ifndef ossimCadrgProjection_HEADER
#define ossimCadrgProjection_HEADER
#include "ossimMapProjection.h"

class ossimCadrgProjection : public ossimMapProjection
{
public:
   ossimCadrgProjection();

                       
   virtual ~ossimCadrgProjection();
   virtual ossimObject *dup()const;
   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   
   virtual ossimDpt worldToLineSample(const ossimGpt &worldPoint)    const;
   virtual ossimGpt lineSampleToWorld(const ossimDpt &projectedPoint)const;
   virtual void worldToLineSample(const ossimGpt &worldPoint,
                                  ossimDpt&       lineSample)const;
   virtual void lineSampleToWorld(const ossimDpt &projectedPoint,
                                  ossimGpt& gpt)const;

   double computeXPixConstant(double scale, long zone)const;
   double computeYPixConstant(double scale)const;
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   
   ossimString getProjectionName()const
      {
         return getClassName();
      }
      
protected:
   /*
    * Are from the ADRG Mil spec for 1:1,000,000 and
    * are scales for each zone.
    */
   static double theCadrgArcA[8];
   

   static double theOldZoneExtents[10];

   static double theNorthLimit;
   static double theSouthLimit;

   long theCadrgZone;

   ossimDpt thePixelConstant;
   double theMapScale;

   double theWidth;
   double theHeight;

   ossimDpt theUlLineSample;

   ossimGpt theUlGpt;
   ossimGpt theLlGpt;
   ossimGpt theLrGpt;
   ossimGpt theUrGpt;
   
   void computeParameters();
TYPE_DATA
};

#endif
