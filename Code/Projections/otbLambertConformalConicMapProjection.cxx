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


/// Parallel1 specification
void LambertConformalConicMapProjection
::SetStandardParallel1(double degree) 
{
	m_LambertConformalConicProjection->setStandardParallel1(degree);
}

/// Parallel2 specification
void LambertConformalConicMapProjection
::SetStandardParallel2(double degree) 
{
	m_LambertConformalConicProjection->setStandardParallel2(degree);
}

/// Specification of the 2 parallels
void LambertConformalConicMapProjection
::SetStandardParallels(double parallel1Degree,double parallel2Degree) 
{
	m_LambertConformalConicProjection->setStandardParallels(parallel1Degree,parallel2Degree);
}

/// False easting specification
void LambertConformalConicMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_LambertConformalConicProjection->setFalseEasting(falseEasting);
}

/// False Northing specification
void LambertConformalConicMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_LambertConformalConicProjection->setFalseNorthing(falseNorthing);
}

/// Set the parameters
void LambertConformalConicMapProjection
::SetParameters(double parallel1Degree,double parallel2Degree,double falseEasting,double falseNorthing) 
{
	m_LambertConformalConicProjection->setParameters(parallel1Degree, parallel2Degree, falseEasting, falseNorthing);
}

/// Set the default parameters
void LambertConformalConicMapProjection
::SetDefaults() 
{
	m_LambertConformalConicProjection->setDefaults();
}

/// Get the False Northing (avoid negative coordinates)
double LambertConformalConicMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_LambertConformalConicProjection->getFalseNorthing();
	
	return falseNorthing;
}

///Get the False Easting (avoid negative coordinates)
double LambertConformalConicMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_LambertConformalConicProjection->getFalseEasting();
	return falseEasting;
}

} // namespace otb

