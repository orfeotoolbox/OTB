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
#ifndef __otbSVMImageClassificationFilter_txx
#define __otbSVMImageClassificationFilter_txx

#include "otbSVMImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::SVMImageClassificationFilter()
{
  this->SetNumberOfInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1,const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
const typename SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::MaskImageType *
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs()<2)
  {
    return 0;
  }
  return  static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::BeforeThreadedGenerateData()
{
  if (!m_Model)
  {
    itkGenericExceptionMacro(<<"No model for classification");
  }
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId)
{
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType> MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;

  ListSamplePointerType listSample = ListSampleType::New();

  InputIteratorType inIt(inputPtr,outputRegionForThread);

  MaskIteratorType maskIt;
  if (inputMaskPtr)
  {
    maskIt = MaskIteratorType(inputMaskPtr,outputRegionForThread);
    maskIt.GoToBegin();
  }

  unsigned int sampleSize = std::min(inputPtr->GetNumberOfComponentsPerPixel(),
                                     VMaxSampleDimension);

  bool validPoint = true;

  for (inIt.GoToBegin();!inIt.IsAtEnd();++inIt)
  {
    if (inputMaskPtr)
    {
      validPoint = maskIt.Get()>0;
      ++maskIt;
    }
    if (validPoint)
    {
      MeasurementVectorType sample;
      sample.Fill(itk::NumericTraits<ValueType>::ZeroValue());
      for (unsigned int i=0;i<sampleSize;i++)
      {
        sample[i]=inIt.Get()[i];
      }
      listSample->PushBack(sample);
    }
  }
  ClassifierPointerType classifier =ClassifierType::New();
  classifier->SetModel(m_Model);
  classifier->SetNumberOfClasses(m_Model->GetNumberOfClasses());
  classifier->SetSample(listSample);
  classifier->Update();

  typename ClassifierType::OutputType::Pointer membershipSample = classifier->GetOutput();
  typename ClassifierType::OutputType::ConstIterator sampleIter = membershipSample->Begin();
  typename ClassifierType::OutputType::ConstIterator sampleLast = membershipSample->End();

  OutputIteratorType outIt(outputPtr,outputRegionForThread);

  outIt.GoToBegin();

  while (!outIt.IsAtEnd()&&(sampleIter!=sampleLast))
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

  while (!outIt.IsAtEnd()&&(sampleIter!=sampleLast))
  {
    if (inputMaskPtr)
    {
      validPoint = maskIt.Get()>0;
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
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension,TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
