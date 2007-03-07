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
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class defines a DEM point.
//
//********************************************************************
// $Id: ossimDemPoint.h,v 1.4 2002/01/12 16:11:24 dburken Exp $

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
   bool operator<(ossimDemPoint const& rhs) const {return false;}
   bool operator==(ossimDemPoint const& rhs) const {return false;}

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
