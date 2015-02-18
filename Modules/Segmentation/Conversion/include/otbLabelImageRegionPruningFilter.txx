/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbLabelImageRegionPruningFilter_txx
#define __otbLabelImageRegionPruningFilter_txx

#include "otbLabelImageRegionPruningFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"


namespace otb
{
template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::LabelImageRegionPruningFilter()
{
  m_MinRegionSize=100;

  this->SetNumberOfRequiredInputs( 2 );

  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0, OutputLabelImageType::New());
  this->SetNthOutput(1, OutputClusteredImageType::New());

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
void
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::SetInputLabelImage( const TInputLabelImage * labelImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputLabelImage *>( labelImage ));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
void
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::SetInputSpectralImage( const TInputSpectralImage * spectralImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputSpectralImage *>( spectralImage ));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
TInputLabelImage *
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GetInputLabelImage()
{
  return dynamic_cast<TInputLabelImage*>(itk::ProcessObject::GetInput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
TInputSpectralImage *
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GetInputSpectralImage()
{
  return dynamic_cast<TInputSpectralImage*>(itk::ProcessObject::GetInput(1));
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::~LabelImageRegionPruningFilter()
{

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
TOutputLabelImage *
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GetLabelOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
      return 0;
    }
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
const TOutputLabelImage *
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GetLabelOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
      return 0;
    }
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
typename LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>::OutputClusteredImageType *
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GetClusteredOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
      return 0;
    }
  return static_cast<OutputClusteredImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
const typename LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>::OutputClusteredImageType *
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GetClusteredOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
      return 0;
    }
  return static_cast<OutputClusteredImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
void
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  unsigned int numberOfComponentsPerPixel = this->GetInputSpectralImage()->GetNumberOfComponentsPerPixel();

