/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "itkImageConstIterator.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkProgressReporter.h"


namespace otb
{
template <class TInputLabelImage, class TInputSpectralImage>
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::PersistentLabelImageSmallRegionMergingFilter() : m_Size(1)
{
}

template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::SetInputLabelImage( const TInputLabelImage * labelImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputLabelImage *>( labelImage ));
}

template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::SetInputSpectralImage( const TInputSpectralImage * spectralImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputSpectralImage *>( spectralImage ));
}

template <class TInputLabelImage, class TInputSpectralImage>
TInputLabelImage *
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::GetInputLabelImage()
{
  return dynamic_cast<TInputLabelImage*>(itk::ProcessObject::GetInput(0));
}

template <class TInputLabelImage, class TInputSpectralImage>
TInputSpectralImage *
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::GetInputSpectralImage()
{
  return dynamic_cast<TInputSpectralImage*>(itk::ProcessObject::GetInput(1));
}

template <class TInputLabelImage, class TInputSpectralImage>
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::~PersistentLabelImageSmallRegionMergingFilter()
{

}


template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::Reset()
{
  std::cout << "Reset" << std::endl;
  
  m_NeighboursMapsTmp.clear();
  m_NeighboursMapsTmp.resize( this->GetNumberOfThreads() );
}


template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::Synthetize()
{
  std::cout << "Synthetize" << std::endl;
  NeigboursMapType neighboursMap;
  // Merge the neighbours maps from all threads
  for( unsigned int threadId = 0; threadId < this->GetNumberOfThreads(); threadId++)
  {
    for (auto it = m_NeighboursMapsTmp[threadId].begin(); it != m_NeighboursMapsTmp[threadId].end(); it++)
    {
      neighboursMap[ it->first ].insert( it->second.begin(), it->second.end() );
    }
  }
  
  // For each label of the label map, find the "closest" connected label, according 
  // to the euclidian distance between the corresponding m_labelStatistic elements.
  for (auto neighbours : neighboursMap)
  {
    double proximity = std::numeric_limits<double>::max();
    InputLabelType label = neighbours.first;
    InputLabelType closestNeighbour = label;
    for (auto neighbour : neighbours.second)
    {
      
      auto statsLabel = m_LabelStatistic[ label ];
      auto statsNeighbour = m_LabelStatistic[ neighbour ];
      assert( statsLabel.Size() == statsNeighbour.Size() );
      double distance = 0;
      for (int i = 0 ; i < statsLabel.Size(); i++)
      {
        distance += pow( statsLabel[i] - statsNeighbour[i] , 2);
      }
      
      if (distance < proximity)
      {
        proximity = distance;
        closestNeighbour = neighbour;
      }
      std::cout << label << " " << neighbour << " " << distance << " " << closestNeighbour <<std::endl;
    }
    m_CorrespondanceMap[label] = closestNeighbour;
    //InputLabelType neighbor = itLabel->first;
    //
    
    /*for (auto itNeigh = itLabel->second.begin(); itNeigh != itLabel->second.end(); ++itNeigh)
    {
      // Compute squared distance between the current label and the current neighbour
      //double dist = std::abs( std::pow( m_labelStatistic[itLabel->first], 2) - std::pow( m_labelStatistic[*itNeigh], 2) );
      for (auto 
    }*/
  }
  
  
  // We have to update corresponding label to propagate the correspondance between the labels.
  for (auto corres : m_CorrespondanceMap)
  {
    corres.second = FindCorrespondingLabel(corres.second);
    std::cout << "label : " << corres.first << " closest : " << corres.second << std::endl;
  }
  
  
  
  /*
  for (auto it = m_NeighboursMap.begin(); it != m_NeighboursMap.end(); it++)
  {
    std::cout << it->first << " " ;
    for  (auto itSet = it->second.begin(); itSet != it->second.end(); itSet++)
      std::cout << *itSet << " " ;
    std::cout << std::endl;
  }*/
}

template <class TInputLabelImage, class TInputSpectralImage>
typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>::InputLabelType
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::FindCorrespondingLabel( typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
                            ::InputLabelType label)
{
  auto correspondingLabel = m_CorrespondanceMap[label];
  while (label != correspondingLabel)
  {std::cout << "yo" << std::endl;
    label = correspondingLabel;
    correspondingLabel = m_CorrespondanceMap[correspondingLabel];
  }
  return correspondingLabel;
}



template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
}

template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId )
{
  std::cout << threadId << " " << outputRegionForThread.GetSize() << " " << outputRegionForThread.GetIndex() << std::endl;
  
  using IteratorType = itk::ImageRegionConstIterator< TInputLabelImage >;
  using NeighborhoodIteratorType = itk::ConstShapedNeighborhoodIterator< TInputLabelImage >;
  typename NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1);

  auto labelImage = this->GetInputLabelImage();
  
  IteratorType it(labelImage, outputRegionForThread);
  NeighborhoodIteratorType itN(radius, labelImage, outputRegionForThread);
  
  // 4 connected Neighborhood (top, bottom, left and right)
  typename IteratorType::OffsetType top = {{0,-1}};
  itN.ActivateOffset(top);
  typename IteratorType::OffsetType bottom = {{0,1}};
  itN.ActivateOffset(bottom);
  typename IteratorType::OffsetType right = {{1,0}};
  itN.ActivateOffset(right);
  typename IteratorType::OffsetType left = {{-1,0}};
  itN.ActivateOffset(left);
    
  for (it.GoToBegin(); ! it.IsAtEnd(); ++it, ++itN)
  {
    assert( !itN.IsAtEnd() );
    //if ( it.Get() == m_Size )
    if ( m_LabelPopulation[it.Get()] == m_Size )
    {
      //std::cout << it.Get() << std::endl;
      for (auto ci = itN.Begin() ; !ci.IsAtEnd(); ci++)
      {
        int neighbourLabel = FindCorrespondingLabel(ci.Get() );
        std::cout << neighbourLabel << " " << ci.Get() << " " << it.Get() << std::endl; 
        if (neighbourLabel != it.Get() && m_LabelPopulation[neighbourLabel] > m_Size)
          m_NeighboursMapsTmp[threadId][ it.Get() ].insert( neighbourLabel );
      }
    }
  }

  
  
}

template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb

#endif
