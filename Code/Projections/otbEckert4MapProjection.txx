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

#ifndef __otbEckert4MapProjection_txx
#define __otbEckert4MapProjection_txx

#include "otbEckert4MapProjection.h"

namespace otb
{ 
  
  template <InverseOrForwardTransformationEnum transform>
	Eckert4MapProjection<transform>
  ::Eckert4MapProjection()
  {
	m_Eckert4Projection = new ossimEckert4Projection();
  }
  
  
  template <InverseOrForwardTransformationEnum transform>
	Eckert4MapProjection<transform>
  ::~Eckert4MapProjection()
  {
    delete m_Eckert4Projection;
  }
  
  ///False easting specification
	template <InverseOrForwardTransformationEnum transform>
	void Eckert4MapProjection<transform>
::SetFalseEasting(double falseEasting) 
  {
    m_Eckert4Projection->setFalseEasting(falseEasting);
  }
  
  ///False Northing specification
  template <InverseOrForwardTransformationEnum transform>
	void Eckert4MapProjection<transform>
  ::SetFalseNorthing(double falseNorthing) 
  {
    m_Eckert4Projection->setFalseNorthing(falseNorthing);
  }
  
  ///Set the defaul parameters
  template <InverseOrForwardTransformationEnum transform>
	void Eckert4MapProjection<transform>
  ::SetDefaults() 
  {
    m_Eckert4Projection->setDefaults();
  }
  
  ///Get the false northing coordinates (avoid negative values)
  template <InverseOrForwardTransformationEnum transform>
	double Eckert4MapProjection<transform>
  ::GetFalseNorthing() const
  {
    double falseNorthing=m_Eckert4Projection->getFalseNorthing();
    return falseNorthing;
  }
  
  ///Get the false easting coordinates (avoid negative values)
  template <InverseOrForwardTransformationEnum transform>
	double Eckert4MapProjection<transform>
  ::GetFalseEasting() const
  {
    double falseEasting=m_Eckert4Projection->getFalseEasting();
    return falseEasting;
  }
  
  template <InverseOrForwardTransformationEnum transform>
	void Eckert4MapProjection<transform>
  ::SetParameters(double falseEasting,double falseNorthing)
  {
    m_Eckert4Projection->setFalseEastingNorthing(falseEasting,falseNorthing);
  }
  
} // namespace otb

#endif
