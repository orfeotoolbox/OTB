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

#ifndef otbGenericMapProjection_h
#define otbGenericMapProjection_h

#include <sstream>

#include "otbTransform.h"
#include "itkMacro.h"
#include "otbMapProjectionAdapter.h"
#include "OTBTransformExport.h"
#include <string>

namespace otb
{

namespace TransformDirection
{
enum TransformationDirection
  {
  FORWARD = 0,
  INVERSE = 1
  };
}

/** \class GenericMapProjection
 *  \brief This is the base class for generic map projection transformation
 *
 * The forward transformation will transform the coordinate for the geographic to the
 * cartographic. The inverse model will transform from the cartographic to the geographic.
 *
 * All derived class assume that the latitude and longitude are given according to the
 * WGS84 ellipsoid model.
 *
 * \ingroup Projection
 *
 *
 * \ingroup OTBTransform
 **/

template <TransformDirection::TransformationDirection TDirectionOfMapping,
    class TScalarType = double,
    unsigned int NInputDimensions = 2,
    unsigned int NOutputDimensions = 2>
class OTBTransform_EXPORT_TEMPLATE GenericMapProjection : public Transform<TScalarType,       // Data type for scalars
      NInputDimensions,                                                         // Number of dimensions in the input space
      NOutputDimensions>                                                         // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef Transform<TScalarType,
      NInputDimensions,
      NOutputDimensions>                Superclass;
  typedef GenericMapProjection          Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef MapProjectionAdapter                      MapProjectionType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericMapProjection, Transform);

  static const TransformDirection::TransformationDirection DirectionOfMapping = TDirectionOfMapping;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  /** Return the Wkt representation of the projection*/
  virtual std::string GetWkt();

  /** Instantiate the projection according to the Wkt specification*/
  virtual void SetWkt(const std::string& projectionRefWkt);

  virtual void PrintMap() const;

  OutputPointType TransformPoint(const InputPointType& point) const override;

  virtual bool InstantiateProjection();

  const MapProjectionAdapter* GetMapProjection() const;

  virtual bool IsProjectionDefined() const;

  void SetParameter(const std::string& key, const std::string& value);
  std::string GetParameter(const std::string& key) const;

protected:
  GenericMapProjection();
  ~GenericMapProjection() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  MapProjectionAdapter::Pointer m_MapProjection;

private:
  GenericMapProjection(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericMapProjection.hxx"
#endif

namespace otb
{
extern template class OTBTransform_EXPORT_TEMPLATE GenericMapProjection<TransformDirection::TransformationDirection::FORWARD, double, 2, 2>;
extern template class OTBTransform_EXPORT_TEMPLATE GenericMapProjection<TransformDirection::TransformationDirection::FORWARD, double, 3, 3>;

extern template class OTBTransform_EXPORT_TEMPLATE GenericMapProjection<TransformDirection::TransformationDirection::INVERSE, double, 2, 2>;
extern template class OTBTransform_EXPORT_TEMPLATE GenericMapProjection<TransformDirection::TransformationDirection::INVERSE, double, 3, 3>;
}

#endif
