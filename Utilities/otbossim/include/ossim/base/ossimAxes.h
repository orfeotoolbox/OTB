//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// Contains class declaration for ossimAxes.  This will allow you to
// define three orthogonal ossimAxes and an origin in 3-D space.  This
// information will be used to allow you to place points relative
// to the defined axes.
//*******************************************************************
//  $Id: ossimAxes.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimAxes_HEADER
#define ossimAxes_HEADER
#include <iostream>
#include <iomanip>
using namespace std;
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimMatrix4x4.h>
class OSSIMDLLEXPORT ossimAxes
{
public:

   friend inline ostream& operator <<(ostream &out, const ossimAxes &axes);
   
   /*!
    * Default constructor is at position 0, 0, 0 with unit axes
    * 
    */
   ossimAxes(const ossimColumnVector3d &origin=ossimColumnVector3d(0,0,0),
             const ossimColumnVector3d &xAxis=ossimColumnVector3d(1,0,0),
             const ossimColumnVector3d &yAxis=ossimColumnVector3d(0,1,0),
             const ossimColumnVector3d &zAxis=ossimColumnVector3d(0,0,1))
      :
         theOrigin(origin),
         theXAxis(xAxis),
         theYAxis(yAxis),
         theZAxis(zAxis)
      {}

   ossimColumnVector3d projectPoint(const ossimColumnVector3d &pt)
      {
         return ossimColumnVector3d(theXAxis.dot(pt),
                                    theYAxis.dot(pt),
                                    theZAxis.dot(pt));
      }

   ossimColumnVector3d pointRelative(const ossimColumnVector3d& pt)
      {
         return projectPoint(pt - theOrigin);
      }
   
   void rotateAxes(const ossimMatrix3x3& m)
      {
         theXAxis = m*theXAxis;
         theYAxis = m*theYAxis;
         theZAxis = m*theZAxis;
      }
   void rotateAxes(const ossimMatrix4x4& m)
      {
         theXAxis = m.rotateOnly(theXAxis);
         theYAxis = m.rotateOnly(theYAxis);
         theZAxis = m.rotateOnly(theZAxis);
      }
   void transformOrigin(const ossimMatrix3x3& m)
      {
         theOrigin = m*theOrigin;
      }
   void transformOrigin(const ossimMatrix4x4& m)
      {
         theOrigin = m*theOrigin;
      }
   const ossimColumnVector3d& origin()const{return theOrigin;}
   const ossimColumnVector3d& xAxis()const{return theXAxis;}
   const ossimColumnVector3d& yAxis()const{return theYAxis;}
   const ossimColumnVector3d& zAxis()const{return theZAxis;}
   
private:
   ossimColumnVector3d theOrigin; // Is the origin of this axes
   ossimColumnVector3d theXAxis;  // Is the direction of the x axis
   ossimColumnVector3d theYAxis;  // Is the direction of the y axis
   ossimColumnVector3d theZAxis;  // Is the direction of the z axis
};

inline ostream& operator <<(ostream &out, const ossimAxes &axes)
{
   return out << "position: " << axes.theOrigin << endl
              << "x axis  : " << axes.theXAxis  << endl
              << "Y axis  : " << axes.theYAxis  << endl
              << "z axis  : " << axes.theZAxis;
}

#endif
