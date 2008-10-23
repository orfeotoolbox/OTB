#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>


ossimDpt3d::ossimDpt3d(const ossimDpt &aPt)
   :x(aPt.x),
    y(aPt.y),
    z(0)
{
   if(aPt.isNan())
   {
      makeNan();
   }
}

ossimDpt3d::ossimDpt3d(const ossimIpt &aPt)
   :x(aPt.x),
    y(aPt.y),
    z(0)
{
   if(aPt.isNan())
   {
      makeNan();
   }
}
