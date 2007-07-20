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

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{ 

LambertConformalConicMapProjection
::LambertConformalConicMapProjection()
{
	m_LambertConformalConicProjection = new ossimLambertConformalConicProjection();
}

LambertConformalConicMapProjection
::~LambertConformalConicMapProjection()
{
	delete m_LambertConformalConicProjection;
}


///Spécification du Parallel1
void LambertConformalConicMapProjection
::SetStandardParallel1(double degree) 
{
	m_LambertConformalConicProjection->setStandardParallel1(degree);
}

///Spécification du Parallel2
void LambertConformalConicMapProjection
::SetStandardParallel2(double degree) 
{
	m_LambertConformalConicProjection->setStandardParallel2(degree);
}

///Spécification des 2 parallels
void LambertConformalConicMapProjection
::SetStandardParallels(double parallel1Degree,double parallel2Degree) 
{
	m_LambertConformalConicProjection->setStandardParallels(parallel1Degree,parallel2Degree);
}

///Spécification du false easting
void LambertConformalConicMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_LambertConformalConicProjection->setFalseEasting(falseEasting);
}

///Spécification du false Northing
void LambertConformalConicMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_LambertConformalConicProjection->setFalseNorthing(falseNorthing);
}

///Instanciation de tous les paramètres
void LambertConformalConicMapProjection
::SetParameters(double parallel1Degree,double parallel2Degree,double falseEasting,double falseNorthing) 
{
	m_LambertConformalConicProjection->setParameters(parallel1Degree, parallel2Degree, falseEasting, falseNorthing);
}

///Instanciation avec les paramètres par défaut
void LambertConformalConicMapProjection
::SetDefaults() 
{
	m_LambertConformalConicProjection->setDefaults();
}

///Méthode pour récupérer le FalseNorthing(pour éviter les coordonnées négatives)
double LambertConformalConicMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_LambertConformalConicProjection->getFalseNorthing();
	
	return falseNorthing;
}

///Méthode pour récupérer le FalseEasting(pour éviter les coordonnées négatives)
double LambertConformalConicMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_LambertConformalConicProjection->getFalseEasting();
	return falseEasting;
}

} // namespace otb

