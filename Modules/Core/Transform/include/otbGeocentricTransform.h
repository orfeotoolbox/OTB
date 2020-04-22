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

#ifndef otbGeocentricTransform_h
#define otbGeocentricTransform_h

#include "otbGenericMapProjection.h"
#include "otbEllipsoidAdapter.h"

namespace otb
{
/** \class GeocentricTransform
 *
 * \brief Convert coordinates from geocentric (X, Y, Z) to geographic (lon, lat, h)
 *
 *
 * \ingroup OTBTransform
 */
template <TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType = double, unsigned int NInputDimensions = 3,
          unsigned int NOutputDimensions = 3>
class ITK_EXPORT       GeocentricTransform : public Transform<TScalarType, // Data type for scalars
                                                        NInputDimensions,  // Number of dimensions in the input space
                                                        NOutputDimensions> // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef GeocentricTransform Self;
  typedef Transform<TScalarType, NInputDimensions, NOutputDimensions> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeocentricTransform, Transform);

  static const TransformDirection::TransformationDirection DirectionOfMapping = TDirectionOfMapping;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions*(NInputDimensions + 1));

  OutputPointType TransformPoint(const InputPointType& point) const override;

protected:
  GeocentricTransform();
  ~GeocentricTransform() override;
  EllipsoidAdapter::Pointer m_Ellipsoid;

private:
  GeocentricTransform(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeocentricTransform.hxx"
#endif

#endif
