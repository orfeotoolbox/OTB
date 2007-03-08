//*****************************************************************************
// FILE: ossimOptimizableProjection.h
//*****************************************************************************
//TODO : forwardRelativeAccuracy should return covariance matrix, and 
//         use an optional ground position as input

#ifndef ossimOptimizableProjection_HEADER
#define ossimOptimizableProjection_HEADER

#include <ossim/projection/ossimProjection.h>

//fwd class declaration
class ossimTieGptSet;

/**
 * virtual class enabling projection optimization
 * (can be used for outlier rejection - RANSAC)
 */
class OSSIMDLLEXPORT ossimOptimizableProjection
{
public:
   /*!
    * Construction and init
    */
   ossimOptimizableProjection()
   {}
   ossimOptimizableProjection(const ossimOptimizableProjection& source)
   {}
   virtual ~ossimOptimizableProjection()
   {}

   virtual ossimOptimizableProjection& operator=(const ossimOptimizableProjection& source)
   {
      return *this;
   }

   /*!
    * setupFromString()
    * initialize parameters needed for optimizeFit and degreesOfFreedom
    */
   inline virtual bool setupOptimizer(const ossimString& setup) { return false; }

   /*!
    * useForward()
    * return true when it's better (more accurate / fast) to use forward (from ground to image)
    * than inverse(from image to ground)
    */
   virtual bool useForward()const=0;

   /*!
    * degreesOfFreedom()
    * number of degrees of freedom (eg: 8 for bilinear, 6 for affine, 2 for translation)
    * especially useful when not inheriting from AdjustableParameterInterface
    */
   virtual ossim_uint32 degreesOfFreedom()const=0;

   /*!
    * needsInitialState()
    * true if the projection needs to be initialized to be optimized, eg: sensor models
    */
   inline virtual bool needsInitialState()const {return false;}

   /*!
    * optimizeFit()
    * adjusts projection to GLOBALLY fit best to ALL tie points (no outlier rejection)
    * can select an optional threshold for accuracy
    *
    * RETURNS : resulting image error variance of fitted model, or negative value for error
    *
    * IMPORTANT : the unit for *targetVariance and the returned value is :
    *    meters^2 when useForward() is true
    *   OR
    *    pixel^2  if useForward() is false
    */
   virtual double optimizeFit(const ossimTieGptSet& tieSet, double* targetVariance=NULL)=0;

TYPE_DATA
};

#endif
