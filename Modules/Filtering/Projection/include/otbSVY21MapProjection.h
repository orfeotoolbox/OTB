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

#ifndef otbSVY21MapProjection_h
#define otbSVY21MapProjection_h

#include "otbTransMercatorMapProjection.h"

namespace otb
{
/** \class SVY21MapProjection
* \brief This class implements the SVY21 map projection (Singapore geodesic system).
* It converts coordinates in longitude, latitude (WGS84) to SVY21 map coordinates.
*
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT SVY21MapProjection : public TransMercatorMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef SVY21MapProjection                     Self;
  typedef TransMercatorMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVY21MapProjection, TransMercatorMapProjection);

protected:
  SVY21MapProjection()
  {
    this->SetParameter("OriginX", "103.83333333333333");
    this->SetParameter("OriginY", "1.3666666666666667");
    this->SetParameter("Datum", "WE");
    this->SetParameter("FalseNorthing", "38744.572");
    this->SetParameter("FalseEasting", "28001.642");
    this->SetParameter("ScaleFactor", "1.00");
  }
  ~SVY21MapProjection() override {}

private:
  SVY21MapProjection(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // namespace otb

#endif
