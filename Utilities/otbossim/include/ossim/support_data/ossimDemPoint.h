//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class defines a DEM point.
//
//********************************************************************
// $Id: ossimDemPoint.h 17195 2010-04-23 17:32:18Z dburken $

#ifndef ossimDemPoint_HEADER
#define ossimDemPoint_HEADER

#include <vector>

class ossimDemPoint
{
public:

   ossimDemPoint(double x = 0.0, double y = 0.0);
     
   // Accessors
   double getX() const;
   double getY() const;
   void getXY(double& x, double& y) const;

   // Modifiers
   void setX(double x);
   void setY(double y);
   void setXY(double x, double y);
   bool operator<(ossimDemPoint const& /* rhs */) const {return false;}
   bool operator==(ossimDemPoint const& /* rhs */) const {return false;}

private:

   double _x;
   double _y;
};

// Inline Methods for class ossimDemPoint

inline
ossimDemPoint::ossimDemPoint(double x, double y)
{
   _x = x;
   _y = y;
}

inline
double
ossimDemPoint::getX() const
{
   return _x;
}

inline
double
ossimDemPoint::getY() const
{
   return _y;
}

inline
void
ossimDemPoint::getXY(double& x, double& y) const
{
   x = _x;
   y = _y;
}

inline
void
ossimDemPoint::setX(double x)
{
   _x = x;
}

inline
void
ossimDemPoint::setY(double y)
{
   _y = y;
}

inline
void
ossimDemPoint::setXY(double x, double y)
{
   _x = x;
   _y = y;
}

typedef std::vector<ossimDemPoint> ossimDemPointVector;

#endif  // #ifndef ossimDemPoint_HEADER
