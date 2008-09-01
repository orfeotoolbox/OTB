/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorRescaleIntensityImageFilter_txx
#define __otbVectorRescaleIntensityImageFilter_txx

#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkListSampleToHistogramGenerator.h"
#include "itkListSample.h"
#include "otbObjectList.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
VectorRescaleIntensityImageFilter<TInputImage, TOutputImage>
::VectorRescaleIntensityImageFilter()
{
  m_ClampThreshold = 0.01;
}

/**
 * Generate output information.
 */
template <class TInputImage, class TOutputImage>
void
VectorRescaleIntensityImageFilter<TInputImage,TOutputImage>
::GenerateOutputInformation(void)
{
  this->Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(
      this->GetInput()->GetNumberOfComponentsPerPixel() );
}
/**
 * Generate input requested region.
 */
template <class TInputImage, class TOutputImage>
void 
VectorRescaleIntensityImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion(void)
      {
	if(this->GetInput())
	  {
	    typename TInputImage::Pointer input = const_cast<TInputImage *>(this->GetInput());
	    typename TInputImage::RegionType inputRegion;
	    this->CallCopyOutputRegionToInputRegion(inputRegion,this->GetOutput()->GetRequestedRegion());
	    input->SetRequestedRegion(inputRegion);
	  }
      }
/**
 * Process to execute before entering the multithreaded section.
 */
template <class TInputImage, class TOutputImage>
void 
VectorRescaleIntensityImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  typedef  typename Superclass::InputImageType      InputImageType;
  typedef  typename Superclass::InputImagePointer   InputImagePointer;

  // Get the input
  InputImagePointer inputImage =   this->GetInput();

  typedef itk::ImageRegionConstIterator< InputImageType >  InputIterator;
  typedef itk::Vector<typename InputImageType::ValueType,1> MeasurementVectorType;
  typedef itk::Statistics::ListSample<MeasurementVectorType> ListSampleType;
  typedef float HistogramMeasurementType;
  typedef itk::Statistics::ListSampleToHistogramGenerator<ListSampleType,HistogramMeasurementType,
    itk::Statistics::DenseFrequencyContainer,1> HistogramGeneratorType;

  typedef otb::ObjectList<ListSampleType> ListSampleListType;


    InputPixelType maximum;
  InputPixelType minimum;
  minimum.SetSize(inputImage->GetNumberOfComponentsPerPixel());
  maximum.SetSize(inputImage->GetNumberOfComponentsPerPixel());
  typename ListSampleListType::Pointer sl =  ListSampleListType::New();
  
  sl->Reserve(inputImage->GetNumberOfComponentsPerPixel());

  for(unsigned int i = 0;i<maximum.GetSize();++i)
    {
      sl->PushBack(ListSampleType::New());
    }

  


  InputIterator it( inputImage, inputImage->GetBufferedRegion() );

  it.GoToBegin();
  
  while( !it.IsAtEnd() )
    {
       InputPixelType pixel = it.Get();
      for(unsigned int i = 0;i<maximum.GetSize();++i)
	{
	  sl->GetNthElement(i)->PushBack(pixel[i]);
	}
      ++it;
    }

  for(unsigned int i = 0;i<maximum.GetSize();++i)
    {
      typename HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
      generator->SetListSample(sl->GetNthElement(i));
      typename HistogramGeneratorType::HistogramType::SizeType size;
      size.Fill(static_cast<unsigned int>(vcl_ceil(1/m_ClampThreshold)*10));
      generator->SetNumberOfBins(size);
      generator->Update();
      minimum[i]=static_cast<typename InputImageType::ValueType>(generator->GetOutput()->Quantile(0,m_ClampThreshold));
      maximum[i]=static_cast<typename InputImageType::ValueType>(generator->GetOutput()->Quantile(0,1-m_ClampThreshold));
    }

  // set up the functor values
  this->GetFunctor().SetInputMinimum( minimum );
  this->GetFunctor().SetInputMaximum ( maximum );
  this->GetFunctor().SetOutputMaximum(m_OutputMaximum );
  this->GetFunctor().SetOutputMinimum(m_OutputMinimum );
}

/**
 * Printself method
 */
template <class TInputImage, class TOutputImage>
void 
VectorRescaleIntensityImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
} // end namespace otb
#endif
