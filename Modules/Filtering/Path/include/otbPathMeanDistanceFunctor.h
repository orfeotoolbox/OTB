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

#ifndef otbPathMeanDistanceFunctor_h
#define otbPathMeanDistanceFunctor_h

#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{

/** \class PathMeanDistanceFunctor
 *  \brief Functor to select path according to the average distance between points
 *
 * This functor compute the average distance between each point of a path
 * and return true is the distance is above the threshold, false otherwise.
 *
 * \sa UnaryFunctorObjectListBooleanFilter
 *
 *  \ingroup Functor
 *
 * \ingroup OTBPath
 */
template <class TInput1>
class PathMeanDistanceFunctor
{
public:
  void SetThreshold(double threshold)
  {
    m_Threshold = threshold;
  }
  double GetThreshold(void) const
  {
    return (m_Threshold);
  }

  PathMeanDistanceFunctor()
  {
    m_Threshold = 0.2;
  }
  ~PathMeanDistanceFunctor()
  {
  }

  inline bool operator()(const TInput1& input)
  {

    double                                                              meanDistance = 0.0;
    typedef typename TInput1::ObjectType::VertexListType::ConstIterator VertexListConstIteratorType;
    typedef typename TInput1::ObjectType::VertexType                    VertexType;
    VertexListConstIteratorType                                         beginIt = input->GetVertexList()->Begin();

    VertexType v1 = beginIt.Value();
    VertexType v2 = beginIt.Value();
    ++beginIt;
    while (beginIt != input->GetVertexList()->End())
    {
      v1 = v2;
      v2 = beginIt.Value();
      meanDistance += std::sqrt(std::pow(v1[0] - v2[0], 2) + std::pow(v1[1] - v2[1], 2));
      ++beginIt;
    }

    double nbVertices = static_cast<double>(input->GetVertexList()->Size());
    if (nbVertices > 1)
    {
      meanDistance = meanDistance / (nbVertices - 1);
    }
    else
    {
      itkGenericExceptionMacro(<< "Object with only one vertex!");
    }

    /*     std::cout << "Num vertex: " << nbVertices << std::endl;
         std::cout << "Mean dist: " << meanDistance << std::endl;
    */
    if (meanDistance > m_Threshold)
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
