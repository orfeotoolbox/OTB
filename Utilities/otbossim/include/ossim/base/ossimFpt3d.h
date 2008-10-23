//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:  
//
// Contains class declaration for fpt3d.
//
// Used to represent a three dimensional point containing an x and y data
// member.
//
//*******************************************************************
// $Id: ossimFpt3d.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimFpt3d_HEADER
#define ossimFpt3d_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFpt.h>


class OSSIMDLLEXPORT ossimFpt3d
{
public:

   ossimFpt3d()
      : x(0.0), y(0.0), z(0.0)
      {}
   ossimFpt3d(const ossim_float32& aX,
              const ossim_float32& aY,
              const ossim_float32& aZ)
      : x(aX), y(aY), z(aZ)
      {}

   ossimFpt pt2d() const { return ossimFpt(x, y); }
   
   ostream& print(ostream& os) const;

   friend ostream& operator<<(ostream& os, const ossimFpt3d& pt);
   friend istream& operator>>(istream& is, ossimFpt3d& pt);

   ossim_float32 x;
   ossim_float32 y;
   ossim_float32 z;
};

#endif
