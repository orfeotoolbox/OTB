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
#ifndef __otbStreamingShrinkImageFilter_txx
#define __otbStreamingShrinkImageFilter_txx

#include "otbStreamingShrinkImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TImage>
StreamingShrinkStreamingManager<TImage>::StreamingShrinkStreamingManager()
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
  splitter->SetTileSizeAlignment(m_ShrinkFactor);

  //Uses the TileHint from the MetaDataDictionnary to find out
  //the tiling sheme of the input file if available.
  typename otb::StreamingShrinkStreamingManager<TImage>::SizeType tileHint;

  unsigned int tileHintX(0), tileHintY(0);

  itk::ExposeMetaData<unsigned int>(input->GetMetaDataDictionary(),
                                    MetaDataKey::TileHintX,
                                    tileHintX);

  itk::ExposeMetaData<unsigned int>(input->GetMetaDataDictionary(),
                                    MetaDataKey::TileHintY,
                                    tileHintY);

  tileHint[0] = tileHintX;
  tileHint[1] = tileHintY;

  splitter->SetTileHint(tileHint);

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

  m_ShrinkedOutput = OutputImageType::New();
  m_ShrinkedOutput->CopyInformation(inputPtr);

  const typename InputImageType::SpacingType&
                                           inputSpacing = inputPtr->GetSpacing();
  const typename InputImageType::SizeType& inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();

  typename OutputImageType::SpacingType shrinkedOutputSpacing;
  typename OutputImageType::RegionType  shrinkedOutputLargestPossibleRegion;
  typename OutputImageType::SizeType    shrinkedOutputSize;
  typename OutputImageType::IndexType   shrinkedOutputStartIndex;

  for (unsigned int i = 0; i < OutputImageType::ImageDimension; ++i)
    {
    shrinkedOutputSpacing[i] = inputSpacing[i] * static_cast<double>(m_ShrinkFactor);
    shrinkedOutputSize[i] = inputSize[i] > m_ShrinkFactor ? inputSize[i] / m_ShrinkFactor : 1;

    // TODO : don't know what to do here.
    // dividing the input index by the shrink factor does not make a lot of sense...
    shrinkedOutputStartIndex[i] = 0;
    }

  m_ShrinkedOutput->SetSpacing(shrinkedOutputSpacing);

  shrinkedOutputLargestPossibleRegion.SetSize(shrinkedOutputSize);
  shrinkedOutputLargestPossibleRegion.SetIndex(shrinkedOutputStartIndex);

  m_ShrinkedOutput->SetRegions(shrinkedOutputLargestPossibleRegion);
  m_ShrinkedOutput->Allocate();
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
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  //std::cout << "outputRegionForThread " << threadId << "  " << outputRegionForThread << std::endl;
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  const InputImageType*  inputPtr = this->GetInput();

  itk::ImageRegionConstIteratorWithIndex<InputImageType> inIt(inputPtr, outputRegionForThread);
  for(inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, progress.CompletedPixel())
    {
    const IndexType& inIndex = inIt.GetIndex();
    if (inIndex[0] % m_ShrinkFactor == 0
        && inIndex[1] % m_ShrinkFactor == 0 )
      {
      IndexType shrinkedIndex;
      shrinkedIndex[0] = inIndex[0] / m_ShrinkFactor;
      shrinkedIndex[1] = inIndex[1] / m_ShrinkFactor;
      if (m_ShrinkedOutput->GetLargestPossibleRegion().IsInside(shrinkedIndex))
        m_ShrinkedOutput->SetPixel(shrinkedIndex, inIt.Get());
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
