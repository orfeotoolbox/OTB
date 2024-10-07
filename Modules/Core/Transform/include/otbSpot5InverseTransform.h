/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSpot5InverseTransform_h
#define otbSpot5InverseTransform_h

#include "otbSpot5TransformBase.h"

namespace otb
{
/** \class Spot5InverseTransform
 *  \brief Projection class based on the Spot5 model.
 *
 *  This is a projection class, based on a Spot5 sensor model. This
 *  class computes an inverse transformation of a a geographic
 *  point in (lat, long) to a point in the sensor geometry (i, j).
 *
 * \ingroup OTBTransform
 */
template <class TScalarType, unsigned int NInputDimensions = 3, unsigned int NOutputDimensions = 2>
class ITK_EXPORT Spot5InverseTransform : public Spot5TransformBase<TScalarType, NInputDimensions, NOutputDimensions>
{
public:
  /** @name Standard class type definitions */
  //@{
  using Self         = Spot5InverseTransform;
  using Superclass   = Spot5TransformBase<TScalarType, NInputDimensions, NOutputDimensions>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self> ;

  typedef itk::Point<TScalarType, NInputDimensions> InputPointType;
  typedef itk::Point<TScalarType, NOutputDimensions> OutputPointType;
  using PixelType =TScalarType;
  //@}

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Spot5InverseTransform, Spot5TransformBase);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);

  /**  Method to transform a point. */
  OutputPointType TransformPoint(const InputPointType& point) const override;

  Spot5InverseTransform();
  ~Spot5InverseTransform() = default;

protected:
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  Spot5InverseTransform(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpot5InverseTransform.hxx"
#endif

#endif
