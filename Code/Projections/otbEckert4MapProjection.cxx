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
#include "otbEckert4MapProjection.h"

namespace otb
{ 

Eckert4MapProjection
::Eckert4MapProjection()
{
	m_Eckert4Projection = new ossimEckert4Projection();
}


Eckert4MapProjection
::~Eckert4MapProjection()
{
	delete m_Eckert4Projection;
}

///False easting specification
void Eckert4MapProjection
::SetFalseEasting(double falseEasting) 
{
	m_Eckert4Projection->setFalseEasting(falseEasting);
}

///False Northing specification
void Eckert4MapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_Eckert4Projection->setFalseNorthing(falseNorthing);
}

///Set the defaul parameters
void Eckert4MapProjection
	::SetDefaults() 
{
m_Eckert4Projection->setDefaults();
}

///Get the false northing coordinates (avoid negative values)
double Eckert4MapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_Eckert4Projection->getFalseNorthing();
	return falseNorthing;
}

///Get the false easting coordinates (avoid negative values)
double Eckert4MapProjection
::GetFalseEasting() const
{
	double falseEasting=m_Eckert4Projection->getFalseEasting();
	return falseEasting;
}

void Eckert4MapProjection
::SetParameters(double falseEasting,double falseNorthing)
{
	m_Eckert4Projection->setFalseEastingNorthing(falseEasting,falseNorthing);
}

} // namespace otb

