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
#ifndef __otbSOMImageClassificationFilter_txx
#define __otbSOMImageClassificationFilter_txx

#include "otbSOMImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::SOMImageClassificationFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
const typename SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::MaskImageType *
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::BeforeThreadedGenerateData()
{
  if (!m_Map)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  ListSamplePointerType listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(inputPtr->GetNumberOfComponentsPerPixel());

  InputIteratorType inIt(inputPtr, outputRegionForThread);

  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }
  unsigned int maxDimension = m_Map->GetNumberOfComponentsPerPixel();
  unsigned int sampleSize = std::min(inputPtr->GetNumberOfComponentsPerPixel(),
                                     maxDimension);
  bool validPoint = true;

  for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
    {
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint)
      {
      SampleType sample;
      sample.SetSize(sampleSize);
      sample.Fill(itk::NumericTraits<ValueType>::ZeroValue());
      for (unsigned int i = 0; i < sampleSize; ++i)
        {
        sample[i] = inIt.Get()[i];
        }
      listSample->PushBack(sample);
      }
    }
  ClassifierPointerType classifier = ClassifierType::New();
  classifier->SetMap(m_Map);
  classifier->SetSample(listSample);
  classifier->Update();

  typename ClassifierType::OutputType::Pointer       membershipSample = classifier->GetOutput();
  typename ClassifierType::OutputType::ConstIterator sampleIter = membershipSample->Begin();
  typename ClassifierType::OutputType::ConstIterator sampleLast = membershipSample->End();

  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  outIt.GoToBegin();

  while (!outIt.IsAtEnd() && (sampleIter != sampleLast))
    {
    outIt.Set(m_DefaultLabel);
    ++outIt;
    }

  outIt.GoToBegin();

  if (inputMaskPtr)
    {
    maskIt.GoToBegin();
    }
  validPoint = true;

  while (!outIt.IsAtEnd() && (sampleIter != sampleLast))
    {
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint)
      {
      outIt.Set(sampleIter.GetClassLabel());
      ++sampleIter;
      }
    ++outIt;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
