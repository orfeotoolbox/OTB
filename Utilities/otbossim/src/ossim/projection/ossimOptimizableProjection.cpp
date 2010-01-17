#include <ossim/projection/ossimOptimizableProjection.h>

RTTI_DEF(ossimOptimizableProjection, "ossimOptimizableProjection");

ossimOptimizableProjection::ossimOptimizableProjection()
{}

ossimOptimizableProjection::ossimOptimizableProjection(
   const ossimOptimizableProjection& /* source */ )
{
   // nothing to do (copy) in base.
}

ossimOptimizableProjection::~ossimOptimizableProjection()
{}

ossimOptimizableProjection& ossimOptimizableProjection::operator=(
   const ossimOptimizableProjection& /* source */ )
{
   // nothing to do (copy) in base.
   // if (this != & source)
   // {
   // }
   return *this;
}

bool ossimOptimizableProjection::setupOptimizer(
   const ossimString& /* setup */ )
{
   return false;
}

bool ossimOptimizableProjection::needsInitialState() const
{
   return false;
}
