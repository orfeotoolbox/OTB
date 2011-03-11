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

namespace otb
{

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

  const InputImageType* input = this->GetInput();
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
  // Reinit the chrono
  m_Chrono = itk::TimeProbe();

  // get pointers to the input and output
  const InputImageType*  inputPtr = this->GetInput();

  m_ShrinkedOutput = OutputImageType::New();

  // we need to compute the output spacing, the output image size, and the
  // output image start index
  const typename InputImageType::SpacingType&
                                           inputSpacing = inputPtr->GetSpacing();
  const typename InputImageType::SizeType& inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename InputImageType::IndexType& inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  
  otbMsgDebugMacro(<< "Input index " << inputStartIndex);
  otbMsgDebugMacro(<< "Input size: " << inputSize);

  typename OutputImageType::SpacingType shrinkedOutputSpacing;
  typename OutputImageType::RegionType  shrinkedOutputLargestPossibleRegion;
  typename OutputImageType::SizeType    shrinkedOutputSize;
  typename OutputImageType::IndexType   shrinkedOutputStartIndex;

  for (unsigned int i = 0; i < OutputImageType::ImageDimension; ++i)
    {
    shrinkedOutputSpacing[i] = inputSpacing[i] * static_cast<double>(m_ShrinkFactor);
    //shrinkedOutputSize[i] = static_cast<int>(static_cast<double>(inputSize[i]) / static_cast<double>(m_ShrinkFactor));
    shrinkedOutputSize[i] = inputSize[i] / m_ShrinkFactor;
    //outputStartIndex[i] = inputStartIndex[i];
    shrinkedOutputStartIndex[i] = 0;
    }

  m_ShrinkedOutput->SetSpacing(shrinkedOutputSpacing);
  m_ShrinkedOutput->SetNumberOfComponentsPerPixel(inputPtr->GetNumberOfComponentsPerPixel());

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
  std::cout << "Shrink time : " << m_Chrono.GetTotal() << std::endl;
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  m_Chrono.Start();
}

template<class TInputImage, class TOutputImage>
void
PersistentShrinkImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  //std::cout << "outputRegionForThread " << threadId << "  " << outputRegionForThread << std::endl;
  const InputImageType*  inputPtr = this->GetInput();

  itk::ImageRegionConstIteratorWithIndex<InputImageType> inIt(inputPtr, outputRegionForThread);
  for(inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
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
  m_Chrono.Stop();
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
