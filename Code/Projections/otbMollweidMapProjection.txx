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

#ifndef __otbMollweidMapProjection_txx
#define __otbMollweidMapProjection_txx

#include "otbMollweidMapProjection.h"

  
  template <InverseOrForwardTransformationEnum transform>
	MollweidMapProjection<transform>
  ::MollweidMapProjection()
  {
    m_MollweidProjection = new ossimMollweidProjection();
  }
  
  template <InverseOrForwardTransformationEnum transform>
	MollweidMapProjection<transform>
  ::~MollweidMapProjection()
  {
    delete m_MollweidProjection;
  }
  
  ///Set the false easting
  template <InverseOrForwardTransformationEnum transform>
	void MollweidMapProjection<transform>
  ::SetFalseEasting(double falseEasting) 
  {
    m_MollweidProjection->setFalseEasting(falseEasting);
  }
  
  ///Set the false Northing
  template <InverseOrForwardTransformationEnum transform>
	void MollweidMapProjection<transform>
  ::SetFalseNorthing(double falseNorthing) 
  {
    m_MollweidProjection->setFalseNorthing(falseNorthing);
  }
  
  ///Set the default parameters
  template <InverseOrForwardTransformationEnum transform>
	void MollweidMapProjection<transform>
  ::SetDefaults() 
  {
    m_MollweidProjection->setDefaults();
  }
  
  ///\return the false northing (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform>
	double MollweidMapProjection<transform>
  ::GetFalseNorthing() const
  {
    double falseNorthing=m_MollweidProjection->getFalseNorthing();
    
    return falseNorthing;
  }
  
  ///\return the false easting (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform>
	double MollweidMapProjection<transform>
  ::GetFalseEasting() const
  {
    double falseEasting=m_MollweidProjection->getFalseEasting();
    
    return falseEasting;
}
  
  template <InverseOrForwardTransformationEnum transform>
	void MollweidMapProjection<transform>
  ::SetParameters(double falseEasting,double falseNorthing)
  {
    m_MollweidProjection->setFalseEastingNorthing(falseEasting,falseNorthing);
  }
  

#endif
