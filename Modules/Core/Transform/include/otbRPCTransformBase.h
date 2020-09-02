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

#ifndef otbRPCTransformBase_h
#define otbRPCTransformBase_h

#include "otbSensorModelBase.h"
#include "otbGeometryMetadata.h"
#include "otbGDALRPCTransformer.h"

namespace otb
{
/** \class RPCTransformBase
 *  \brief Base projection class based on the RPC method.
 *
 *  This is a projection class, based on a RPC sensor model. This
 *  class allows:
 *
 *  - Forward Transformation of a point in the sensor geometry (i, j)
 *  to a geographic point in (lat, long)
 *
 *  - Inverse Transformation of a a geographic point in (lat, long) to
 *  a point in the sensor geometry (i, j).
 *
 * \ingroup OTBTransform
 */
template <class TScalarType, unsigned int NInputDimensions = 3, unsigned int NOutputDimensions = 2>
class ITK_EXPORT RPCTransformBase : public SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
{
public:
  /** @name Standard class type definitions */
  //@{
  using Self         = RPCTransformBase;
  using Superclass   = SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self> ;

  using InputPointType  = itk::Point<TScalarType, NInputDimensions>;
  using OutputPointType = itk::Point<TScalarType, NOutputDimensions>;
  using PixelType =TScalarType;
  //@}

  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Superclass);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);

  /*
   * Set the model as it is defined in the ImageMetadata.
   * Used by the factory to find the correct class.
   * Return false if model not valid.
   */
  bool SetMetadataModel(boost::any imdModel) override;

  /** Check model validity */
  bool IsValidSensorModel() const override;

protected:
  RPCTransformBase() = default;
  ~RPCTransformBase() = default;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  std::unique_ptr<Projection::RPCParam> m_RPCParam;
  std::unique_ptr<GDALRPCTransformer> m_Transformer;

private:
  RPCTransformBase(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRPCTransformBase.hxx"
#endif

#endif
