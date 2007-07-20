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

// Destructeur 
 TransMercatorMapProjection
 ::~TransMercatorMapProjection()
{
	delete m_TransMercatorProjection;
}

///Ellipsoid propre par défaut
void TransMercatorMapProjection
::SetEllipsoid() 
{
	ossimEllipsoid ellipse(6378137.0,6356752.3142);
	ossimGpt origin(49.83,6.16);
	 
	delete m_TransMercatorProjection;
	m_TransMercatorProjection = new ossimTransMercatorProjection(ellipse,origin,80000.0,100000.0,1.0);
}

///Spécification du false easting
void TransMercatorMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_TransMercatorProjection->setFalseEasting(falseEasting);
}

///Spécification du false Northing
void TransMercatorMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_TransMercatorProjection->setFalseNorthing(falseNorthing);
}

///Spécification du scaleFactor
void TransMercatorMapProjection
::SetScaleFactor(double scaleFactor) 
{
	m_TransMercatorProjection->setScaleFactor(scaleFactor);
}

///Instanciation de tous les paramètres
void TransMercatorMapProjection
::SetParameters(double falseEasting,double falseNorthing,double scaleFactor) 
{
	m_TransMercatorProjection->setParameters(falseEasting, falseNorthing, scaleFactor);
}

///Instanciation avec les paramètres par défaut
void TransMercatorMapProjection
::SetDefaults() 
{
	m_TransMercatorProjection->setDefaults();
}

///Instanciation avec les paramètres par défaut
double TransMercatorMapProjection
::GetScaleFactor() const
{
	double scaleFactor;
	scaleFactor=m_TransMercatorProjection->getScaleFactor();
	
	return scaleFactor;
}

///Méthode pour récupérer le FalseNorthing(pour éviter les coordonnées négatives)
double TransMercatorMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_TransMercatorProjection->getFalseNorthing();
	
	return falseNorthing;
}

///Méthode pour récupérer le FalseEasting(pour éviter les coordonnées négatives)
double TransMercatorMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_TransMercatorProjection->getFalseEasting();
	
	return falseEasting;
}

} // namespace otb

