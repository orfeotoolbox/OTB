//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc.
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
// Author:  Garrett Potts <gpotts@imagelinks.com>
//
//*******************************************************************
//  $Id: ossimAtCorrGridRemapper.h,v 1.3 2003/10/07 17:10:02 gpotts Exp $
#ifndef ossimAtCorrGridRemapper_HEADER
#define ossimAtCorrGridRemapper_HEADER
#include "ossimAtCorrRemapper.h"

/*!
 * The grid is encoded into band separate grid and allows bilinear interpolation
 * of atmospheric correction coefficients.  It uses the implementation found in
 * ossimAtCorrRemapper.  The base class will call interpolate to calculate the
 * a, b, c coefficients used.  This class just overrides the base interpolation method. For
 * gridded remappers
 *
 *
 *
 * Sample keywordlist includes:
 *
 * band1.grid.rows: 4
 * band1.grid.cols: 4
 * band1.grid.a1:
 * band1.grid.b1:
 * band1.grid.c1:
 * band1.grid.a2:
 * band1.grid.b2:
 * band1.grid.c2:
 * band1.grid.a3:
 * band1.grid.b3:
 * band1.grid.c3:
 * band1.grid.a4:
 * band1.grid.b4:
 * band1.grid.c4:
 * band2.grid.rows: 4
 * band2.grid.cols: 4
 * band2.grid.a1:
 * band2.grid.b1:
 * band2.grid.c1:
 * band2.grid.a2:
 * band2.grid.b2:
 * band2.grid.c2:
 * band2.grid.a3:
 * band2.grid.b3:
 * band2.grid.c3:
 * band2.grid.a4:
 * band2.grid.b4:
 * band2.grid.c4:
 *
 *  :
 *  :
 *  :
 */
class ossimAtCorrGridRemapper: public ossimAtCorrRemapper
{
public:
   ossimAtCorrGridRemapper(ossimObject* owner = NULL,
                           ossimImageSource* inputSource =  NULL,
                           const ossimString& sensorType = "");
   
   virtual ~ossimAtCorrGridRemapper();


   void setGridRect(const ossimIrect& rect)
      {
         theGridBounds  = rect;
         theUlGridBound = rect.ul();
      }
   void setGridSize(ossim_uint32 numberOfBands, const ossimIpt& gridSize);
   ossimIpt getGridSize()const
      {
         return theGridSize;
      }
   
   void getValues(ossim_uint32 band,
                  ossim_uint32 row,
                  ossim_uint32 col,
                  double& a,
                  double& b,
                  double& c)
      {
         a = theAGrid[band][row][col];
         b = theAGrid[band][row][col];
         c = theAGrid[band][row][col];
      }

   void getValues(const ossimDpt& pt,
                  ossim_uint32 band,
                  double& a,
                  double& b,
                  double& c)
      {
         interpolate(pt, band, a, b, c);
      }
   
   void setValues(ossim_uint32 band,
                  ossim_uint32 row,
                  ossim_uint32 col,
                  const double& a,
                  const double& b,
                  const double& c)
      {
         theAGrid[band][row][col] = a;
         theBGrid[band][row][col] = b;
         theCGrid[band][row][col] = c;
      }
   
   virtual void initialize();
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

protected:
   ossimIrect theGridBounds;
   ossimIpt   theUlGridBound;
   ossimIpt   theGridSize;
   std::vector< std::vector< std::vector<double> > >theAGrid;
   std::vector< std::vector< std::vector<double> > >theBGrid;
   std::vector< std::vector< std::vector<double> > >theCGrid;

   virtual void interpolate(const ossimDpt& pt,
                            int band,
                            double& a,
                            double& b,
                            double& c)const;
   virtual void setBaseToAverage();
   
TYPE_DATA
};

#endif
