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

  /** \class ForwardSensorModel

   * \brief Class for direct sensor models
   *
   * Based on ossimProjectionFactoryRegistry and ossimProjection methods.
   * It takes as input an index and computes the world coordinates.
   * (i,j,h) -> (lon,lat).
   * Notice that the elevation h is optional.
   *
   * \ingroup Transform
   * \ingroup Projection
   */

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
          /** Standard class typedefs. */
          typedef ForwardSensorModel                         Self;
          typedef SensorModelBase< TScalarType,
          NInputDimensions,
          NOutputDimensions,
          NParametersDimensions >   Superclass;
          typedef itk::SmartPointer<Self>                    Pointer;
          typedef itk::SmartPointer<const Self>              ConstPointer;

          typedef typename Superclass::InputPointType        InputPointType;
          typedef typename Superclass::OutputPointType       OutputPointType;

          /** Method for creation through the object factory. */
          itkNewMacro( Self );

          /** Run-time type information (and related methods). */
          itkTypeMacro( ForwardSensorModel, SensorModelBase );

          itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
          itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
          itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!

          /** Compute the world coordinates. */
          OutputPointType TransformPoint(const InputPointType &point) const;

        protected:
          ForwardSensorModel();
          virtual ~ForwardSensorModel();

          /** PrintSelf method */
          void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private :

          ForwardSensorModel(const Self&); //purposely not implemented
          void operator=(const Self&);     //purposely not implemented

          /** Difference between consecutives heights in iterative transform point with DEM */
          double m_Epsilon;

          /** Number of iterations in iterative transform point with DEM */
          double m_NbIter;
      };


} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbForwardSensorModel.txx"
#endif

#endif
