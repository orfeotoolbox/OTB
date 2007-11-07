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

#ifndef __otbSinusoidalMapProjection_txx
#define __otbSinusoidalMapProjection_txx

#include "otbSinusoidalMapProjection.h"

namespace otb
{
  
  template <InverseOrForwardTransformationEnum transform> 
	SinusoidalMapProjection<transform> 
  ::SinusoidalMapProjection()
  {
  }
  
  
  template <InverseOrForwardTransformationEnum transform> 
	SinusoidalMapProjection<transform>
  ::~SinusoidalMapProjection()
  {
  }
  
  ///Set the false Easting
  template <InverseOrForwardTransformationEnum transform> 
	void SinusoidalMapProjection<transform>
  ::SetFalseEasting(double falseEasting) 
  {
    this->m_MapProjection->setFalseEasting(falseEasting);
  }
  
  ///Set the False Northing
  template <InverseOrForwardTransformationEnum transform> 
	void SinusoidalMapProjection<transform>
  ::SetFalseNorthing(double falseNorthing) 
  {
    this->m_MapProjection->setFalseNorthing(falseNorthing);
  }
  
  ///Set the default parameter
  template <InverseOrForwardTransformationEnum transform> 
	void SinusoidalMapProjection<transform>
  ::SetDefaults() 
  {
	this->m_MapProjection->setDefaults();
  }
  
  ///\return the False Northing (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform> 
	double SinusoidalMapProjection<transform>
  ::GetFalseNorthing() const
  {
    double falseNorthing=this->m_MapProjection->getFalseNorthing();
    
    return falseNorthing;
  }
  
  ///\return the False Easting (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform> 
	double SinusoidalMapProjection<transform>
  ::GetFalseEasting() const
  {
    double falseEasting=this->m_MapProjection->getFalseEasting();
    
    return falseEasting;
  }
  
  template <InverseOrForwardTransformationEnum transform> 
	void SinusoidalMapProjection<transform>
	::SetParameters(double falseEasting,double falseNorthing)
  {
    this->m_MapProjection->setFalseEastingNorthing(falseEasting,falseNorthing);
  }
  
  
}
#endif
