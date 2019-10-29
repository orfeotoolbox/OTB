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


#ifndef otbLabelImageSmallRegionMergingFilter_hxx
#define otbLabelImageSmallRegionMergingFilter_hxx

#include "otbLabelImageSmallRegionMergingFilter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputLabelImage>
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::PersistentLabelImageSmallRegionMergingFilter() : m_Size(1)
{
}

template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::SetLabelPopulation(LabelPopulationType const& labelPopulation)
{
  m_LabelPopulation = labelPopulation;

  // Initialize m_CorrespondingMap to the identity (i.e. m[label] = label)
  for (auto label : m_LabelPopulation)
  {
    m_LUT[label.first] = label.first;
  }
}

template <class TInputLabelImage>
typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::LabelPopulationType const&
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::GetLabelPopulation() const
{
  return m_LabelPopulation;
}


template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::SetLabelStatistic(LabelStatisticType const& labelStatistic)
{
  m_LabelStatistic = labelStatistic;
}

template <class TInputLabelImage>
typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::LabelStatisticType const&
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::GetLabelStatistic() const
{
  return m_LabelStatistic;
}

template <class TInputLabelImage>
typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::LUTType const&
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::GetLUT() const
{
  return m_LUT;
}

template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::Reset()
{
  m_NeighboursMapsTmp.clear();
  m_NeighboursMapsTmp.resize(this->GetNumberOfThreads());
}

template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::Synthetize()
{
  NeigboursMapType neighboursMap;

  // Merge the neighbours maps from all threads
  for (unsigned int threadId = 0; threadId < this->GetNumberOfThreads(); threadId++)
  {
    for (auto const& neighbours : m_NeighboursMapsTmp[threadId])
    {
      neighboursMap[neighbours.first].insert(neighbours.second.begin(), neighbours.second.end());
    }
  }

  // For each label of the label map, find the "closest" connected label,
  // according to the euclidian distance between the corresponding
  // m_labelStatistic elements.
  for (auto const& neighbours : neighboursMap)
  {
    double         proximity        = itk::NumericTraits<double>::max();
    InputLabelType label            = neighbours.first;
    InputLabelType closestNeighbour = label;
    auto const&    statsLabel       = m_LabelStatistic[label];

    for (auto const& neighbour : neighbours.second)
    {
      auto const& statsNeighbour = m_LabelStatistic[neighbour];
      assert(statsLabel.Size() == statsNeighbour.Size());

      double distance = (statsLabel - statsNeighbour).GetSquaredNorm();
      if (distance < proximity)
      {
        proximity        = distance;
        closestNeighbour = neighbour;
      }
    }

    auto curLabelLUT = FindCorrespondingLabel(label);
    auto adjLabelLUT = FindCorrespondingLabel(closestNeighbour);

    // Keep the smallest label (this prevents infinite loop in the LUT
    // (like LUT[i]=j and LUT[j]=i)
    if (curLabelLUT < adjLabelLUT)
    {
      m_LUT[adjLabelLUT] = curLabelLUT;
    }
    else
    {
      m_LUT[curLabelLUT] = adjLabelLUT;
    }
  }

  // Update the LUT
  for (auto& label : m_LUT)
  {
    label.second = FindCorrespondingLabel(label.first);
  }

  // Update statistics : for each newly merged segments, sum the population, and
  // recompute the mean.
  for (auto const& label : m_LUT)
  {
    if ((label.second != label.first) && (m_LabelPopulation[label.first] != 0))
    {
      // Cache values to reduce number of lookups
      auto const& populationFirst  = m_LabelPopulation[label.first];
      auto const& populationSecond = m_LabelPopulation[label.second];
      auto const& statisticFirst   = m_LabelStatistic[label.first];
      auto const& statisticSecond  = m_LabelStatistic[label.second];

      m_LabelStatistic[label.second] = ((statisticFirst * populationFirst) + (statisticSecond * populationSecond)) / (populationFirst + populationSecond);

      m_LabelPopulation[label.second] += populationFirst;

      // Do not use this label anymore
      m_LabelPopulation[label.first] = 0;
    }
  }
}

template <class TInputLabelImage>
typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::InputLabelType
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::FindCorrespondingLabel(
    typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::InputLabelType label)
{
  auto correspondingLabel = m_LUT[label];
  while (label != correspondingLabel)
  {
    label              = correspondingLabel;
    correspondingLabel = m_LUT[correspondingLabel];
  }
  return correspondingLabel;
}

