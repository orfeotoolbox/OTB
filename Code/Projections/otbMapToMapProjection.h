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
#ifndef __otbMapToMapProjection_h
#define __otbMapToMapProjection_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "itkTransform.h"
#include "itkMacro.h"
#include "otbMapProjection.h"

namespace otb
{


  /** \class MapToMapProjection

  * \brief Class for switching from a Map Projection coordinates to other Map Projection coordinates.
  * It converts MapProjection1 coordinates to MapProjection2 coordinates by using MapProjection methods.
  * It takes a point in input.
  * (X_1, Y_1) -> (lat, lon) -> (X_2, Y_2)
  * \ingroup Transform
  */
  template <class TInputMapProjection,
            class TOutputMapProjection,
            class TScalarType=double,
            unsigned int NInputDimensions=2,
            unsigned int NOutputDimensions=2>
    class ITK_EXPORT MapToMapProjection: public itk::Transform<TScalarType,       // Data type for scalars
                                                               NInputDimensions,  // Number of dimensions in the input space
                                                               NOutputDimensions> // Number of dimensions in the output space
    {
      public :
      /** Standard class typedefs */
      typedef itk::Transform< TScalarType,
                              NInputDimensions,
                              NOutputDimensions >  	 Superclass;
      typedef MapToMapProjection                         Self;
      typedef itk::SmartPointer<Self>              	 Pointer;
      typedef itk::SmartPointer<const Self>        	 ConstPointer;

      typedef TInputMapProjection                  	 		InputMapProjectionType;
			typedef typename InputMapProjectionType::Pointer	InputMapProjectionPointerType;
			typedef typename InputMapProjectionType::OutputPointType OutputPointOfInputMapProjectionType;

      typedef TOutputMapProjection                 	 		OutputMapProjectionType;
			typedef typename OutputMapProjectionType::Pointer	OutputMapProjectionPointerType;

			typedef typename Superclass::InputPointType  InputPointType;
      typedef typename Superclass::OutputPointType  OutputPointType;

      /** Method for creation through the object factory. */
      itkNewMacro( Self );

      /** Run-time type information (and related methods). */
      itkTypeMacro( MapToMapProjection, itk::Transform );

      itkStaticConstMacro(InputSpaceDimension,  unsigned int, NInputDimensions);
      itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
      itkStaticConstMacro(SpaceDimension,       unsigned int, NInputDimensions);
      itkStaticConstMacro(ParametersDimension,  unsigned int, NInputDimensions*(NInputDimensions+1));

      /** Set MapProjection1. */
      itkSetObjectMacro(InputMapProjection,InputMapProjectionType);

      /** Set MapProjection2. */
      itkSetObjectMacro(OutputMapProjection,OutputMapProjectionType);

			/** Transform Point from MapProjection1 to MapProjection2 */
      OutputPointType TransformPoint(const InputPointType &point) const;

//    Compute MapProjection1 coordinates to MapProjection2 coordinates. */
//    OutputPointType ComputeProjection1ToProjection2(const InputPointType &point1);

//		Compute MapProjection1 coordinates to MapProjection2 coordinates. */
//    InputPointType ComputeProjection2ToProjection1(const OutputPointType &point2);

      protected:
      MapToMapProjection();
      ~MapToMapProjection();

      InputMapProjectionPointerType  m_InputMapProjection;
      OutputMapProjectionPointerType m_OutputMapProjection;

      private:
      MapToMapProjection(const Self&); //purposely not implemented
      void operator=(const Self&);     //purposely not implemented

    };

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMapToMapProjection.txx"
#endif

#endif
