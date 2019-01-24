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

#ifndef otbStreamingShrinkImageFilter_hxx
#define otbStreamingShrinkImageFilter_hxx

#include "otbStreamingShrinkImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TImage>
StreamingShrinkStreamingManager<TImage>::StreamingShrinkStreamingManager()
 : m_ShrinkFactor(10)
{
}

template <class TImage>
StreamingShrinkStreamingManager<TImage>::~StreamingShrinkStreamingManager()
{
}

template <class TImage>
void
StreamingShrinkStreamingManager<TImage>::PrepareStreaming( itk::DataObject * input, const RegionType &region )
{
  typedef otb::StreamingShrinkImageRegionSplitter TileSplitterType;
  TileSplitterType::Pointer splitter = TileSplitterType::New();
  splitter->SetShrinkFactor(m_ShrinkFactor);
  this->m_Splitter = splitter;

  unsigned long nbDivisions = this->EstimateOptimalNumberOfDivisions(input, region, 0);
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);
  otbMsgDevMacro(<< "Number of split : " << this->m_ComputedNumberOfSplits)

  // Save the region to generate the splits later
  this->m_Region = region;
}


/** Constructor */
template <class TInputImage, class TOutputImage>
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::PersistentShrinkImageFilter()
 : m_ShrinkFactor(10)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}

/** Destructor */
template <class TInputImage, class TOutputImage>
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::~PersistentShrinkImageFilter()
{

}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  const InputImageType*  input = this->GetInput();

  OutputImageType* output = this->GetOutput();

  if (input)
    {
    output->CopyInformation(input);
    output->SetLargestPossibleRegion(input->GetLargestPossibleRegion());

    if (output->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      output->SetRequestedRegion(output->GetLargestPossibleRegion());
      }
    }
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}


template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::Reset()
{
  // Get pointers to the input and output
  InputImageType* inputPtr = const_cast<InputImageType*>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  m_ShrunkOutput = OutputImageType::New();
  m_ShrunkOutput->CopyInformation(inputPtr);

  const typename InputImageType::SpacingType&
                                           inputSpacing = inputPtr->GetSignedSpacing();
  const typename InputImageType::SizeType& inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename InputImageType::IndexType& inputIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  typename InputImageType::IndexType startIndex;

  typename OutputImageType::SpacingType shrunkOutputSpacing;
  typename OutputImageType::RegionType  shrunkOutputLargestPossibleRegion;
  typename OutputImageType::SizeType    shrunkOutputSize;
  typename OutputImageType::IndexType   shrunkOutputStartIndex;
  typename OutputImageType::PointType   shrunkOutputOrigin;

  for (unsigned int i = 0; i < OutputImageType::ImageDimension; ++i)
    {
    startIndex[i] = inputIndex[i] + (m_ShrinkFactor - 1) / 2;
    if (m_ShrinkFactor > inputSize[i])
      startIndex[i] = inputIndex[i] + (inputSize[i] - 1) / 2;
    m_Offset[i] = startIndex[i] % m_ShrinkFactor;
    shrunkOutputSpacing[i] = inputSpacing[i] * static_cast<double>(m_ShrinkFactor);
    shrunkOutputSize[i] = inputSize[i] > m_ShrinkFactor ? inputSize[i] / m_ShrinkFactor : 1;
    
    shrunkOutputOrigin[i] = inputPtr->GetOrigin()[i] + inputSpacing[i] * startIndex[i];

    // we choose to output a region with a start index [0,0]
    // the origin is set accordingly
    shrunkOutputStartIndex[i] = 0;
    }

  m_ShrunkOutput->SetSignedSpacing(shrunkOutputSpacing);
  m_ShrunkOutput->SetOrigin(shrunkOutputOrigin);

  shrunkOutputLargestPossibleRegion.SetSize(shrunkOutputSize);
  shrunkOutputLargestPossibleRegion.SetIndex(shrunkOutputStartIndex);

  m_ShrunkOutput->SetRegions(shrunkOutputLargestPossibleRegion);
  m_ShrunkOutput->Allocate();
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::Synthetize()
{
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  //std::cout << "outputRegionForThread " << threadId << "  " << outputRegionForThread << std::endl;
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  const InputImageType*  inputPtr = this->GetInput();

  itk::ImageRegionConstIteratorWithIndex<InputImageType> inIt(inputPtr, outputRegionForThread);
  for(inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, progress.CompletedPixel())
    {
    const IndexType& inIndex = inIt.GetIndex();
    // TODO the pixel value should be taken near the centre of the cell, not at the corners
    if ((inIndex[0] - m_Offset[0]) % m_ShrinkFactor == 0
        && (inIndex[1] - m_Offset[1]) % m_ShrinkFactor == 0 )
      {
      IndexType shrunkIndex;
      shrunkIndex[0] = (inIndex[0] - m_Offset[0]) / m_ShrinkFactor;
      shrunkIndex[1] = (inIndex[1] - m_Offset[1]) / m_ShrinkFactor;
      if (m_ShrunkOutput->GetLargestPossibleRegion().IsInside(shrunkIndex))
        m_ShrunkOutput->SetPixel(shrunkIndex, inIt.Get());
      }
    }
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData()
{
}

template <class TImage, class TOutputImage>
void
PersistentShrinkImageFilter<TImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Shrink factor: " << m_ShrinkFactor << std::endl;
}

} // End namespace otb
#endif
