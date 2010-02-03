//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//$Id: ossimPolyArea2d.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimPolyArea2d_HEADER
#define ossimPolyArea2d_HEADER
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/kbool/bool_globals.h>
class Bool_Engine;

class OSSIM_DLL ossimPolyArea2d : public ossimReferenced
{
public:
   friend OSSIM_DLL std::ostream& operator <<(std::ostream& out, const ossimPolyArea2d& data);
   ossimPolyArea2d()
      :theEngine(0)
      {
         clearEngine();
      }
   ossimPolyArea2d(const vector<ossimDpt>& polygon)
      :theEngine(0)
   {
      clearEngine();
      (*this) = polygon;
   }
   ossimPolyArea2d(const vector<ossimIpt>& polygon)
         :theEngine(0)
      {
         clearEngine();
         (*this) = polygon;
      }
   ossimPolyArea2d(const vector<ossimGpt>& polygon)
      :theEngine(0)
      {
         clearEngine();
         (*this) = polygon;
      }
      
   ossimPolyArea2d(const ossimDpt& p1,
                   const ossimDpt& p2,
                   const ossimDpt& p3,
                   const ossimDpt& p4);
   ossimPolyArea2d(const ossimPolyArea2d& rhs);
      
   ossimPolyArea2d(const ossimIrect& rect)
      :theEngine(0)
      {
         (*this) = rect;
      }
   ossimPolyArea2d(const ossimDrect& rect)
      :theEngine(0)
      {
         (*this) = rect;
      }
   ossimPolyArea2d(const ossimPolygon& polygon)
      :theEngine(0)
   {
      (*this) = polygon;
   }
   ~ossimPolyArea2d();

   void clear()
   {
      clearEngine();
   }
   const ossimPolyArea2d& operator =(const ossimPolyArea2d& rhs);
   const ossimPolyArea2d& operator =(const ossimPolygon& rhs);
   const ossimPolyArea2d& operator =(const ossimIrect& rect);
   const ossimPolyArea2d& operator =(const ossimDrect& rect);
   const ossimPolyArea2d& operator =(const vector<ossimDpt>& polygon);
   const ossimPolyArea2d& operator =(const vector<ossimGpt>& polygon);
   const ossimPolyArea2d& operator =(const vector<ossimIpt>& polygon);


   
   ossimPolyArea2d operator &(const ossimPolyArea2d& rhs)const;
   ossimPolyArea2d operator -(const ossimPolyArea2d& rhs)const;
   ossimPolyArea2d operator +(const ossimPolyArea2d& rhs)const;
   ossimPolyArea2d operator ^(const ossimPolyArea2d& rhs)const;
   const ossimPolyArea2d& operator &=(const ossimPolyArea2d& rhs);
   const ossimPolyArea2d& operator -=(const ossimPolyArea2d& rhs);
   const ossimPolyArea2d& operator +=(const ossimPolyArea2d& rhs);
   const ossimPolyArea2d& operator ^=(const ossimPolyArea2d& rhs);
   

   void add(const ossimPolyArea2d& rhs);
   
   bool getVisiblePolygons(vector<ossimPolygon>& polyList)const;
   bool getPolygonHoles(vector<ossimPolygon>& polyList, bool includeFalsePolygons=false)const;
   bool isEmpty()const;
   bool isPointWithin(const ossimDpt& point, double epsilonBall=.0000001)const;
   bool isPointWithin(double x, double y, double epsilonBall=.0000001)const;
   void getBoundingRect(ossimDrect& rect);

   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
   
protected:
   void armBoolEng();
   void clearEngine();
   void performOperation(const ossimPolyArea2d& rhs,
                         BOOL_OP operation); 
   void performOperation(ossimPolyArea2d& result,
                         const ossimPolyArea2d& rhs,
                         BOOL_OP operation)const;
   void clearPolygons();
   
   mutable Bool_Engine* theEngine;
};



#endif








#if 0


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
//*************************************************************************
// $Id: ossimPolyArea2d.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimPolyArea2d_HEADER
#define ossimPolyArea2d_HEADER
extern "C"
{
#include "libpolyclip/polyarea.h"
}

#include <iostream>
#include <vector>
using namespace std;

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPolygon.h>

/*!
 * This class uses the multiclip 2-d polygon library
 * to do intersections, unions, set difference, and xor
 * operations.  It will also use the library to do quick
 * point inside and poly inside tests.
 */
