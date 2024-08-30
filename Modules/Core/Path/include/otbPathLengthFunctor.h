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


#ifndef otbPathLengthFunctor_h
#define otbPathLengthFunctor_h

#include "otbMath.h"

namespace otb
{

/** \class PathLengthFunctor
 *  \brief Select paths according to their length
 *
 * This functor gets the length of a path
 * and returns true if it is above the threshold, false otherwise.
 *
 *  \ingroup Functor
 *
 * \ingroup OTBPath
 */
template <class TInput1>
class PathLengthFunctor
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

  PathLengthFunctor()
  {
    m_Threshold = 10;
  }
  ~PathLengthFunctor()
  {
  }

  inline bool operator()(const TInput1& input)
  {
    double length = input->GetLength();

    if (length > m_Threshold)
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
