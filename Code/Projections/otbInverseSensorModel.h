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

/** \class InverseSensorModel

 * \brief Class for inverse sensor models 
 *
 * Based to ossimProjectionFactoryRegistry and ossimProjection methods.
 * It takes in input an world point and compute the index position of the correspondant point in the input image.
 * (lat,lon,h) -> (i,j).
 * Notice that the elevation h is optional. 
 *
 * \ingroup Transform 
 */
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

	/** Standard class typedefs. */
  typedef InverseSensorModel                                Self;
  typedef SensorModelBase< TScalarType,
                   NInputDimensions,
                   NOutputDimensions,
                   NParametersDimensions >                  Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

  typedef typename Superclass::InputPointType            InputPointType;
  typedef typename Superclass::OutputPointType           OutputPointType;      

	/** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( InverseSensorModel, SensorModelBase );
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
  // Pour projeter un point géo sur l'image en géométrie capteur.
	OutputPointType TransformPoint(const InputPointType &point) const;
  // Pour projeter un point géo connaissant son altitude.
  OutputPointType TransformPoint(const InputPointType &point, double height) const;

protected:
  InverseSensorModel(); 
  virtual ~InverseSensorModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
	
private :

  InverseSensorModel(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseSensorModel.txx"
#endif


#endif
