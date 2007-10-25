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

#include "otbMollweidMapProjection.h"

namespace otb
{ 

MollweidMapProjection
::MollweidMapProjection()
{
	m_MollweidProjection = new ossimMollweidProjection();
}

MollweidMapProjection
::~MollweidMapProjection()
{
	delete m_MollweidProjection;
}

///Spécification du false easting
void MollweidMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_MollweidProjection->setFalseEasting(falseEasting);
}

///Spécification du false Northing
void MollweidMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_MollweidProjection->setFalseNorthing(falseNorthing);
}

///Instanciation avec les paramètres par défaut
void MollweidMapProjection
::SetDefaults() 
{
	m_MollweidProjection->setDefaults();
}

///Méthode pour récupérer le FalseNorthing(pour éviter les coordonnées négatives)
double MollweidMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_MollweidProjection->getFalseNorthing();
	
	return falseNorthing;
}

///Méthode pour récupérer le FalseEasting(pour éviter les coordonnées négatives)
double MollweidMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_MollweidProjection->getFalseEasting();
	
	return falseEasting;
}

void MollweidMapProjection
::SetParameters(double falseEasting,double falseNorthing)
{
	m_MollweidProjection->setFalseEastingNorthing(falseEasting,falseNorthing);
}

} // namespace otb

