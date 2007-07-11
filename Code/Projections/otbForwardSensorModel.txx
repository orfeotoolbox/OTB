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
#include "itkExceptionObject.h"
#include "otbMacro.h"

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
::ForwardSensorModel()
{
}

// Destructeur 
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::~ForwardSensorModel()
{
}

///Méthode TransformPoint:
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::TransformPoint(const InputPointType &point) const
{
        //On transforme le type "itk::point" en type "ossim::ossimDpt" 
        ossimDpt ossimPoint(point[0], point[1]);
  
        //On calcule 
        ossimGpt ossimGPoint;
        if( this->m_Model == NULL)
        {
                itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
        }

        this->m_Model->lineSampleToWorld(ossimPoint, ossimGPoint); //Projection par la méthode "lineSampleToWorld" de la classe ossimSensorModel
  
        //On stocke le resultat dans un "OutputPointType"  
        OutputPointType outputPoint;
        outputPoint[0]=ossimGPoint.lat;
        outputPoint[1]=ossimGPoint.lon;
        return outputPoint;
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
ForwardSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}//fin namespace

#endif

