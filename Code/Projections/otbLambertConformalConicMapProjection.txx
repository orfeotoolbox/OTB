/*=========================================================================
  
Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbLambertConformalConicMapProjection_txx
#define __otbLambertConformalConicMapProjection_txx

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{ 
  
	template <InverseOrForwardTransformationEnum transform>
  LambertConformalConicMapProjection<transform>
  ::LambertConformalConicMapProjection()
  {
    m_LambertConformalConicProjection = new ossimLambertConformalConicProjection();
  }
  
	template <InverseOrForwardTransformationEnum transform>
  LambertConformalConicMapProjection<transform>
  ::~LambertConformalConicMapProjection()
  {
    delete m_LambertConformalConicProjection;
  }
  
  
  /// Parallel1 specification
	template <InverseOrForwardTransformationEnum transform>
  void LambertConformalConicMapProjection<transform>
  ::SetStandardParallel1(double degree) 
  {
    m_LambertConformalConicProjection->setStandardParallel1(degree);
  }
  
  /// Parallel2 specification
  template <InverseOrForwardTransformationEnum transform>
	void LambertConformalConicMapProjection<transform>
  ::SetStandardParallel2(double degree) 
  {
    m_LambertConformalConicProjection->setStandardParallel2(degree);
  }
  
  /// Specification of the 2 parallels
  template <InverseOrForwardTransformationEnum transform>
	void LambertConformalConicMapProjection<transform>
  ::SetStandardParallels(double parallel1Degree,double parallel2Degree) 
  {
    m_LambertConformalConicProjection->setStandardParallels(parallel1Degree,parallel2Degree);
  }
  
  /// False easting specification
  template <InverseOrForwardTransformationEnum transform>
	void LambertConformalConicMapProjection<transform>
  ::SetFalseEasting(double falseEasting) 
  {
    m_LambertConformalConicProjection->setFalseEasting(falseEasting);
  }
  
  /// False Northing specification
  template <InverseOrForwardTransformationEnum transform>
	void LambertConformalConicMapProjection<transform>
  ::SetFalseNorthing(double falseNorthing) 
  {
    m_LambertConformalConicProjection->setFalseNorthing(falseNorthing);
  }
  
  /// Set the parameters
  template <InverseOrForwardTransformationEnum transform>
	void LambertConformalConicMapProjection<transform>
  ::SetParameters(double parallel1Degree,double parallel2Degree,double falseEasting,double falseNorthing) 
  {
    m_LambertConformalConicProjection->setParameters(parallel1Degree, parallel2Degree, falseEasting, falseNorthing);
  }
  
  /// Set the default parameters
  template <InverseOrForwardTransformationEnum transform>
	void LambertConformalConicMapProjection<transform>
  ::SetDefaults() 
  {
    m_LambertConformalConicProjection->setDefaults();
  }
  
  /// Get the False Northing (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform>
	double LambertConformalConicMapProjection<transform>
  ::GetFalseNorthing() const
  {
    double falseNorthing=m_LambertConformalConicProjection->getFalseNorthing();
    
    return falseNorthing;
  }
  
  ///Get the False Easting (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform>
	double LambertConformalConicMapProjection<transform>
  ::GetFalseEasting() const
  {
    double falseEasting=m_LambertConformalConicProjection->getFalseEasting();
    return falseEasting;
  }
  
} // namespace otb

#endif
