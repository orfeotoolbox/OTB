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


#ifndef otbRandomPointSetSource_hxx
#define otbRandomPointSetSource_hxx

#include "otbRandomPointSetSource.h"

namespace otb
{

template <class TOutputPointSet>
RandomPointSetSource<TOutputPointSet>::RandomPointSetSource()
{
  m_NumberOfPoints = 1;
  m_Generator      = GeneratorType::New();
  m_Generator->Initialize();

  m_MinPoint.Fill(0.0);
  m_MaxPoint.Fill(1.0);
}

// init seed

template <class TOutputPointSet>
void RandomPointSetSource<TOutputPointSet>::GenerateData(void)
{
  OutputPointSetPointer outputPointSet = this->GetOutput();
  outputPointSet->Initialize();

  PointsContainerPointer outPoints = outputPointSet->GetPoints();
  outPoints->Reserve(m_NumberOfPoints);

  typename PointsContainerType::Iterator outputPoint = outPoints->Begin();
  while (outputPoint != outPoints->End())
  {
    PointType point;
    for (unsigned int i = 0; i < OutputPointSetType::PointDimension; ++i)
    {
      double v = m_Generator->GetVariateWithClosedRange(1.0);
      point[i] = (1.0 - v) * m_MinPoint[i] + v * m_MaxPoint[i];
    }
    outputPoint.Value() = point;
    ++outputPoint;
  }
}
}

#endif