  if(this->GetClusteredOutput())
    {
    this->GetClusteredOutput()->SetNumberOfComponentsPerPixel(numberOfComponentsPerPixel);
    }

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
void
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::EnlargeOutputRequestedRegion( itk::DataObject *itkNotUsed(output) )
 {
  // This filter requires all of the output images in the buffer.
  for ( unsigned int j = 0; j < this->GetNumberOfOutputs(); j++ )
    {
    if ( this->itk::ProcessObject::GetOutput(j) )
      {
      this->itk::ProcessObject::GetOutput(j)->SetRequestedRegionToLargestPossibleRegion();
      }
    }
  }

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
void
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::GenerateData()
{
  typename InputSpectralImageType::Pointer spectralImage = this->GetInputSpectralImage();
  typename InputLabelImageType::Pointer inputLabelImage = this->GetInputLabelImage();
  typename OutputLabelImageType::Pointer outputLabelImage = this->GetLabelOutput();
  typename OutputClusteredImageType::Pointer outputClusteredImage = this->GetClusteredOutput();

  // Allocate output
  outputLabelImage->SetBufferedRegion( outputLabelImage->GetRequestedRegion() );
  outputLabelImage->Allocate();
  outputClusteredImage->SetBufferedRegion( outputClusteredImage->GetRequestedRegion() );
  outputClusteredImage->Allocate();

  m_NumberOfComponentsPerPixel = spectralImage->GetNumberOfComponentsPerPixel();

  //std::cout << "Copy input label image to output label image" << std::endl;
  // Copy input label image to output label image
  typename itk::ImageRegionConstIterator<InputLabelImageType> inputIt(inputLabelImage, outputLabelImage->GetRequestedRegion());
  typename itk::ImageRegionIterator<OutputLabelImageType> outputIt(outputLabelImage, outputLabelImage->GetRequestedRegion());
  inputIt.GoToBegin();
  outputIt.GoToBegin();
  while(!inputIt.IsAtEnd())
    {
    outputIt.Set(inputIt.Get());
    ++inputIt;
    ++outputIt;
    }

  RegionAdjacencyMapType regionAdjacencyMap = LabelImageToRegionAdjacencyMap(outputLabelImage);
  unsigned int regionCount = regionAdjacencyMap.size() - 1;

  // Initialize arrays for mode information
  m_CanonicalLabels.clear();
  m_CanonicalLabels.resize(regionCount+1);

  m_Modes.clear();
  m_Modes.reserve(regionCount+1);
  for(unsigned int i = 0; i < regionCount+1; ++i)
    {
    m_Modes.push_back( SpectralPixelType(m_NumberOfComponentsPerPixel) );
    }

  m_PointCounts.clear();
  m_PointCounts.resize(regionCount+1); // = std::vector<unsigned int>(regionCount+1);


  // Associate each label to a spectral value, a canonical label and a point count
  typename itk::ImageRegionConstIterator<InputLabelImageType> inputItWithIndex(inputLabelImage, outputLabelImage->GetRequestedRegion());
  inputItWithIndex.GoToBegin();
  while(!inputItWithIndex.IsAtEnd())
    {
    LabelType label = inputItWithIndex.Get();
    // if label has not been initialized yet ..
    if(m_PointCounts[label] == 0)
      {
     // m_CanonicalLabels[label] = label;
      m_Modes[label] = spectralImage->GetPixel(inputItWithIndex.GetIndex());
      }
    m_PointCounts[label]++;
    ++inputItWithIndex;
    }
  // Region Pruning

  bool finishedPruning = false;
  unsigned int pruneIterations = 0;
  unsigned int minRegionCount = 0;
  do{

     minRegionCount = 0;


      // Initialize Canonical Labels
      for(LabelType curLabel = 1; curLabel <= regionCount; ++curLabel)
        {
        m_CanonicalLabels[curLabel] = curLabel;
       /* if(m_PointCounts[curLabel] <= m_MinRegionSize)
                {
                  minRegionCount++;
                }*/
        }
    // std::cout<<minRegionCount<<" regions are too be fused"<<std::endl;
    // Iterate over all regions

      for(LabelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      {

      if((m_PointCounts[curLabel] ==0) || (m_PointCounts[curLabel] > m_MinRegionSize))
        {
        // do not process empty regions
        continue;
        }
      minRegionCount++;
     // std::cout<<" point in label "<<curLabel<<" "<<m_PointCounts[curLabel]<<std::endl;
      const SpectralPixelType & curSpectral = m_Modes[curLabel];

      // Iterate over all adjacent regions and check for merge
      typename AdjacentLabelsContainerType::const_iterator adjIt = regionAdjacencyMap[curLabel].begin();

      LabelType neighborCandidate=0;
      RealType bestNorm2=itk::NumericTraits< float >::max();

      while (adjIt != regionAdjacencyMap[curLabel].end())
        {
        LabelType adjLabel = *adjIt;
        assert(adjLabel <= regionCount);
        const SpectralPixelType & adjSpectral = m_Modes[adjLabel];
        // Check condition to merge regions


        //find the spectrally nearest adjacent label

        RealType norm2 = 0;
         for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; ++comp)
          {
          RealType e;
          e = (curSpectral[comp] - adjSpectral[comp]);
          norm2 += e*e;
          }
        if(norm2 < bestNorm2)
          {
           bestNorm2=norm2;
           neighborCandidate=adjLabel;
          };
         ++adjIt;
        } // end of loop over adjacent labels

        if(neighborCandidate!=0)
          {
          // Find canonical label for current region
          LabelType curCanLabel = curLabel; //m_CanonicalLabels[curLabel];
          while(m_CanonicalLabels[curCanLabel] != curCanLabel)
            {
            curCanLabel = m_CanonicalLabels[curCanLabel];
            }

          // Find canonical label for adjacent region
          LabelType adjCanLabel =neighborCandidate; //m_CanonicalLabels[curLabel];
          while(m_CanonicalLabels[adjCanLabel] != adjCanLabel)
            {
            adjCanLabel = m_CanonicalLabels[adjCanLabel];
            }

          // Assign same canonical label to both regions
          if(curCanLabel < adjCanLabel)
            {
            m_CanonicalLabels[adjCanLabel] = curCanLabel;
            }
          else
            {
            m_CanonicalLabels[m_CanonicalLabels[curCanLabel]] = adjCanLabel;
            m_CanonicalLabels[curCanLabel] = adjCanLabel;
            }
          }


      } // end of loop over labels

    //std::cout << "Simplify the table of canonical labels" << std::endl;
    /* Simplify the table of canonical labels */
    for(LabelType i = 1; i < regionCount+1; ++i)
      {
      LabelType can = i;
      while(m_CanonicalLabels[can] != can)
        {
        can = m_CanonicalLabels[can];
        }
      m_CanonicalLabels[i] = can;
      }

    //std::cout << "merge regions with same canonical label" << std::endl;
    /* Merge regions with same canonical label */
    /* - update modes and point counts */
    std::vector<SpectralPixelType> newModes;
    newModes.reserve(regionCount+1); //(regionCount+1, SpectralPixelType(m_NumberOfComponentsPerPixel));
    for(unsigned int i = 0; i < regionCount+1; ++i)
      {
      newModes.push_back( SpectralPixelType(m_NumberOfComponentsPerPixel) );
      }
    std::vector<unsigned int> newPointCounts(regionCount+1);

    for(unsigned int i = 1; i < regionCount+1; ++i)
      {
      newModes[i].Fill(0);
      newPointCounts[i] = 0;
      }

    for(unsigned int i = 1; i < regionCount+1; ++i)
      {
      LabelType canLabel = m_CanonicalLabels[i];
      unsigned int nPoints = m_PointCounts[i];
      for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; ++comp)
        {
        newModes[canLabel][comp] += nPoints * m_Modes[i][comp];
        }
      newPointCounts[canLabel] += nPoints;
      }

    //std::cout << "re-labeling" << std::endl;
    /* re-labeling */
    std::vector<LabelType> newLabels(regionCount+1);
    std::vector<bool> newLabelSet(regionCount+1);
    for(unsigned int i = 1; i < regionCount+1; ++i)
      {
      newLabelSet[i] = false;
      }

    LabelType label = 0;
    for(unsigned int i = 1; i < regionCount+1; ++i)
      {
      LabelType canLabel = m_CanonicalLabels[i];
      if(newLabelSet[canLabel] == false)
        {
        newLabelSet[canLabel] = true;
        label++;
        newLabels[canLabel] = label;
        unsigned int nPoints = newPointCounts[canLabel];
        for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; ++comp)
          {
          m_Modes[label][comp] = newModes[canLabel][comp] / nPoints;
          }

        m_PointCounts[label] = newPointCounts[canLabel];
        }
      }

