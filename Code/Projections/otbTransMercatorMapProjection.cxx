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

#include "otbTransMercatorMapProjection.h"

namespace otb
{ 

TransMercatorMapProjection
::TransMercatorMapProjection()
{
	m_TransMercatorProjection = new ossimTransMercatorProjection();
}

/// Desctructor
 TransMercatorMapProjection
 ::~TransMercatorMapProjection()
{
	delete m_TransMercatorProjection;
}

///Set the default ellipsoid 
void TransMercatorMapProjection
::SetEllipsoid() 
{
	ossimEllipsoid ellipse(6378137.0,6356752.3142);
	ossimGpt origin(49.83,6.16);
	 
	delete m_TransMercatorProjection;
	m_TransMercatorProjection = new ossimTransMercatorProjection(ellipse,origin,80000.0,100000.0,1.0);
}

///Set the false easting
void TransMercatorMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_TransMercatorProjection->setFalseEasting(falseEasting);
}

///Set the False Northing
void TransMercatorMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_TransMercatorProjection->setFalseNorthing(falseNorthing);
}

///Set the scale factor
void TransMercatorMapProjection
::SetScaleFactor(double scaleFactor) 
{
	m_TransMercatorProjection->setScaleFactor(scaleFactor);
}

///Set the parameters
void TransMercatorMapProjection
::SetParameters(double falseEasting,double falseNorthing,double scaleFactor) 
{
	m_TransMercatorProjection->setParameters(falseEasting, falseNorthing, scaleFactor);
}

///Set the default parameters
void TransMercatorMapProjection
::SetDefaults() 
{
	m_TransMercatorProjection->setDefaults();
}

///\return the scale factor
double TransMercatorMapProjection
::GetScaleFactor() const
{
	double scaleFactor;
	scaleFactor=m_TransMercatorProjection->getScaleFactor();
	
	return scaleFactor;
}

///\return the false northing (avoid negative coordinates)
double TransMercatorMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_TransMercatorProjection->getFalseNorthing();
	
	return falseNorthing;
}

///\return the false easting (avoid negative coordinates)
double TransMercatorMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_TransMercatorProjection->getFalseEasting();
	
	return falseEasting;
}

} // namespace otb

