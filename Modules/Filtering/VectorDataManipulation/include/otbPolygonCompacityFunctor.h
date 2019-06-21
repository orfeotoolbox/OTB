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

#ifndef otbPolygonCompacityFunctor_h
#define otbPolygonCompacityFunctor_h

#include "otbMath.h"

#if ITK_VERSION_MAJOR >= 5
#include "vcl_legacy_aliases.h" // ITK v5 compatibility
#endif

namespace otb
{

/** \class PolygonCompacityFunctor
 *  \brief Select polygons according to their compacity
 *
* This functor compute the compacity of a polygon
* and return true is the distance is above the threshold, false otherwise.
*
* The compacity is defined as:
*
*  \f$ 4\pi \frac{A}{L^2}\f$
*
* where \f$ A \f$ is the area (obtained by the method GetArea() )
* and \f$ L \f$ the perimeter (obtained by the method GetLength() ).
 *
 *  \ingroup Functor
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TInput1>
class PolygonCompacityFunctor
{
public:
  void SetThreshold(double threshold)
  {
    m_Threshold = threshold;
  }
  double GetThreshold(void) const
  {
    return m_Threshold;
  }

  PolygonCompacityFunctor()
  {
    m_Threshold = 0.2;
  }
  ~PolygonCompacityFunctor() {}

  inline bool operator ()(const TInput1& input)
  {
    double circularityRatio = 4*CONST_PI*input->GetArea()
                              / vnl_math_sqr(input->GetLength());

    if (circularityRatio > m_Threshold)
      {
      return true;
      }
    else
      {
      return false;
      }
  }

private:
  double m_Threshold;
};

}

#endif