    unsigned int oldRegionCount = regionCount;

    regionCount = label;

    /* reassign labels in label image */
    outputIt.GoToBegin();
    while(!outputIt.IsAtEnd())
      {

      LabelType l = outputIt.Get();
      LabelType canLabel;
      itkAssertOrThrowMacro(m_CanonicalLabels[l] <= oldRegionCount,"Found a label greater than region count")
      canLabel = newLabels[m_CanonicalLabels[l]];
      outputIt.Set( canLabel );
      ++outputIt;
      }


    finishedPruning =  !minRegionCount ||  regionCount == 1 ||  pruneIterations>=10;

    // only one iteration for now

    if(!finishedPruning)
      {
      /* Update adjacency table */
      regionAdjacencyMap = LabelImageToRegionAdjacencyMap(outputLabelImage);
      }

    pruneIterations++;
    }while(!finishedPruning);

    // end of main iteration loop
  // std::cout << "merge iterations: " << mergeIterations << std::endl;
  // std::cout << "number of label objects: " << regionCount << std::endl;

  // Generate clustered output
  itk::ImageRegionIterator<OutputClusteredImageType> outputClusteredIt(outputClusteredImage, outputClusteredImage->GetRequestedRegion() );
  outputClusteredIt.GoToBegin();
  outputIt.GoToBegin();
  while( !outputClusteredIt.IsAtEnd() )
    {
    LabelType label = outputIt.Get();
    const SpectralPixelType & p = m_Modes[ label ];
    outputClusteredIt.Set(p);
    ++outputClusteredIt;
    ++outputIt;
    }
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
void
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Minimum Region Size: "                  << m_MinRegionSize                 << std::endl;
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage, class TOutputClusteredImage>
typename LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>::RegionAdjacencyMapType
LabelImageRegionPruningFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage, TOutputClusteredImage>
::LabelImageToRegionAdjacencyMap(typename OutputLabelImageType::Pointer  labelImage)
{
  // declare the output map
  RegionAdjacencyMapType ram;

  // Find the maximum label value
  itk::ImageRegionConstIterator<OutputLabelImageType> it(labelImage, labelImage->GetRequestedRegion());
  it.GoToBegin();
  LabelType maxLabel = 0;
  while(!it.IsAtEnd())
    {
    LabelType label = it.Get();
    maxLabel = vcl_max(maxLabel, label);
    ++it;
    }

  // Set the size of the adjacency map
  ram.resize(maxLabel+1);

  // set the image region without bottom and right borders so that bottom and
  // right neighbors always exist
  RegionType regionWithoutBottomRightBorders  = labelImage->GetRequestedRegion();
  SizeType size = regionWithoutBottomRightBorders.GetSize();
  for(unsigned int d = 0; d < ImageDimension; ++d) size[d] -= 1;
  regionWithoutBottomRightBorders.SetSize(size);
  itk::ImageRegionConstIteratorWithIndex<OutputLabelImageType> inputIt(labelImage, regionWithoutBottomRightBorders);

  inputIt.GoToBegin();
  while(!inputIt.IsAtEnd())
    {
    const InputIndexType & index = inputIt.GetIndex();
    LabelType label = inputIt.Get();

    // check neighbors
    for(unsigned int d = 0; d < ImageDimension; ++d)
      {
      InputIndexType neighborIndex = index;
      neighborIndex[d]++;

      LabelType neighborLabel = labelImage->GetPixel(neighborIndex);

      // add adjacency if different labels
      if(neighborLabel != label)
        {
        ram[label].insert(neighborLabel);
        ram[neighborLabel].insert(label);
        }
      }
    ++inputIt;
    }

  return ram;
}

} // end namespace otb

#endif
