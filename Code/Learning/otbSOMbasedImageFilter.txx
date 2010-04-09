/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSOMbasedImageFilter_txx
#define __otbSOMbasedImageFilter_txx

#include "otbSOMbasedImageFilter.txx"

namespace otb {

namespace Functor {

template <class TInput, class TOutput, class TDistanceMetric, class TMap>
TOutput
SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>
::operator() (const TInput &input)
  {
  return static_cast<TOutput>(GetMap()->GetPixel(this->GetWinner(input)));
  }

template <class TInput, class TOutput, class TDistanceMetric, class TMap>
void
SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>
::SetMap(const MapType * theMap)
{
  m_Map = const_cast<MapType *>(theMap);
  m_Distance = DistanceType::New();
}

template <class TInput, class TOutput, class TDistanceMetric, class TMap>
typename SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>::IndexType
SOMbasedImageFilterFunctor<TInput, TOutput, TDistanceMetric, TMap>
::GetWinner(const NeuronType& sample)
{
  typedef itk::ImageRegionIteratorWithIndex<MapType> IteratorType;
  IteratorType it(GetMap(), GetMap()->GetLargestPossibleRegion());
  it.GoToBegin();

  IndexType minPos = it.GetIndex();

  double tempDistance,
         minDistance = GetDistance()->Evaluate(sample, it.Get());
  ++it;

  while (!it.IsAtEnd())
    {
    tempDistance = GetDistance()->Evaluate(sample, it.Get());

    if (tempDistance < minDistance)
      {
      minDistance = tempDistance;
      minPos = it.GetIndex();
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
SOMbasedImageFilter<TInputImage, TOutputImage, TDistanceMetric, TMap>
::SOMbasedImageFilter ()
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
}

template <class TInputImage, class TOutputImage, class TDistanceMetric, class TMap>
void
SOMbasedImageFilter<TInputImage, TOutputImage, TDistanceMetric, TMap>
::SetMap(const MapType * theMap)
{
  m_Map = const_cast<MapType *>(theMap);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TDistanceMetric, class TMap>
void
SOMbasedImageFilter<TInputImage, TOutputImage, TDistanceMetric, TMap>
::BeforeThreadedGenerateData()
{
  m_Map->Update();
  this->GetFunctor().SetMap(m_Map);
}

} // end of namespace otb

#endif
