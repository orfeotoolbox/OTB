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
#ifndef __otbTransMercatorMapProjection_txx
#define __otbTransMercatorMapProjection_txx

#include "otbTransMercatorMapProjection.h"

namespace otb
{
  
	template <InverseOrForwardTransformationEnum transform>
  TransMercatorMapProjection<transform>
  ::TransMercatorMapProjection()
  {
    m_TransMercatorProjection = new ossimTransMercatorProjection();
  }
  
  /// Desctructor
  template <InverseOrForwardTransformationEnum transform>
	TransMercatorMapProjection<transform>
  ::~TransMercatorMapProjection()
  {
    delete m_TransMercatorProjection;
  }
  
  ///Set the default ellipsoid 
  template <InverseOrForwardTransformationEnum transform>
	void TransMercatorMapProjection<transform>
  ::SetEllipsoid() 
  {
    ossimEllipsoid ellipse(6378137.0,6356752.3142);
    ossimGpt origin(49.83,6.16);
    
    delete m_TransMercatorProjection;
    m_TransMercatorProjection = new ossimTransMercatorProjection(ellipse,origin,80000.0,100000.0,1.0);
  }
  
  ///Set the false Easting
  template <InverseOrForwardTransformationEnum transform>
	void TransMercatorMapProjection<transform>
  ::SetFalseEasting(double falseEasting) 
  {
    m_TransMercatorProjection->setFalseEasting(falseEasting);
  }
  
  ///Set the False Northing
  template <InverseOrForwardTransformationEnum transform>
	void TransMercatorMapProjection<transform>
  ::SetFalseNorthing(double falseNorthing) 
  {
    m_TransMercatorProjection->setFalseNorthing(falseNorthing);
  }
  
  ///Set the scale factor
  template <InverseOrForwardTransformationEnum transform>
	void TransMercatorMapProjection<transform>
  ::SetScaleFactor(double scaleFactor) 
  {
    m_TransMercatorProjection->setScaleFactor(scaleFactor);
  }
  
  ///Set the parameters
  template <InverseOrForwardTransformationEnum transform>
	void TransMercatorMapProjection<transform>
  ::SetParameters(double falseEasting,double falseNorthing,double scaleFactor) 
  {
    m_TransMercatorProjection->setParameters(falseEasting, falseNorthing, scaleFactor);
  }
  
  ///Set the default parameters
  template <InverseOrForwardTransformationEnum transform>
	void TransMercatorMapProjection<transform>
  ::SetDefaults() 
  {
    m_TransMercatorProjection->setDefaults();
  }
  
  ///\return the scale factor
  template <InverseOrForwardTransformationEnum transform>
	double TransMercatorMapProjection<transform>
  ::GetScaleFactor() const
  {
    double scaleFactor;
    scaleFactor=m_TransMercatorProjection->getScaleFactor();
    
    return scaleFactor;
  }
  
  ///\return the false northing (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform>
	double TransMercatorMapProjection<transform>
  ::GetFalseNorthing() const
  {
    double falseNorthing=m_TransMercatorProjection->getFalseNorthing();
    
    return falseNorthing;
  }
  
  ///\return the false easting (avoid negative coordinates)
  template <InverseOrForwardTransformationEnum transform>
	double TransMercatorMapProjection<transform>
  ::GetFalseEasting() const
  {
    double falseEasting=m_TransMercatorProjection->getFalseEasting();
    
    return falseEasting;
  }
  
}
#endif
