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

#include "otbSensorModelBase.h"
#include "itkMacro.h"
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
class ITK_EXPORT ForwardSensorModel : public SensorModelBase<TScalarType,          
			                                    NInputDimensions,  
			                                    NOutputDimensions,
                                                            NParametersDimensions> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef ForwardSensorModel                                  Self;
  typedef SensorModelBase< TScalarType,
                   NInputDimensions,
                   NOutputDimensions,
                   NParametersDimensions >                    Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

  itkTypeMacro( ForwardSensorModel, SensorModelBase );
  itkNewMacro( Self );

  typedef typename Superclass::InputPointType            InputPointType;
  typedef typename Superclass::OutputPointType           OutputPointType;      
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
  /** Projection point */
  OutputPointType TransformPoint(const InputPointType &point) const;

protected:
  ForwardSensorModel(); 
  virtual ~ForwardSensorModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

};

}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbForwardSensorModel.txx"
#endif

#endif
