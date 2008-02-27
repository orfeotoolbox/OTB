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
  }
  
  
  template <InverseOrForwardTransformationEnum transform>
      Eckert4MapProjection<transform>
  ::~Eckert4MapProjection()
  {
  }
  
  ///False easting specification
  template <InverseOrForwardTransformationEnum transform>
      void Eckert4MapProjection<transform>
  ::SetFalseEasting(double falseEasting) 
  {
    this->m_MapProjection->setFalseEasting(falseEasting);
  }
  
  ///False Northing specification
  template <InverseOrForwardTransformationEnum transform>
      void Eckert4MapProjection<transform>
  ::SetFalseNorthing(double falseNorthing) 
  {
    this->m_MapProjection->setFalseNorthing(falseNorthing);
  }
  
  ///Set the defaul parameters
  template <InverseOrForwardTransformationEnum transform>
      void Eckert4MapProjection<transform>
  ::SetDefaults() 
  {
    this->m_MapProjection->setDefaults();
  }
  
  ///Get the false northing coordinates (avoid negative values)
  template <InverseOrForwardTransformationEnum transform>
      double Eckert4MapProjection<transform>
  ::GetFalseNorthing() const
  {
    double falseNorthing=this->m_MapProjection->getFalseNorthing();
    return falseNorthing;
  }
  
  ///Get the false easting coordinates (avoid negative values)
  template <InverseOrForwardTransformationEnum transform>
      double Eckert4MapProjection<transform>
  ::GetFalseEasting() const
  {
    double falseEasting=this->m_MapProjection->getFalseEasting();
    return falseEasting;
  }
  
  template <InverseOrForwardTransformationEnum transform>
      void Eckert4MapProjection<transform>
  ::SetParameters(double falseEasting,double falseNorthing)
  {
    this->m_MapProjection->setFalseEastingNorthing(falseEasting,falseNorthing);
  }
  
} // namespace otb

#endif
