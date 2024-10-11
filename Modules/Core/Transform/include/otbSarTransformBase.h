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

#ifndef otbSarTransformBase_h
#define otbSarTransformBase_h

#include "otbSensorTransformBase.h"
#include "otbSARMetadata.h"
#include "otbSarSensorModel.h"


namespace otb
{
/** \class SarTransformBase
 *  \brief Base projection class based on the Sar model.
 *
 *  This is a projection class, based on a Sar sensor model. This
 *  class allows:
 *
 *  - Forward Transformation of a point in the sensor geometry (i, j)
 *  to a geographic point in (lat, long)
 *
 *  - Inverse Transformation of a geographic point in (lat, long) to
 *  a point in the sensor geometry (i, j).
 *
 * \ingroup OTBTransform
 */
template <class TScalarType, unsigned int NInputDimensions = 3, unsigned int NOutputDimensions = 2>
class ITK_EXPORT SarTransformBase : public SensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
{
public:
  /** @name Standard class type definitions */
  //@{
  using Self         = SarTransformBase;
  using Superclass   = SensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self> ;

  using InputPointType  = itk::Point<TScalarType, NInputDimensions>;
  using OutputPointType = itk::Point<TScalarType, NOutputDimensions>;
  using TiePointsType = std::vector<std::pair<InputPointType,OutputPointType>>;
  using PixelType =TScalarType;
  //@}

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarTransformBase, SensorTransformBase);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);

  /*
   * Provide the ImageMetadata in order to set the model.
   * Return false if model not valid.
   */
  bool SetMetadata(const ImageMetadata& imd) override;

  /** Check model validity */
  bool IsValidSensorModel() const override;

  /** Refining the sensor model */
  void OptimizeParameters(ImageMetadata& imd, const TiePointsType& tiepoints, double& rmsError) final;

protected:
  SarTransformBase(TransformDirection dir) : Superclass(dir) {};
  ~SarTransformBase() = default;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  std::unique_ptr<SARParam> m_SarParam;
  std::unique_ptr<SarSensorModel> m_Transformer;

private:
  SarTransformBase(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarTransformBase.hxx"
#endif

#endif
