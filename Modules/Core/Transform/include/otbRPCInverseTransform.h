/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRPCInverseTransform_h
#define otbRPCInverseTransform_h

#include "otbRPCTransformBase.h"

namespace otb
{
/** \class RPCInverseTransform
 *  \brief Projection class based on the RPC method.
 *
 *  This is a projection class, based on a RPC sensor model. This
 *  class computes an inverse transformation of a a geographic
 *  point in (lat, long) to a point in the sensor geometry (i, j).
 *
 * \ingroup OTBTransform
 */
template <class TScalarType>
class ITK_EXPORT RPCInverseTransform : public RPCTransformBase<TScalarType, 3, 2>
{
public:
  /** Standard class typedefs. */
  using Self         = RPCInverseTransform;
  using Superclass   = RPCTransformBase<TScalarType, 3, 2>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self> ;

  using InputPointType  = itk::Point<TScalarType, 3>;
  using OutputPointType = itk::Point<TScalarType, 2>;
  using PixelType =TScalarType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Superclass);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, 3);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, 2);

  /**  Method to transform a point. */
  OutputPointType TransformPoint(const InputPointType& point) const override;

protected:
  RPCInverseTransform() = default;
  ~RPCInverseTransform() = default;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  RPCInverseTransform(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRPCInverseTransform.hxx"
#endif

#endif
