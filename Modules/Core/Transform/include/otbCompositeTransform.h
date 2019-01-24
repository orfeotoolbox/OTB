/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCompositeTransform_h
#define otbCompositeTransform_h

#include <sstream>
#include <stdio.h>
#include "otbTransform.h"
#include "itkMacro.h"

namespace otb
{

/** \class CompositeTransform

* \brief Class which compose two itk::Transform to obtain one itk::Transform
*    T1    T2      T3
* (x, y) -> (x', y') -> (x'', y'')   =>    (x, y) -> (x'', y'')
*
 *
* \ingroup Transform
 *
 * \ingroup OTBTransform
*/

//typedef enum
//{
//  PROJDEFAULT=0,
//  PROJIDENTITY=1,
//  PROJMAPFORWARD=2,
//  PROJMAPINVERSE=3,
//  PROJSENSORFORWARD=4,
//  PROJSENSORINVERSE=5
//} ProjectionTypeEnum;

template <class TFirstTransform,
    class TSecondTransform,
    class TScalarType = typename TFirstTransform::ScalarType,
    unsigned int NInputDimensions = TFirstTransform::InputSpaceDimension,
    unsigned int NOutputDimensions = TSecondTransform::OutputSpaceDimension>
class ITK_EXPORT CompositeTransform : public Transform<TScalarType,  // Data type for scalars
      NInputDimensions,                                                       // Number of dimensions in the input space
      NOutputDimensions>                                                       // Number of dimensions in the output space
{
public:

  /** Standard class typedefs */
  typedef Transform<TScalarType,
      NInputDimensions,
      NOutputDimensions>         Superclass;
  typedef CompositeTransform            Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TFirstTransform                    FirstTransformType;
  typedef typename TFirstTransform::Pointer  FirstTransformPointerType;
  typedef TSecondTransform                   SecondTransformType;
  typedef typename TSecondTransform::Pointer SecondTransformPointerType;

  /** Standard vector type for this class. */
  //  typedef typename TFirstTransform::InputVectorType  FirstTransformInputVectorType;
  //  typedef typename TFirstTransform::OutputVectorType FirstTransformOutputVectorType;

  /** Standard covariant vector type for this class */
  //  typedef typename TFirstTransform::InputCovariantVectorType FirstTransformInputCovariantVectorType;
  //  typedef typename TFirstTransform::OutputCovariantVectorType FirstTransformOutputCovariantVectorType;

  /** Standard vnl_vector type for this class. */
  //  typedef typename TFirstTransform::InputVnlVectorType FirstTransformInputVnlVectorType;
  //  typedef typename TFirstTransform::OutputVnlVectorType FirstTransformOutputVnlVectorType;

  /** Standard coordinate point type for this class */
  typedef typename Superclass::InputPointType       FirstTransformInputPointType;
  typedef typename TFirstTransform::OutputPointType FirstTransformOutputPointType;
  /** TSecondTransform::InputPointType is purposely not defined
   *  It contrains user to choose First Transform Output compatible
   *  with Second Transform Input */
  typedef typename Superclass::OutputPointType SecondTransformOutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CompositeTransform, Transform);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension,      unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  /** Set first transformation */
  itkSetObjectMacro(FirstTransform, FirstTransformType);

  /** Set second transformation */
  itkSetObjectMacro(SecondTransform, SecondTransformType);

/** Get First and second transform*/
  itkGetConstReferenceMacro(FirstTransform, FirstTransformPointerType);
  itkGetConstReferenceMacro(SecondTransform, SecondTransformPointerType);

  /**  Method to transform a point. */
  SecondTransformOutputPointType TransformPoint(const FirstTransformInputPointType&) const override;

  /**  Method to transform a vector. */
  //  virtual OutputVectorType TransformVector(const InputVectorType &) const;

  /**  Method to transform a vnl_vector. */
  //  virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const;

  /**  Method to transform a CovariantVector. */
  //  virtual OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &) const;

protected:
  CompositeTransform();
  ~CompositeTransform() override;

  FirstTransformPointerType  m_FirstTransform;
  SecondTransformPointerType m_SecondTransform;

private:
  CompositeTransform(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCompositeTransform.hxx"
#endif

#endif
