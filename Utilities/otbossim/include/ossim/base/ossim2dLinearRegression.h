//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossim2dLinearRegression.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossim2dLinearRegression_HEADER
#define ossim2dLinearRegression_HEADER
#include <ossim/base/ossimDpt.h>
#include <vector>
#include <iostream>

class ossim2dLinearRegression
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossim2dLinearRegression& data)
      {
         out << "y = " << data.theSlope << "*x + " << data.theIntercept;

         return out;
      }
   ossim2dLinearRegression();
   void getEquation(double& slope,
                    double& intercept)
   {
      slope     = theSlope;
      intercept = theIntercept;
   }
   void addPoint(const ossimDpt& pt)
      {
//         thePoints.push_back(pt);
         ++theNumberOfPoints;
         theSumX  += pt.x;
         theSumY  += pt.y;
         theSumXX += pt.x*pt.x;
         theSumYY += pt.y*pt.y;
         theSumXY += pt.x*pt.y; 
         theSolvedFlag     = false;
      }
   void clear()
      {
//         thePoints.clear();
         theNumberOfPoints = 0;
         theSumX           = 0.0;
         theSumXX          = 0.0;
         theSumY           = 0.0;
         theSumYY          = 0.0;
         theIntercept      = 0.0;
         theSlope          = 0.0;
         theSumXY          = 0.0;
         theSolvedFlag     = false;
      }
   void solve();
   ossim_uint32 getNumberOfPoints()const
      {
         return theNumberOfPoints;
//         return thePoints.size();
      }
protected:
//   std::vector<ossimDpt> thePoints;
   ossim_uint32 theNumberOfPoints;
   double theSumX;
   double theSumY;
   double theSumXX;
   double theSumYY;
   double theSumXY;
   
   double theIntercept;
   double theSlope;
   bool   theSolvedFlag;
};

#endif
