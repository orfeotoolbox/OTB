/******************************************************************************
 * $Id: thinplatespline.h 14122 2008-03-30 10:33:39Z rouault $
 *
 * Project:  GDAL Warp API
 * Purpose:  Declarations for 2D Thin Plate Spline transformer. 
 * Author:   VIZRT Development Team.
 *
 * This code was provided by Gilad Ronnen (gro at visrt dot com) with 
 * permission to reuse under the following license.
 * 
 ******************************************************************************
 * Copyright (c) 2004, VIZRT Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/
#ifndef ossimThinPlateSpline_HEADER
#define ossimThinPlateSpline_HEADER
#include <vector>
#include <iostream>
#include <ossim/base/ossimConstants.h>
class OSSIM_DLL ossimThinPlateSpline
{
public:
   typedef enum
   {
      VIZ_GEOREF_SPLINE_ZERO_POINTS,
      VIZ_GEOREF_SPLINE_ONE_POINT,
      VIZ_GEOREF_SPLINE_TWO_POINTS,
      VIZ_GEOREF_SPLINE_ONE_DIMENSIONAL,
      VIZ_GEOREF_SPLINE_FULL,
      
      VIZ_GEOREF_SPLINE_POINT_WAS_ADDED,
      VIZ_GEOREF_SPLINE_POINT_WAS_DELETED
      
   } vizGeorefInterType;
   
   ossimThinPlateSpline(int nof_vars = 1){
      _tx = _ty = 0.0;		
      _ta = 10.0;
      _nof_points = 0;
      _nof_vars = nof_vars;
      _max_nof_points = 0;
      rhs.resize(_nof_vars);
      coef.resize(_nof_vars);
      growPoints();
      for ( int v = 0; v < _nof_vars; v++ )
         for ( int i = 0; i < 3; i++ )
            //            rhs[i][v] = 0.0;
            rhs[v][i] = 0.0;
      type = VIZ_GEOREF_SPLINE_ZERO_POINTS;
   }
   
   ~ossimThinPlateSpline(){
      _AA.clear();
      _Ainv.clear();
      x.clear();
      y.clear();
      u.clear();
      unused.clear();
      index.clear();
      rhs.clear();
      coef.clear();
   }
   
   int getNumberOfPoints()const
   {
      return _nof_points;
   }
   void setToler( double tx, double ty ){
      _tx = tx;
      _ty = ty;
   }
   
   void getToler( double& tx, double& ty)const {
      tx = _tx;
      ty = _ty;
   }
   
   vizGeorefInterType getInterpolationType ( )const{
      return type;
   }
   
   int deleteList()
	{
      _nof_points = 0;
      type = VIZ_GEOREF_SPLINE_ZERO_POINTS;
      _AA.clear();
      _Ainv.clear();
      return _nof_points;
	}
   
   void growPoints();
   int addPoint( const double Px, const double Py, const double *Pvars );
   int deletePoint(const double Px, const double Py );
   int getPoint( const double Px, const double Py, double *Pvars )const;
   bool getXy(int index, double& x, double& y)const;
   bool changePoint(int index, double x, double y, double* Pvars);
   void reset(void) { _nof_points = 0; }
   int solve(void);
   
private:	
   double baseFunc( const double x1, const double y1,
                    const double x2, const double y2 )const;
   
   vizGeorefInterType type;
   
   int _nof_vars;
   int _nof_points;
   int _max_nof_points;
   int _nof_eqs;
   
   double _tx, _ty;
   double _ta;
   double _dx, _dy;
   
   std::vector<double> x; // [VIZ_GEOREF_SPLINE_MAX_POINTS+3];
   std::vector<double> y; // [VIZ_GEOREF_SPLINE_MAX_POINTS+3];
   
   std::vector<std::vector<double> > rhs;
   std::vector<std::vector<double> > coef;
   
   std::vector<double> u; // [VIZ_GEOREF_SPLINE_MAX_POINTS];
   std::vector<int> unused; // [VIZ_GEOREF_SPLINE_MAX_POINTS];
   std::vector<int> index; // [VIZ_GEOREF_SPLINE_MAX_POINTS];
	
   std::vector<double> _AA, _Ainv;
};

#endif

