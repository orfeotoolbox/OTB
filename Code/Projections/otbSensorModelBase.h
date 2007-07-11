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
#ifndef __otbSensorModelBase_h
#define __otbSensorModelBase_h

#include <iostream>

#include "itkTransform.h"
#include "otbMacro.h"
#include "otbImageKeywordlist.h"
#include "projection/ossimProjection.h"
#include "itkSmartPointer.h"
#include "itkObject.h"

namespace otb
{
//On définit le template avec un type générique.
//Cette classe hérite de itk::Transform
template <class TScalarType,
          unsigned int NInputDimensions=2,
          unsigned int NOutputDimensions=2,
          unsigned int NParametersDimensions=3>
class ITK_EXPORT SensorModelBase : public itk::Transform<TScalarType,          
			                                    NInputDimensions,  
			                                    NOutputDimensions> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef SensorModelBase                               Self;
  typedef itk::Transform< TScalarType,
                   NInputDimensions,
                   NOutputDimensions >                  Superclass;
  typedef itk::SmartPointer<Self>                    	Pointer;
  typedef itk::SmartPointer<const Self>              	ConstPointer;

  itkTypeMacro( SensorModelBase, Transform );
  itkNewMacro( Self );

  typedef itk::Point<TScalarType, NInputDimensions >          InputPointType;
  typedef itk::Point<TScalarType, NOutputDimensions >         OutputPointType;      
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
  /* Get the ImageKeywordlist */
  ImageKeywordlist & GetImageGeometryKeywordlist(void) const;
  /* Get an ossimKeywordlist */
  ossimKeywordlist GetOssimKeywordlist(void);

  /* Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
  virtual void SetImageGeometry(ImageKeywordlist image_kwl);

  /* Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
  virtual void SetImageGeometry(ossimKeywordlist &geom_kwl);

protected:
  SensorModelBase(); 
  virtual ~SensorModelBase();

  /** Create the projection ( m_Model). Called by the SetImageGeometry methods */
  void CreateProjection(const ImageKeywordlist & image_kwl);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ImageKeywordlist */
  ImageKeywordlist m_ImageKeywordlist;
  /** Pointer on an ossim projection (created with the keywordlist) */
  ossimProjection * m_Model;
};

}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSensorModelBase.txx"
#endif


#endif
