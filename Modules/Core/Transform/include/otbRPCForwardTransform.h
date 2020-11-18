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

#ifndef otbRPCForwardTransform_h
#define otbRPCForwardTransform_h

#include "otbRPCTransformBase.h"

namespace otb
{
/** \class RPCForwardTransform
 *  \brief Projection class based on the RPC method.
 *
 *  This is a projection class, based on a RPC sensor model. This
 *  class computes a forward transformation of a point in the sensor
 *  geometry (i, j) to a geographic point in (lat, long)
 *
 * \ingroup OTBTransform
 */
template <class TScalarType, unsigned int NInputDimensions = 3, unsigned int NOutputDimensions = 2>
class ITK_EXPORT RPCForwardTransform : public RPCTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
{
public:
  /** @name Standard class type definitions */
  //@{
  using Self         = RPCForwardTransform;
  using Superclass   = RPCTransformBase<TScalarType, NInputDimensions, NOutputDimensions>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self> ;

  typedef itk::Point<TScalarType, NInputDimensions> InputPointType;
  typedef itk::Point<TScalarType, NOutputDimensions> OutputPointType;
  using PixelType =TScalarType;
  //@}

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RPCForwardTransform, RPCTransformBase);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);

  /**  Method to transform a point. */
  OutputPointType TransformPoint(const InputPointType& point) const override;

protected:
  RPCForwardTransform() = default;
  ~RPCForwardTransform() = default;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  RPCForwardTransform(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRPCForwardTransform.hxx"
#endif

#endif
