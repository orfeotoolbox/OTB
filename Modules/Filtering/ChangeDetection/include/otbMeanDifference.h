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

#ifndef otbMeanDifference_h
#define otbMeanDifference_h

namespace otb
{

namespace Functor
{

template <class TInput1, class TInput2, class TOutput>
class MeanDifference
{
public:
  MeanDifference()
  {
  }
  virtual ~MeanDifference()
  {
  }
  inline TOutput operator()(const TInput1& itA, const TInput2& itB)
  {

    TOutput meanA = 0.0;
    TOutput meanB = 0.0;

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
    {

      meanA += static_cast<TOutput>(itA.GetPixel(pos));
      meanB += static_cast<TOutput>(itB.GetPixel(pos));
    }
    return static_cast<TOutput>((meanA - meanB) / itA.Size());
  }
};
}
}

#endif
