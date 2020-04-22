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

#ifndef otbMeanFunctor_h
#define otbMeanFunctor_h

namespace otb
{
namespace Functor
{
/** \class MeanFunctor
 *  \brief This functor generate the mean value of a component pixels
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TOutputValue>
class MeanFunctor
{
public:
  typedef MeanFunctor<TInput, TOutputValue> MeanFunctorType;

  MeanFunctor()
  {
  }

  ~MeanFunctor()
  {
  }
  inline TOutputValue operator()(const TInput& inPix)
  {
    double value(0.);
    if (inPix.Size() <= 0)
      return static_cast<TOutputValue>(0.);
    for (unsigned int i = 0; i < inPix.Size(); ++i)
    {
      value += static_cast<double>(inPix[i]);
    }
    return static_cast<TOutputValue>(value / static_cast<double>(inPix.Size()));
  }
};

} // end namespace functor
} // end namespace otb

#endif
