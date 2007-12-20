//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
//
// DESCRIPTION:
//   Contains implementation of class 
//
// SOFTWARE HISTORY:
//>
//   ddmmm2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimLsrVector.cpp 11428 2007-07-27 18:44:18Z gpotts $

#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimCommon.h>

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrVector(ossimEcefVector, ossimLsrSpace)
//  
//  Establishes an LSR vector given the ECEF equivalent and the LSR space.
//  
//*****************************************************************************
ossimLsrVector::ossimLsrVector(const ossimEcefVector& convert_this,
                               const ossimLsrSpace&   new_space)
   : theLsrSpace (new_space)
{
   initialize(convert_this);
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrVector(ossimLsrVector, ossimLsrSpace)
//  
//  Establishes an LSR vector in a new space given another LSR vector in a
//  different space.
//  
//*****************************************************************************
ossimLsrVector::ossimLsrVector(const ossimLsrVector& convert_this,
                               const ossimLsrSpace&  new_space)
   : theLsrSpace (new_space)
{
   initialize(ossimEcefVector(convert_this));
}

//*****************************************************************************
//  METHOD: ossimLsrVector::dot(ossimLsrVector)
//  
//  Computes the scalar product.
//  
//*****************************************************************************
double ossimLsrVector::dot(const ossimLsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.theLsrSpace))
   {
      ossimLsrSpace::lsrSpaceErrorMessage();
      return ossim::nan();
   }
   return theData.dot(v.data());
}

//*****************************************************************************
//  METHOD: ossimLsrVector::angleTo(ossimLsrVector)
//  
//  Returns the angle subtended (in DEGREES) between this and arg vector
//
//*****************************************************************************
double ossimLsrVector::angleTo(const ossimLsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.theLsrSpace))
   {
      ossimLsrSpace::lsrSpaceErrorMessage();
      return ossim::nan();
   }
   double mag_product = theData.magnitude() * v.theData.magnitude();
   return ossim::acosd(theData.dot(v.theData)/mag_product);
}

//*****************************************************************************
//  METHOD: ossimLsrVector::cross(ossimLsrVector)
//  
//  Computes the cross product.
//  
//*****************************************************************************
ossimLsrVector ossimLsrVector::cross(const ossimLsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.theLsrSpace))
   {
      ossimLsrSpace::lsrSpaceErrorMessage();
      return ossimLsrVector(ossim::nan(), ossim::nan(), ossim::nan(),
                            theLsrSpace);
   }
   return ossimLsrVector(theData.cross(v.data()), theLsrSpace);
}

