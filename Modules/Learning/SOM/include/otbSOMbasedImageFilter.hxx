/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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

#ifndef otbSOMbasedImageFilter_hxx
#define otbSOMbasedImageFilter_hxx

#include "otbSOMbasedImageFilter.h"

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TDistanceMetric, class TMap>
TOutput SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>::operator()(const TInput& input)
{
  return static_cast<TOutput>(GetMap()->GetPixel(this->GetWinner(input)));
}

template <class TInput, class TOutput, class TDistanceMetric, class TMap>
void SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>::SetMap(const MapType* theMap)
{
  m_Map      = const_cast<MapType*>(theMap);
  m_Distance = DistanceType::New();
}

template <class TInput, class TOutput, class TDistanceMetric, class TMap>
typename SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>::IndexType
SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>::GetWinner(const NeuronType& sample)
{
  typedef itk::ImageRegionIteratorWithIndex<MapType> IteratorType;
  IteratorType                                       it(GetMap(), GetMap()->GetLargestPossibleRegion());
  it.GoToBegin();

  IndexType minPos = it.GetIndex();

  double tempDistance, minDistance = GetDistance()->Evaluate(sample, it.Get());
  ++it;

  while (!it.IsAtEnd())
  {
    tempDistance = GetDistance()->Evaluate(sample, it.Get());

    if (tempDistance < minDistance)
    {
      minDistance = tempDistance;
      minPos      = it.GetIndex();
    }

    ++it;
  }

  return minPos;
}

} // end of namespace Functor

/*
 * Implementation of the class SOMbasedImageFilter
 */
template <class TInputImage, class TOutputImage, class TDistanceMetric, class TMap>
SOMbasedImageFilter<TInputImage, TOutputImage, TDistanceMetric, TMap>::SOMbasedImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
}

template <class TInputImage, class TOutputImage, class TDistanceMetric, class TMap>
void SOMbasedImageFilter<TInputImage, TOutputImage, TDistanceMetric, TMap>::SetMap(const MapType* theMap)
{
  m_Map = const_cast<MapType*>(theMap);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TDistanceMetric, class TMap>
void SOMbasedImageFilter<TInputImage, TOutputImage, TDistanceMetric, TMap>::BeforeThreadedGenerateData()
{
  m_Map->Update();
  this->GetFunctor().SetMap(m_Map);
}

} // end of namespace otb

#endif
