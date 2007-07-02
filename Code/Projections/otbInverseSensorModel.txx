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
#ifndef __otbInverseSensorModel_txx
#define __otbInverseSensorModel_txx


/**InverseSensorModel.cxx
*  Purpose:
*  Cette classe permet de projeter un point au sol en un point sur un point sur l'image capteur.
*  (lat,lon) -> (i,j) ou (lat,lon,h) -> (i,j)
**/
#include "otbInverseSensorModel.h"

namespace otb
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par défault
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::InverseSensorModel(): Superclass(OutputSpaceDimension, ParametersDimension)
{
 m_Model = NULL;
}

// Destructeur 
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::~InverseSensorModel()
{
delete m_Model;
}

/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/

/// Méthode GetGeometryKeywordlist
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ossimKeywordlist
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::GetImageGeometryKeywordlist(char *src)
{
return m_geom_kwl;
}

/// Méthode SetGeometry :Créer et instancier le modèle de capteur grâce aux metadata.
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::SetImageGeometry(ossimKeywordlist& geom_kwl)
{
	//m_geom_kwl = geom_kwl;
	if(m_Model)
	{
	 delete m_Model;
	}
m_Model = ossimProjectionFactoryRegistry::instance()->createProjection(geom_kwl);

}

///Méthode InverseTransformPoint:
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::InverseTransformPoint(const InputPointType &point) const
  {//On transforme le type "itk::point" en type "ossim::ossimGpt" 
    ossimGpt ossimGPoint(point[0], point[1]);
  
  //On calcule 
   ossimDpt ossimDPoint;

  m_Model->worldToLineSample(ossimGPoint, ossimDPoint);
  
  //On stocke le resultat dans un "OutputPointType"  
  OutputPointType outputPoint;
   outputPoint[0]=ossimDPoint.x;
   outputPoint[1]=ossimDPoint.y;
  return outputPoint;
  }
  
  ///Méthode InverseTransformPoint en tenant compte de l'altitude:
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::InverseTransformPoint(const InputPointType &point, double height) const
  {//On transforme le type "itk::point" en type "ossim::ossimGpt" 
    ossimGpt ossimGPoint(point[0], point[1], height);
  
  //On calcule 
   ossimDpt ossimDPoint;

  m_Model->worldToLineSample(ossimGPoint, ossimDPoint); //"worldToLineSample" appelle la méthode "lineSampleHeightToWorld" pour prendre en compte l'élévation. 
  
  //On stocke le resultat dans un "OutputPointType"  
  OutputPointType outputPoint;
  outputPoint[0]=ossimDPoint.x;
  outputPoint[1]=ossimDPoint.y;
  return outputPoint;
  }
}//fin namespace

#endif

