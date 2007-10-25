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

#include "otbSinusoidalMapProjection.h"

namespace otb
{ 
  
  SinusoidalMapProjection
  ::SinusoidalMapProjection()
  {
    m_SinusoidalProjection = new ossimSinusoidalProjection();
  }
  
  
  SinusoidalMapProjection
  ::~SinusoidalMapProjection()
  {
    delete m_SinusoidalProjection;
  }
  
  ///Set the false Easting
  void SinusoidalMapProjection
  ::SetFalseEasting(double falseEasting) 
  {
    m_SinusoidalProjection->setFalseEasting(falseEasting);
  }
  
  ///Set the False Northing
  void SinusoidalMapProjection
  ::SetFalseNorthing(double falseNorthing) 
  {
    m_SinusoidalProjection->setFalseNorthing(falseNorthing);
  }
  
  ///Set the default parameter
  void SinusoidalMapProjection
  ::SetDefaults() 
  {
	m_SinusoidalProjection->setDefaults();
  }
  
  ///\return the False Northing (avoid negative coordinates)
  double SinusoidalMapProjection
  ::GetFalseNorthing() const
  {
    double falseNorthing=m_SinusoidalProjection->getFalseNorthing();
    
    return falseNorthing;
  }
  
  ///\return the False Easting (avoid negative coordinates)
  double SinusoidalMapProjection
  ::GetFalseEasting() const
  {
    double falseEasting=m_SinusoidalProjection->getFalseEasting();
    
    return falseEasting;
  }
  
  void SinusoidalMapProjection
::SetParameters(double falseEasting,double falseNorthing)
  {
    m_SinusoidalProjection->setFalseEastingNorthing(falseEasting,falseNorthing);
  }
  
  
} // namespace otb