class OSSIM_DLL ossimPolyArea2d
{
public:
   friend OSSIM_DLL ostream& operator  <<(ostream& out,
                                const ossimPolyArea2d& data);
   /*!
    * we will use the multi clip library in
    * libpolyclip directory.
    */
   ossimPolyArea2d();
   /*!
    * If the copy polygon flag is false
    * it will set its internal thePolygon to polygon
    * else it will make a copy of it.
    */
   ossimPolyArea2d(POLYAREA* polygon,
                  bool    copyPolygonFlag=false);
   
   ossimPolyArea2d(const ossimPolyArea2d& rhs);
   ossimPolyArea2d(const vector<ossimDpt>& polygon);
   ossimPolyArea2d(const vector<ossimGpt>& polygon);
   ossimPolyArea2d(ossimDpt* points,
                  long number);

   ossimPolyArea2d(const ossimDpt& p1,
                   const ossimDpt& p2,
                   const ossimDpt& p3,
                   const ossimDpt& p4);
   ossimPolyArea2d(const ossimIrect& rect)
      :thePolygon(NULL)
      {
         (*this) = rect;
      }
   ossimPolyArea2d(const ossimDrect& rect)
      :thePolygon(NULL)
      {
         (*this) = rect;
      }
   
   ossimPolyArea2d(const ossimPolygon& polygon);
   ~ossimPolyArea2d();

   /*!
    * Will scale the polygon along the x and y direction
    * by the passed in scale.  It will modify itself and
    * return a const reference to itself.
    */
   const ossimPolyArea2d& operator *=(const ossimDpt& scale);
   
   /*!
    * Will scale the polygon along the x and y direction
    * by the passed in scale.  It will return another object.
    */
   ossimPolyArea2d operator *(const ossimDpt& scale)const;

   /*!
    * Scales evenly along the x and y direction.
    */
   const ossimPolyArea2d& operator *=(double scale);

   /*!
    * Scales evenly along the x and y direction.
    */
   ossimPolyArea2d operator *(double scale)const;

   /*!
    * Assignment operator.  Allows you to assign
    * or copy one polygon to another.
    */
   const ossimPolyArea2d& operator =(const ossimPolyArea2d& rhs);

   const ossimPolyArea2d& operator =(const ossimIrect& rect);
   const ossimPolyArea2d& operator =(const ossimDrect& rect);
   
   /*!
    * Will do a set difference on the this - right
    * and return a new polygon.
    */
   ossimPolyArea2d operator -(const ossimPolyArea2d& rhs)const;

   /*!
    * This will do the same as - but will
    * update itself.
    */
   ossimPolyArea2d& operator -=(const ossimPolyArea2d& rhs);
   
   /*!
    * do a union of 2 polygons.
    */
   ossimPolyArea2d operator +(const ossimPolyArea2d& rhs)const;

   /*!
    * This will do the same as the operator
    * + but will update itself.
    */
   ossimPolyArea2d& operator +=(const ossimPolyArea2d& rhs);


   /*!
    * do an intersection of 2 polygons
    */
   ossimPolyArea2d operator &(const ossimPolyArea2d& rhs)const;

   /*!
    * This will do the same as the operator &
    * but will update itself.
    */

   ossimPolyArea2d& operator &=(const ossimPolyArea2d& rhs);

   /*!
    * Will perform the "excusive or" of the this
    * object with the right side
    */
   ossimPolyArea2d operator ^(const ossimPolyArea2d& rhs)const;

   /*!
    * This is the non const version and will update
    * itself.
    */
   ossimPolyArea2d& operator ^=(const ossimPolyArea2d& rhs);

   /*!
    * This only makes since if you know that there are no holes.
    * For example, using it after an intersection only operation by loading
    * up two filled regions and intersecting them.
    */
   bool getAllVisiblePolygons(vector<ossimPolygon>& polyList)const;
   
   bool isEmpty()const
      {
         return (thePolygon == NULL);
      }

   bool isPointWithin(const ossimDpt& point)const;
   bool isPointWithin(double x, double y)const;
   void getBoundingRect(ossimDrect& rect);

   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
protected:
   POLYAREA *thePolygon;

   void loadPolygonForCurrentContour(ossimPolygon& polygon,
                                     bool visible);
};

#endif
#endif
