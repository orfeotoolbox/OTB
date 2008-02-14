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
#ifndef _otbSVMImageClassificationFilter_txx
#define _otbSVMImageClassificationFilter_txx

#include "otbSVMImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension>
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension>
::SVMImageClassificationFilter()
{
  for(unsigned int i = 0;i<static_cast<unsigned int>(this->GetNumberOfThreads());++i)
    {
      m_ClassifierVector.push_back(ClassifierType::New());
    }
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension>
::BeforeThreadedGenerateData()
{
  if(!m_Model)
    {
      itkGenericExceptionMacro(<<"No model for classification");
    }
  for(unsigned int i = 0;i<static_cast<unsigned int>(this->GetNumberOfThreads());++i)
    {
      m_ClassifierVector[i]->SetModel(m_Model);
      m_ClassifierVector[i]->SetNumberOfClasses(m_Model->GetNumberOfClasses());
    }
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension>
::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId)
{
  InputImageConstPointerType inputPtr = this->GetInput();
  OutputImagePointerType     outputPtr = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  
  ListSamplePointerType listSample = ListSampleType::New();
  
  InputIteratorType inIt(inputPtr,outputRegionForThread);
    
  unsigned int sampleSize = std::min(inputPtr->GetNumberOfComponentsPerPixel(),
				     VMaxSampleDimension);
  for(inIt.GoToBegin();!inIt.IsAtEnd();++inIt)
    {
      MeasurementVectorType sample;
      sample.Fill(itk::NumericTraits<ValueType>::ZeroValue());
      for(unsigned int i=0;i<sampleSize;i++)
	{
	  sample[i]=inIt.Get()[i];
	}
      listSample->PushBack(sample);
    }
  
  m_ClassifierVector[threadId]->SetSample(listSample);
  m_ClassifierVector[threadId]->Update();

  typename ClassifierType::OutputType::Pointer membershipSample = m_ClassifierVector[threadId]->GetOutput() ;
  typename ClassifierType::OutputType::ConstIterator sampleIter = membershipSample->Begin() ;
  typename ClassifierType::OutputType::ConstIterator sampleLast = membershipSample->End() ;

  OutputIteratorType outIt(outputPtr,outputRegionForThread);

  outIt.GoToBegin();

  while(!outIt.IsAtEnd()&&(sampleIter!=sampleLast))
  {
    outIt.Set(sampleIter.GetClassLabel());
    ++outIt;
    ++sampleIter;
  }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension>
void
SVMImageClassificationFilter<TInputImage,TOutputImage,VMaxSampleDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
