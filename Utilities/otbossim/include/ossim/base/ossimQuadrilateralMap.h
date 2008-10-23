#ifndef ossimQuadrilateralMap_HEADER
#define ossimQuadrilateralMap_HEADER
#include <ossim/base/ossimDpt.h>

class OSSIMDLLEXPORT ossimQuadrilateralMap
{
public:
   ossimQuadrilateralMap(const ossimDpt& sourceP1,
                         const ossimDpt& sourceP2,
                         const ossimDpt& sourceP3,
                         const ossimDpt& sourceP4,
                         const ossimDpt& destP1,
                         const ossimDpt& destP2,
                         const ossimDpt& destP3,
                         const ossimDpt& destP4);
   ossimQuadrilateralMap();

  ossimQuadrilateralMap(const ossimQuadrilateralMap& rhs);
   /*!
    * Uses a standard form of quadrilateral to quadrilateral
    * mapping:
    *
    * X = (a*U + b*V + c) / (g*U + h*V + 1)
    * Y = (d*U + e*V+f)/(g*U + h*V + 1)
    *
    * where:
    * X and Y are the mapped destination point.
    * U and V are the source point to warp
    * a, b, c, d, e, f, g, h are coefficients sovled by
    * the points passed to the constructor.
    */
   void map(const ossimDpt& source, ossimDpt& dest)const;

   void solveCoefficients(const ossimDpt& sourceP1,
                          const ossimDpt& sourceP2,
                          const ossimDpt& sourceP3,
                          const ossimDpt& sourceP4,
                          const ossimDpt& destP1,
                          const ossimDpt& destP2,
                          const ossimDpt& destP3,
                          const ossimDpt& destP4);

protected:
   double a;
   double b;
   double c;
   double d;
   double e;
   double f;
   double g;
   double h;
   
};

#endif
