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
#ifndef __otbInverseSensorModel_h
#define __otbInverseSensorModel_h

#include "otbSensorModelBase.h"
#include "itkMacro.h"
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
class ITK_EXPORT InverseSensorModel : public SensorModelBase<TScalarType,          
			                                    NInputDimensions,  
			                                    NOutputDimensions,
                                                            NParametersDimensions>
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef InverseSensorModel                                  Self;
  typedef SensorModelBase< TScalarType,
                   NInputDimensions,
                   NOutputDimensions,
                   NParametersDimensions >                    Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

  itkTypeMacro( InverseSensorModel, SensorModelBase );
  itkNewMacro( Self );

  typedef typename Superclass::InputPointType            InputPointType;
  typedef typename Superclass::OutputPointType           OutputPointType;      
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
  // Pour projeter un point géo sur l'image en géométrie capteur.
  OutputPointType     TransformPoint(const InputPointType &point) const;
  // Pour projeter un point géo connaissant son altitude.
  OutputPointType     TransformPoint(const InputPointType &point, double height) const;

protected:
  InverseSensorModel(); 
  virtual ~InverseSensorModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

};

}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseSensorModel.txx"
#endif


#endif
