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

#ifndef otbSarSensorModel_h
#define otbSarSensorModel_h

#include "otbImageMetadata.h"


#include "itkPoint.h"

namespace otb
{

class SarSensorModel
{
public:
  SarSensorModel(const ImageMetadata & imd);
  virtual ~SarSensorModel() = default;

  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;

  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) */
  bool WorldToLineSample(const Point3DType& inGeoPoint, Point2DType& outLineSample) const;

protected:

private:
  const ImageMetadata & m_Imd;
};

}

#endif