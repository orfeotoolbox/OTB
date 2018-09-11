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

#include <time.h>
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
  m_NeighboursMapsTmp.clear();
  m_NeighboursMapsTmp.resize( this->GetNumberOfThreads() );
}


template <class TInputLabelImage, class TInputSpectralImage>
void
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::Synthetize()
{
  clock_t tic = clock();
  NeigboursMapType neighboursMap;
  // Merge the neighbours maps from all threads
  for( unsigned int threadId = 0; threadId < this->GetNumberOfThreads(); threadId++)
  {
    for (auto it = m_NeighboursMapsTmp[threadId].begin(); it != m_NeighboursMapsTmp[threadId].end(); it++)
    {
      neighboursMap[ it->first ].insert( it->second.begin(), it->second.end() );
    }
  }
  
  // LUTtmp creation in order to modify the LUT only at the end of the pass
  auto CorrespondanceMapTmp (m_CorrespondanceMap);
  
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
      //std::cout << label << " " << neighbour << " " << distance << " " << m_LabelStatistic[ neighbour ]  <<std::endl;
      if (distance < proximity)
      {
        proximity = distance;
        closestNeighbour = neighbour;
      }
    }

    auto curLabelLUT = label;
    auto adjLabelLUT = closestNeighbour;
    std::cout << label << " " << closestNeighbour;
    while(CorrespondanceMapTmp[curLabelLUT] != curLabelLUT)
    {
      curLabelLUT = CorrespondanceMapTmp[curLabelLUT];
    }
    while(CorrespondanceMapTmp[adjLabelLUT] != adjLabelLUT)
    {
      adjLabelLUT = CorrespondanceMapTmp[adjLabelLUT];
    }
    
    if(curLabelLUT < adjLabelLUT)
    {
      CorrespondanceMapTmp[adjLabelLUT] = curLabelLUT;
    }
    else
    {
      CorrespondanceMapTmp[CorrespondanceMapTmp[curLabelLUT]] = adjLabelLUT; 
      CorrespondanceMapTmp[curLabelLUT] = adjLabelLUT;
    }
                  
    
    
    
    //m_CorrespondanceMap[label] = closestNeighbour;
    
    // Update Stats
    /*m_LabelStatistic[closestNeighbour] = (m_LabelStatistic[closestNeighbour]*m_LabelPopulation[closestNeighbour] + 
                        m_LabelStatistic[label]*m_LabelPopulation[label] ) / (m_LabelPopulation[label]+m_LabelPopulation[closestNeighbour]);
    m_LabelPopulation[closestNeighbour] += m_LabelPopulation[label];*/
    
  }
  
  for(InputLabelType label = 0; label < CorrespondanceMapTmp.size(); ++label)
  {
    InputLabelType can = label;
    while(CorrespondanceMapTmp[can] != can)
    {
      can = CorrespondanceMapTmp[can];
    }
  CorrespondanceMapTmp[label] = can;
  }

  
  for(InputLabelType label = 0; label < CorrespondanceMapTmp.size(); ++label)
  {
    InputLabelType correspondingLabel = CorrespondanceMapTmp[label];
    m_CorrespondanceMap[label] = correspondingLabel;
    
    if((m_LabelPopulation[label]!=0) && (correspondingLabel != label))
    {
      m_LabelStatistic[ correspondingLabel ] = (m_LabelStatistic[correspondingLabel]*m_LabelPopulation[correspondingLabel] + 
                        m_LabelStatistic[label]*m_LabelPopulation[label] ) / (m_LabelPopulation[label]+m_LabelPopulation[correspondingLabel]);
      m_LabelPopulation[ correspondingLabel ] += m_LabelPopulation[ label ] ;
      m_LabelPopulation[ label ] = 0;
    }
    
  }
  /*
  // We have to update corresponding label to propagate the correspondance between the labels.
  for (auto & corres : m_CorrespondanceMap)
  {
    corres = FindCorrespondingLabel(corres);
   // corres.second = FindCorrespondingLabel(corres.second); //TODO
  }*/
  clock_t toc = clock();
  
  std::cout << "Synthetize : " << (double)(toc - tic) / CLOCKS_PER_SEC << std::endl;
}

template <class TInputLabelImage, class TInputSpectralImage>
typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>::InputLabelType
PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
::FindCorrespondingLabel( typename PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage>
                            ::InputLabelType label)
{
  auto correspondingLabel = m_CorrespondanceMap[label];
  while (label != correspondingLabel)
  {
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
  
  clock_t tic = clock();
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
    int currentLabel = FindCorrespondingLabel(it.Get());
    
    //if ( it.Get() == m_Size )
    if ( m_LabelPopulation[currentLabel] == m_Size )
    {
      for (auto ci = itN.Begin() ; !ci.IsAtEnd(); ci++)
      {
        //int neighbourLabel = ci.Get();
        int neighbourLabel = FindCorrespondingLabel(ci.Get() );
        //if (neighbourLabel != it.Get() && m_LabelPopulation[neighbourLabel] > m_Size)
        if (neighbourLabel != currentLabel)
          m_NeighboursMapsTmp[threadId][ currentLabel ].insert( neighbourLabel );
      }
    }
  }

  clock_t toc = clock();

  if (threadId==0)
    std::cout << threadId << " " << this->GetNumberOfThreads() << " Elapsed time : " << (double)(toc - tic) / CLOCKS_PER_SEC << std::endl;
  
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
