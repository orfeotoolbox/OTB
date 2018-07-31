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

#ifndef otbImageClassificationFilter_hxx
#define otbImageClassificationFilter_hxx

#include "otbImageClassificationFilter.h"
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ImageClassificationFilter()
{
  this->SetNumberOfIndexedInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();

  OTB_DISABLE_DYNAMIC_MT
  this->SetNumberOfRequiredOutputs(3);
  
  this->SetNthOutput(0,TOutputImage::New());
  this->SetNthOutput(1,ConfidenceImageType::New());
  this->SetNthOutput(2,ProbaImageType::New());
  m_UseConfidenceMap = false;
  m_UseProbaMap = false;
  m_BatchMode = true;
  m_NumberOfClasses = 1;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::MaskImageType *
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return nullptr;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
typename ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ConfidenceImageType *
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetOutputConfidence()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return nullptr;
    }
  return static_cast<ConfidenceImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
typename ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ProbaImageType *
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetOutputProba()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return nullptr;
    }
  return static_cast<ProbaImageType *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::BeforeThreadedGenerateData()
{
  if (!m_Model)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }
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
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ClassicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              itk::ThreadIdType threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();
  ProbaImagePointerType      probaPtr      = this->GetOutputProba();
  // Progress reporting
  itk::ProgressReporter progress(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<ConfidenceImageType> ConfidenceMapIteratorType;
  typedef itk::ImageRegionIterator<ProbaImageType>      ProbaMapIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  // Eventually iterate on masks
  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  // setup iterator for confidence map
  bool computeConfidenceMap(m_UseConfidenceMap && m_Model->HasConfidenceIndex() &&
                            !m_Model->GetRegressionMode());
  ConfidenceMapIteratorType confidenceIt;
  if (computeConfidenceMap)
    {
    confidenceIt = ConfidenceMapIteratorType(confidencePtr,outputRegionForThread);
    confidenceIt.GoToBegin();
    }

  // setup iterator for proba map
  bool computeProbaMap(m_UseProbaMap &&  m_Model->HasProbaIndex() &&
                       !m_Model->GetRegressionMode());

  ProbaMapIteratorType probaIt;

  if(computeProbaMap)
    {
      probaIt = ProbaMapIteratorType(probaPtr,outputRegionForThread);
      probaIt.GoToBegin();
    }

  bool validPoint = true;
  double confidenceIndex = 0.0;
  ProbaSampleType probaVector{m_NumberOfClasses};
  probaVector.Fill(0);
  // Walk the part of the image
  for (inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd() && !outIt.IsAtEnd();
       ++inIt, ++outIt)
    {
    // Check pixel validity
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    // If point is valid
    if (validPoint)
    {
      // Classifify
      if (computeProbaMap)
      {
        outIt.Set(m_Model->Predict(inIt.Get(),&confidenceIndex,
                                   &probaVector)[0]);
      }
      else if (computeConfidenceMap)
      {
        outIt.Set(m_Model->Predict(inIt.Get(),&confidenceIndex)[0]);
      }
      else
      {
        outIt.Set(m_Model->Predict(inIt.Get())[0]);
      }
    }
    else
      {
      // else, set default value
      outIt.Set(m_DefaultLabel);
      confidenceIndex = 0.0;
      }
    if (computeConfidenceMap)
      {
      confidenceIt.Set(confidenceIndex);
      ++confidenceIt;
      }
    if (computeProbaMap)
    {
      probaIt.Set(probaVector);
      ++probaIt;
    }
    progress.CompletedPixel();
    }

}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::BatchThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId)
{
  bool computeConfidenceMap(m_UseConfidenceMap && m_Model->HasConfidenceIndex()
                            && !m_Model->GetRegressionMode());

  bool computeProbaMap(m_UseProbaMap && m_Model->HasProbaIndex()
                       && !m_Model->GetRegressionMode());
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();
  ProbaImagePointerType      probaPtr      = this->GetOutputProba();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<ConfidenceImageType> ConfidenceMapIteratorType;
  typedef itk::ImageRegionIterator<ProbaImageType>      ProbaMapIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  typedef typename ModelType::InputSampleType      InputSampleType;
  typedef typename ModelType::InputListSampleType  InputListSampleType;
  typedef typename ModelType::TargetValueType      TargetValueType;
  typedef typename ModelType::TargetListSampleType TargetListSampleType;
  typedef typename ModelType::ConfidenceListSampleType ConfidenceListSampleType;
  typedef typename ModelType::ProbaListSampleType ProbaListSampleType;
  typename InputListSampleType::Pointer samples = InputListSampleType::New();
  unsigned int num_features = inputPtr->GetNumberOfComponentsPerPixel();
  samples->SetMeasurementVectorSize(num_features);
  InputSampleType sample(num_features);
  // Fill the samples
  bool validPoint = true;
  for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
    {
    // Check pixel validity
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if(validPoint)
      {
      typename InputImageType::PixelType pix = inIt.Get();
      for(size_t feat=0; feat<num_features; ++feat)
        {
        sample[feat]=pix[feat];
        }
      samples->PushBack(sample);
      }
    }
  //Make the batch prediction
  typename TargetListSampleType::Pointer labels;
  typename ConfidenceListSampleType::Pointer confidences;
  typename ProbaListSampleType::Pointer probas;
  if(computeConfidenceMap)
    confidences = ConfidenceListSampleType::New();

  if(computeProbaMap)
    probas = ProbaListSampleType::New();
  // This call is threadsafe
  labels = m_Model->PredictBatch(samples,confidences,probas);

  // Set the output values
  ConfidenceMapIteratorType confidenceIt;
  if (computeConfidenceMap)
    {
    confidenceIt = ConfidenceMapIteratorType(confidencePtr,outputRegionForThread);
    confidenceIt.GoToBegin();
    }

  ProbaMapIteratorType probaIt;
  if (computeProbaMap)
    {
      probaIt = ProbaMapIteratorType(probaPtr,outputRegionForThread);
      probaIt.GoToBegin();
    }
  typename TargetListSampleType::ConstIterator labIt = labels->Begin();
  maskIt.GoToBegin();
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    double confidenceIndex = 0.0;
    TargetValueType labelValue(m_DefaultLabel);
    ProbaSampleType probaValues{m_NumberOfClasses};
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint && labIt!=labels->End())
      {
      labelValue = labIt.GetMeasurementVector()[0];

       if(computeConfidenceMap)
        {
        confidenceIndex =
          confidences->GetMeasurementVector(labIt.GetInstanceIdentifier())[0];
        }
       if(computeProbaMap)
       {
         //The probas may have different size than the m_NumberOfClasses set by the user
         auto tempProbaValues = probas->GetMeasurementVector(labIt.GetInstanceIdentifier());
         for(unsigned int i=0; i<m_NumberOfClasses; ++i)
         {
           if(i<tempProbaValues.Size()) probaValues[i] = tempProbaValues[i];
           else probaValues[i] = 0;
         }
       }
      ++labIt;
      }
    else
      {
      labelValue = m_DefaultLabel;
      }

    outIt.Set(labelValue);

    if(computeConfidenceMap)
      {
      confidenceIt.Set(confidenceIndex);
      ++confidenceIt;
      }
    if(computeProbaMap)
    {
      probaIt.Set(probaValues);
      ++probaIt;
    }
    progress.CompletedPixel();
    }
}
template <class TInputImage, class TOutputImage, class TMaskImage>
void
ImageClassificationFilter<TInputImage, TOutputImage,
                          TMaskImage>::ThreadedGenerateData(
                            const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId)
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
ImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
