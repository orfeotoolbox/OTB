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
/** Eckert4MapProjection.cxx
Définition des méthodes propres à Eckert4MapProjection.
**/

#include "otbEckert4MapProjection.h"

namespace otb
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par défault
Eckert4MapProjection::Eckert4MapProjection()
{
 m_Eckert4Projection = new ossimEckert4Projection();
}

// Destructeur 
 Eckert4MapProjection::~Eckert4MapProjection()
{
delete m_Eckert4Projection;
}
/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/
///Spécification du false easting
void    Eckert4MapProjection::SetFalseEasting(double falseEasting) 
{
m_Eckert4Projection->setFalseEasting(falseEasting);
}

///Spécification du false Northing
void    Eckert4MapProjection::SetFalseNorthing(double falseNorthing) 
{
m_Eckert4Projection->setFalseNorthing(falseNorthing);
}

///Instanciation avec les paramètres par défaut
void    Eckert4MapProjection::SetDefaults() 
{
m_Eckert4Projection->setDefaults();
}

///Méthode pour récupérer le FalseNorthing(pour éviter les coordonnées négatives)
 double Eckert4MapProjection::GetFalseNorthing() const
{
double falseNorthing=m_Eckert4Projection->getFalseNorthing();
return falseNorthing;
}

///Méthode pour récupérer le FalseEasting(pour éviter les coordonnées négatives)
double Eckert4MapProjection::GetFalseEasting() const
{
double falseEasting=m_Eckert4Projection->getFalseEasting();
return falseEasting;
}

}//fin namespace

