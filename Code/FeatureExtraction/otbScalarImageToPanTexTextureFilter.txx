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
#ifndef __otbScalarImageToPanTexTextureFilter_txx
#define __otbScalarImageToPanTexTextureFilter_txx

#include "otbScalarImageToPanTexTextureFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::ScalarImageToPanTexTextureFilter() : m_Radius(),
  m_NumberOfBinsPerAxis(8),
  m_InputImageMinimum(0),
  m_InputImageMaximum(256)
{
  // There are 1 output corresponding to the Pan Tex texture indice
  this->SetNumberOfRequiredOutputs(1);

  //Fill the offset list for contrast computation
  OffsetType off;
  off[0] = 0;
  off[1] = 1;
  m_OffsetList.push_back(off);   //(0, 1)
  off[1] = 2;
  m_OffsetList.push_back(off);   //(0, 2)
  off[0] = 1;
  off[1] = -2;
  m_OffsetList.push_back(off);   //(1, -2)
  off[1] = -1;
  m_OffsetList.push_back(off);   //(1, -1)
  off[1] = 0;
  m_OffsetList.push_back(off);   //(1, 0)
  off[1] = 1;
  m_OffsetList.push_back(off);   //(1, 1)
  off[1] = 2;
  m_OffsetList.push_back(off);   //(1, 2)
  off[0] = 2;
  off[1] = -1;
  m_OffsetList.push_back(off);   //(2, -1)
  off[1] = 0;
  m_OffsetList.push_back(off);   //(2, 0)
  off[1] = 1;
  m_OffsetList.push_back(off);   //(2, 1)
}

template <class TInputImage, class TOutputImage>
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::~ScalarImageToPanTexTextureFilter()
{}

template <class TInputImage, class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // First, call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // Retrieve the output requested region
  // We use only the first output since requested regions for all outputs are enforced to be equal
  // by the default GenerateOutputRequestedRegiont() implementation
  OutputRegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

  // Build the input requested region
  InputRegionType inputRequestedRegion = outputRequestedRegion;

  // Apply the radius
  SizeType maxOffsetSize;
  maxOffsetSize[0] = 2;
  maxOffsetSize[1] = 2;
  inputRequestedRegion.PadByRadius(m_Radius + maxOffsetSize);

  // Try to apply the requested region to the input image
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{  
  // Retrieve the input and output pointers
  InputImagePointerType inputPtr = const_cast<InputImageType *> (this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(outputPtr, outputRegionForThread);

  // Go to begin
  outputIt.GoToBegin();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while (!outputIt.IsAtEnd())
    {
    // Initialise output value
    double out = itk::NumericTraits<double>::max();
    
    // For each offset
    typename OffsetListType::const_iterator offIt;
    for (offIt = m_OffsetList.begin(); offIt != m_OffsetList.end(); ++offIt)
      {
      OffsetType currentOffset = *offIt;

      // Compute the region on which co-occurence will be estimated
      typename InputRegionType::IndexType inputIndex, inputIndexWithTwiceOffset;
      typename InputRegionType::SizeType inputSize, inputSizeWithTwiceOffset;

      // First, apply offset
      for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
        {
        inputIndex[dim] = std::min(
                                  static_cast<int>(outputIt.GetIndex()[dim] - m_Radius[dim]),
                                  static_cast<int>(outputIt.GetIndex()[dim] - m_Radius[dim] + currentOffset[dim])
                                  );
        inputSize[dim] = 2 * m_Radius[dim] + 1 + std::abs(currentOffset[dim]);

        inputIndexWithTwiceOffset[dim] = static_cast<int>(outputIt.GetIndex()[dim] - m_Radius[dim] - std::abs(currentOffset[dim]));
        inputSizeWithTwiceOffset[dim] = inputSize[dim] + std::abs(currentOffset[dim]);
        }

      // Build the input  region
      InputRegionType inputRegion;
      inputRegion.SetIndex(inputIndex);
      inputRegion.SetSize(inputSize);
      inputRegion.Crop(inputPtr->GetRequestedRegion());

      InputRegionType inputRegionWithTwiceOffset;
      inputRegionWithTwiceOffset.SetIndex(inputIndexWithTwiceOffset);
      inputRegionWithTwiceOffset.SetSize(inputSizeWithTwiceOffset);
      inputRegionWithTwiceOffset.Crop(inputPtr->GetRequestedRegion());

      /*********************************************************************************/
      //Local copy of the input image around the processed pixel *outputIt
      InputImagePointerType localInputImage = InputImageType::New();
      localInputImage->SetRegions(inputRegionWithTwiceOffset);
      localInputImage->Allocate();
      typedef itk::ImageRegionIteratorWithIndex<InputImageType> ImageRegionIteratorType;
      ImageRegionIteratorType itInputPtr(inputPtr, inputRegionWithTwiceOffset);
      ImageRegionIteratorType itLocalInputImage(localInputImage, inputRegionWithTwiceOffset);
      for (itInputPtr.GoToBegin(), itLocalInputImage.GoToBegin(); !itInputPtr.IsAtEnd(); ++itInputPtr, ++itLocalInputImage)
        {
        itLocalInputImage.Set(itInputPtr.Get());
        }
      /*********************************************************************************/

      InputImagePointerType maskImage = InputImageType::New();
      maskImage->SetRegions(inputRegionWithTwiceOffset);
      maskImage->Allocate();
      maskImage->FillBuffer(0);

      ImageRegionIteratorType itMask(maskImage, inputRegion);
      for (itMask.GoToBegin(); !itMask.IsAtEnd(); ++itMask)
        {
        itMask.Set(1);
        }


      // Build the co-occurence matrix generator
      CoocurrenceMatrixGeneratorPointerType coOccurenceMatrixGenerator = CoocurrenceMatrixGeneratorType::New();
      coOccurenceMatrixGenerator->SetInput(localInputImage);
      coOccurenceMatrixGenerator->SetOffset(currentOffset);
      coOccurenceMatrixGenerator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis);
      coOccurenceMatrixGenerator->SetPixelValueMinMax(m_InputImageMinimum, m_InputImageMaximum);

      // Compute the co-occurence matrix
      coOccurenceMatrixGenerator->SetMaskImage(maskImage);
      coOccurenceMatrixGenerator->SetInsidePixelValue(1);
      coOccurenceMatrixGenerator->Update();

      typename HistogramType::ConstPointer histo = coOccurenceMatrixGenerator->GetOutput();

      double inertia = 0;
      typename HistogramType::TotalAbsoluteFrequencyType  totalFrequency = histo->GetTotalFrequency();
      typename HistogramType::ConstIterator itr = histo->Begin();
      typename HistogramType::ConstIterator end = histo->End();      
      for(; itr != end; ++itr )
        {
        MeasurementType frequency = itr.GetFrequency();
        if (frequency == 0)
          {
          continue; // no use doing these calculations if we're just multiplying by zero.
          }
        typename HistogramType::IndexType index = histo->GetIndex(itr.GetInstanceIdentifier());
        inertia += (index[0] - index[1]) * (index[0] - index[1]) * frequency / totalFrequency;
        }

      if (inertia < out)
        {
        out = inertia;
        }
      }

    outputIt.Set(out);
    ++outputIt;
    progress.CompletedPixel();
    }
}

} // End namespace otb

#endif
