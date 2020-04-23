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

#ifndef otbSimplePointCountStrategy_h
#define otbSimplePointCountStrategy_h

#include "otbMath.h"

namespace otb
{

/** \class Count
 * \brief Compute the density of a neighboorhood centerred in a pixel
 *
 * This filter is templated over the pixel type of the input image
 * and the pixel type of the output image.
 *
 * The filter will walk over all the pixels in the input image, and for
 * each one of them it will do the following:

 *
 * \ingroup OTBPointSet
 */

template <class TPointSet, class TRadiusType, class TIndexType>
class Count
{
public:
  Count()
  {
  }
  ~Count()
  {
  }

  inline float operator()(const TPointSet* pointSet, const TRadiusType& size, const TIndexType& index)
  {

    int    accu    = 0;
    double surface = CONST_PI * size * size;

    if (pointSet->GetNumberOfPoints() != 0)
    {
      typedef typename TPointSet::PointsContainer::ConstIterator iteratorType;
      iteratorType                                               it = pointSet->GetPoints()->Begin();

      while (it != pointSet->GetPoints()->End())
      {
        float distX2 = (index[0] - it.Value()[0]) * (index[0] - it.Value()[0]);
        float distY2 = (index[1] - it.Value()[1]) * (index[1] - it.Value()[1]);
        float dist   = std::sqrt(distX2 + distY2);

        if (dist <= size)
          accu++;

        ++it;
      }
    }
    else
      return 0.;

    return static_cast<float>(accu / surface);
  }
};

} // end namespace otb

#endif
