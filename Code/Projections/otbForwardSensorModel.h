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
#ifndef __otbForwardSensorModel_h
#define __otbForwardSensorModel_h

#include <sstream>
#include <stdio.h>
#include <iostream>
#include "itkExceptionObject.h"
#include "itkTransform.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "projection/ossimSensorModelFactory.h"
#include "projection/ossimProjection.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "base/ossimKeywordlist.h"
#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "itkSmartPointer.h"
#include "itkObject.h"


namespace otb
{
/**otbForwardSensorModel.h
*  Cette classe encapsule quelques méthodes de ossimProjectionFactoryRegistry et ossimProjection.
**/
//Cette classe hérite de itk::Transform
template <class TScalarType,
          unsigned int NInputDimensions=2,
          unsigned int NOutputDimensions=2,
          unsigned int NParametersDimensions=3>
class ITK_EXPORT ForwardSensorModel : public itk::Transform<TScalarType,          
			                                    NInputDimensions,  
			                                    NOutputDimensions> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef ForwardSensorModel                                  Self;
  typedef itk::Transform< TScalarType,
                   NInputDimensions,
                   NOutputDimensions >                        Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

itkTypeMacro( ForwardSensorModel, Transform );
itkNewMacro( Self );

  typedef itk::Point<TScalarType, NInputDimensions >          InputPointType;
  typedef itk::Point<TScalarType, NOutputDimensions >         OutputPointType;      
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/	    
//Pour lire les kwl.                                  
ossimKeywordlist    GetImageGeometryKeywordlist(char *src);	  
//Pour construire le modèle.
void                SetImageGeometry(ossimKeywordlist &geom_kwl);
// Pour projeter le point au sol.
OutputPointType     TransformPoint(const InputPointType &point) const;

protected:
ForwardSensorModel(); 
virtual ~ForwardSensorModel();

//Variables protégés:
ossimKeywordlist m_geom_kwl;
ossimProjection* m_Model;
};

}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbForwardSensorModel.txx"
#endif

#endif