template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input
  auto inputPtr = const_cast<TInputLabelImage*>(this->GetInput());


  // get a copy of the input requested region
  auto inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(1);
  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
  {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
  }
  else
  {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription(
        "Requested region is (at least partially) outside the "
        "largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
  }
}

template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  using IteratorType             = itk::ImageRegionConstIterator<TInputLabelImage>;
  using NeighborhoodIteratorType = itk::ConstShapedNeighborhoodIterator<TInputLabelImage>;

  typename NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1);

  auto labelImage = this->GetInput();

  IteratorType             it(labelImage, outputRegionForThread);
  NeighborhoodIteratorType itN(radius, labelImage, outputRegionForThread);

  // 4 connected Neighborhood (top, bottom, left and right)
  typename IteratorType::OffsetType top = {{0, -1}};
  itN.ActivateOffset(top);
  typename IteratorType::OffsetType bottom = {{0, 1}};
  itN.ActivateOffset(bottom);
  typename IteratorType::OffsetType right = {{1, 0}};
  itN.ActivateOffset(right);
  typename IteratorType::OffsetType left = {{-1, 0}};
  itN.ActivateOffset(left);

  for (it.GoToBegin(); !it.IsAtEnd(); ++it, ++itN)
  {
    assert(!itN.IsAtEnd());
    int currentLabel = m_LUT[it.Get()];

    if (m_LabelPopulation[currentLabel] == m_Size)
    {
      for (auto ci = itN.Begin(); !ci.IsAtEnd(); ci++)
      {
        int neighbourLabel = m_LUT[ci.Get()];
        if (neighbourLabel != currentLabel)
          m_NeighboursMapsTmp[threadId][currentLabel].insert(neighbourLabel);
      }
    }
  }
}

template <class TInputLabelImage>
void PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputLabelImage>
LabelImageSmallRegionMergingFilter<TInputLabelImage>::LabelImageSmallRegionMergingFilter() : m_MinSize(1)
{
  m_SmallRegionMergingFilter = LabelImageSmallRegionMergingFilterType::New();
}

template <class TInputLabelImage>
void LabelImageSmallRegionMergingFilter<TInputLabelImage>::SetInputLabelImage(const TInputLabelImage* labelImage)
{
  m_SmallRegionMergingFilter->GetFilter()->SetInput(labelImage);
}

template <class TInputLabelImage>
void LabelImageSmallRegionMergingFilter<TInputLabelImage>::SetLabelPopulation(LabelPopulationType const& labelPopulation)
{
  m_SmallRegionMergingFilter->GetFilter()->SetLabelPopulation(labelPopulation);
}

template <class TInputLabelImage>
typename LabelImageSmallRegionMergingFilter<TInputLabelImage>::LabelPopulationType const&
LabelImageSmallRegionMergingFilter<TInputLabelImage>::GetLabelPopulation() const
{
  return m_SmallRegionMergingFilter->GetFilter()->GetLabelPopulation();
}

template <class TInputLabelImage>
void LabelImageSmallRegionMergingFilter<TInputLabelImage>::SetLabelStatistic(LabelStatisticType const& labelStatistic)
{
  m_SmallRegionMergingFilter->GetFilter()->SetLabelStatistic(labelStatistic);
}

template <class TInputLabelImage>
typename LabelImageSmallRegionMergingFilter<TInputLabelImage>::LabelStatisticType const&
LabelImageSmallRegionMergingFilter<TInputLabelImage>::GetLabelStatistic() const
{
  return m_SmallRegionMergingFilter->GetFilter()->GetLabelStatistic();
}

template <class TInputLabelImage>
typename LabelImageSmallRegionMergingFilter<TInputLabelImage>::LUTType const& LabelImageSmallRegionMergingFilter<TInputLabelImage>::GetLUT() const
{
  return m_SmallRegionMergingFilter->GetFilter()->GetLUT();
}

template <class TInputLabelImage>
void LabelImageSmallRegionMergingFilter<TInputLabelImage>::Update()
{
  this->GenerateData();
}

template <class TInputLabelImage>
void LabelImageSmallRegionMergingFilter<TInputLabelImage>::GenerateData()
{
  this->SetProgress(0.0);

  // Update the filter for all sizes.
  for (unsigned int size = 1; size < m_MinSize; size++)
  {
    m_SmallRegionMergingFilter->GetFilter()->SetSize(size);
    m_SmallRegionMergingFilter->Update();
    this->UpdateProgress(static_cast<double>(size + 1) / m_MinSize);
  }
}


} // end namespace otb

#endif
