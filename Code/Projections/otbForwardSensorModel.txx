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
#ifndef __otbForwardSensorModel_txx
#define __otbForwardSensorModel_txx


/**otbForwardSensorModel.cxx
*  Purpose:
*  Cette classe permet de projeter un point d'une image en géométrie capteur  en un point au sol .
*  (i,j) -> (lat, lon) ou (i,j,h) -> (lat, lon)
**/

#include "otbForwardSensorModel.h"

namespace otb
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par défault
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::ForwardSensorModel():
Superclass(OutputSpaceDimension, ParametersDimension)
{
 m_Model = NULL;
}

// Destructeur 
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::~ForwardSensorModel()
{
delete m_Model;
}

/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/

/// Méthode GetGeometryKeywordlist : 
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ossimKeywordlist
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::GetImageGeometryKeywordlist(char *src)
{
return m_geom_kwl;
}

/// Méthode SetGeometry : Créer et instancier le modèle de capteur grâce aux metadata.
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::SetImageGeometry(ossimKeywordlist& geom_kwl)
{
	//m_geom_kwl = geom_kwl;
	if(m_Model)
	{
	 delete m_Model;
	}
m_Model = ossimProjectionFactoryRegistry::instance()->createProjection(geom_kwl);

}

///Méthode TransformPoint:
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::TransformPoint(const InputPointType &point) const
  {//On transforme le type "itk::point" en type "ossim::ossimDpt" 
    ossimDpt ossimPoint(point[0], point[1]);
  
  //On calcule 
   ossimGpt ossimGPoint;

  m_Model->lineSampleToWorld(ossimPoint, ossimGPoint); //Projection par la méthode "lineSampleToWorld" de la classe ossimSensorModel
  
  //On stocke le resultat dans un "OutputPointType"  
  OutputPointType outputPoint;
   outputPoint[0]=ossimGPoint.lat;
   outputPoint[1]=ossimGPoint.lon;
  return outputPoint;
  }
}//fin namespace

#endif

