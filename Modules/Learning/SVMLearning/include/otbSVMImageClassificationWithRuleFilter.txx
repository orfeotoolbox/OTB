/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSVMImageClassificationWithRuleFilter_txx
#define __otbSVMImageClassificationWithRuleFilter_txx
#include "otbSVMImageClassificationWithRuleFilter.h"

#include "itkNumericTraits.h"

namespace otb {

template <class TInputImage, class TOutputImage, class TMaskImage>
SVMImageClassificationWithRuleFilter<TInputImage, TOutputImage, TMaskImage>
::SVMImageClassificationWithRuleFilter()
{
  m_OutputRule = OutputRuleImageType::New();
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SVMImageClassificationWithRuleFilter<TInputImage, TOutputImage, TMaskImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if (this->GetModel() == NULL)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }

  // add output information on the rule image
  this->GetOutputRule()->SetNumberOfComponentsPerPixel(
    this->GetModel()->GetNumberOfClasses() * (this->GetModel()->GetNumberOfClasses() - 1) / 2);

  this->GetOutputRule()->CopyInformation(this->GetInput());
  this->GetOutputRule()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SVMImageClassificationWithRuleFilter<TInputImage, TOutputImage, TMaskImage>
::AllocateOutputs()
{
  Superclass::AllocateOutputs();

  if (this->GetModel() == NULL)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }

  // add allocation for the rule image
  OutputRuleImageType * output = this->GetOutputRule();
  output->SetNumberOfComponentsPerPixel(
    this->GetModel()->GetNumberOfClasses() * (this->GetModel()->GetNumberOfClasses() - 1) / 2);
  output->Allocate();
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SVMImageClassificationWithRuleFilter<TInputImage, TOutputImage, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr      = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr     = this->GetOutput();
  OutputRuleImagePointerType outputRulePtr = this->GetOutputRule();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<OutputRuleImageType> OutputRuleIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);
  OutputRuleIteratorType outRuleIt(outputRulePtr, outputRegionForThread);

  // Eventually iterate on masks
  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  bool validPoint = true;
  typename ModelType::DistancesVectorType defaultDistancesVector
    (outputRulePtr->GetNumberOfComponentsPerPixel());
  defaultDistancesVector.Fill(itk::NumericTraits<RuleValueType>::ZeroValue());

  // Walk the part of the images
  for (inIt.GoToBegin(), outIt.GoToBegin(), outRuleIt.GoToBegin();
       !inIt.IsAtEnd() && !outIt.IsAtEnd() && !outRuleIt.IsAtEnd();
       ++inIt, ++outIt, ++outRuleIt)
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
      MeasurementType measure;
      for (unsigned int i = 0; i < inIt.Get().Size(); ++i)
        {
        measure.push_back(inIt.Get()[i]);
        }
      outIt.Set(this->GetModel()->EvaluateLabel(measure));
      // And get rules
      outRuleIt.Set(this->GetModel()->EvaluateHyperplanesDistances(measure));
      }
    else
      {
      // else, set default value
      outIt.Set(this->GetDefaultLabel());
      outRuleIt.Set(defaultDistancesVector);
      }
    progress.CompletedPixel();
    }
}

} // end of namespace otb

#endif
