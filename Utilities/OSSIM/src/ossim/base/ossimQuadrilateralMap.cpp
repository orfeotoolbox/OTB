#include <ossim/base/ossimQuadrilateralMap.h>
#include <iostream>
using namespace std;

#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/matrix/newmatap.h>

ossimQuadrilateralMap::ossimQuadrilateralMap()
   : a(0.0),
     b(0.0),
     c(0.0),
     d(0.0),
     e(0.0),
     f(0.0),
     g(0.0),
     h(0.0)
{
}
ossimQuadrilateralMap::ossimQuadrilateralMap(const ossimDpt& sourceP1,
                                             const ossimDpt& sourceP2,
                                             const ossimDpt& sourceP3,
                                             const ossimDpt& sourceP4,
                                             const ossimDpt& destP1,
                                             const ossimDpt& destP2,
                                             const ossimDpt& destP3,
                                             const ossimDpt& destP4)
{
   solveCoefficients(sourceP1, sourceP2, sourceP3, sourceP4,
                     destP1, destP2, destP3, destP4);
}

ossimQuadrilateralMap::ossimQuadrilateralMap(const ossimQuadrilateralMap& rhs)
  :a(rhs.a),
   b(rhs.b),
   c(rhs.c),
   d(rhs.d),
   e(rhs.e),
   f(rhs.f),
   g(rhs.g)
{
}

void ossimQuadrilateralMap::solveCoefficients(const ossimDpt& sourceP1,
                                              const ossimDpt& sourceP2,
                                              const ossimDpt& sourceP3,
                                              const ossimDpt& sourceP4,
                                              const ossimDpt& destP1,
                                              const ossimDpt& destP2,
                                              const ossimDpt& destP3,
                                              const ossimDpt& destP4)
{
      NEWMAT::Matrix m(8,8);
      
      m << sourceP1.x << sourceP1.y << 1.0 << 0.0 << 0.0 << 0.0 << -sourceP1.x*destP1.x << -sourceP1.y*destP1.x
        << sourceP2.x << sourceP2.y << 1.0 << 0.0 << 0.0 << 0.0 << -sourceP2.x*destP2.x << -sourceP2.y*destP2.x 
        << sourceP3.x << sourceP3.y << 1.0 << 0.0 << 0.0 << 0.0 << -sourceP3.x*destP3.x << -sourceP3.y*destP3.x 
        << sourceP4.x << sourceP4.y << 1.0 << 0.0 << 0.0 << 0.0 << -sourceP4.x*destP4.x << -sourceP4.y*destP4.x 
        << 0.0 << 0.0 << 0.0 << sourceP1.x << sourceP1.y << 1.0 << -sourceP1.x*destP1.y << -sourceP1.y*destP1.y 
        << 0.0 << 0.0 << 0.0 << sourceP2.x << sourceP2.y << 1.0 << -sourceP2.x*destP2.y << -sourceP2.y*destP2.y
        << 0.0 << 0.0 << 0.0 << sourceP3.x << sourceP3.y << 1.0 << -sourceP3.x*destP3.y << -sourceP3.y*destP3.y
        << 0.0 << 0.0 << 0.0 << sourceP4.x << sourceP4.y << 1.0 << -sourceP4.x*destP4.y << -sourceP4.y*destP4.y;
   
      NEWMAT::Matrix A = m.i();
      NEWMAT::ColumnVector b(8);
      b << destP1.x << destP2.x << destP3.x << destP4.x << destP1.y << destP2.y << destP3.y << destP4.y;
      NEWMAT::ColumnVector y = A * b;

      a = y[0];
      b = y[1];
      c = y[2];
      d = y[3];
      e = y[4];
      f = y[5];
      g = y[6];
      h = y[7];
}

void ossimQuadrilateralMap::map(const ossimDpt& source, ossimDpt& dest)const
{
   double numerator;  
   double denominator;

   //solve x first
   numerator   = a*source.x + b*source.y + c;
   denominator = g*source.x + h*source.y + 1;

   dest.x = numerator/denominator;
   
   //solve y 
   numerator   = d*source.x + e*source.y + f;
   denominator = g*source.x + h*source.y + 1;

   dest.y = numerator/denominator;
}
