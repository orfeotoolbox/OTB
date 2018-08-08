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
#ifndef otbImageDimensionalityReductionFilter_hxx
#define otbImageDimensionalityReductionFilter_hxx

#include "otbImageDimensionalityReductionFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::ImageDimensionalityReductionFilter()
{
  OTB_DISABLE_DYNAMIC_MT;
  this->SetNumberOfIndexedInputs(2);
  this->SetNumberOfRequiredInputs(1);

  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TOutputImage::New());
  this->SetNthOutput(1,ConfidenceImageType::New());
  m_UseConfidenceMap = false;
  m_BatchMode = true;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::MaskImageType *
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return nullptr;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
typename ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::ConfidenceImageType *
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::GetOutputConfidence()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return nullptr;
    }
  return static_cast<ConfidenceImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::BeforeThreadedGenerateData()
{
  if(m_BatchMode)
    {
    #ifdef _OPENMP
    // OpenMP will take care of threading
    this->SetNumberOfThreads(1);
    #endif
    }
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::ClassicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  // Walk the part of the image
  for (inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd() && !outIt.IsAtEnd(); ++inIt, ++outIt)
    {
    // Classifify
    outIt.Set(m_Model->Predict(inIt.Get()));
    progress.CompletedPixel();
    }
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (!m_Model)
    {
    itkGenericExceptionMacro(<< "No model for dimensionality reduction");
    }
  this->GetOutput()->SetNumberOfComponentsPerPixel( m_Model->GetDimension() );
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::BatchThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  typedef typename ModelType::InputSampleType      InputSampleType;
  typedef typename ModelType::InputListSampleType  InputListSampleType;
  typedef typename ModelType::TargetValueType      TargetValueType;
  typedef typename ModelType::TargetListSampleType TargetListSampleType;

  typename InputListSampleType::Pointer samples = InputListSampleType::New();
  unsigned int num_features = inputPtr->GetNumberOfComponentsPerPixel();
  samples->SetMeasurementVectorSize(num_features);
  InputSampleType sample(num_features);

  // Fill the samples
  for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
    {
    typename InputImageType::PixelType pix = inIt.Get();
    for(size_t feat=0; feat<num_features; ++feat)
      {
      sample[feat]=pix[feat];
      }
    samples->PushBack(sample);
    }
  //Make the batch prediction
  typename TargetListSampleType::Pointer labels;

  // This call is threadsafe
  labels = m_Model->PredictBatch(samples);

  // Set the output values
  typename TargetListSampleType::ConstIterator labIt = labels->Begin();
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    itk::VariableLengthVector<TargetValueType> labelValue;
    labelValue = labIt.GetMeasurementVector();
    ++labIt;    
    outIt.Set(labelValue);
    progress.CompletedPixel();
    }
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  if(m_BatchMode)
    {
    this->BatchThreadedGenerateData(outputRegionForThread, threadId);
    }
  else
    {
    this->ClassicThreadedGenerateData(outputRegionForThread, threadId);
    }
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageDimensionalityReductionFilter<TInputImage, TOutputImage, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
