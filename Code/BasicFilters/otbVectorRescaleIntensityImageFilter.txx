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
#include "itkSampleToHistogramFilter.h"
#include "itkDenseFrequencyContainer2.h"
#include "itkHistogram.h"
#include "itkListSample.h"
#include "otbObjectList.h"
#include "otbMacro.h"
#include "itkImageRegionConstIterator.h"

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
  m_Gamma = 1.0;
  m_AutomaticInputMinMaxComputation = true;
}

/**
 * Generate output information.
 */
template <class TInputImage, class TOutputImage>
void
VectorRescaleIntensityImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  this->Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(
    this->GetInput()->GetNumberOfComponentsPerPixel());
}
/**
 * Generate input requested region.
 */
template <class TInputImage, class TOutputImage>
void
VectorRescaleIntensityImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion(void)
{
  if (this->GetInput())
    {
    typename TInputImage::Pointer    input = const_cast<TInputImage *>(this->GetInput());
    typename TInputImage::RegionType inputRegion;
    this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
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
  if (m_ClampThreshold < 0.)
    {
    itkExceptionMacro(<< "Invalid Clamp Threshold must be greater than 0.0");
    }

  if (m_AutomaticInputMinMaxComputation)
    {

    typedef  typename Superclass::InputImageType    InputImageType;
    typedef  typename Superclass::InputImagePointer InputImagePointer;

    // Get the input
    InputImagePointer inputImage =   this->GetInput();

    typedef itk::ImageRegionConstIterator<InputImageType>              InputIterator;
    typedef itk::Vector<typename InputImageType::InternalPixelType, 1> MeasurementVectorType;
    typedef itk::Statistics::ListSample<MeasurementVectorType>         ListSampleType;
    typedef float                                                      HistogramMeasurementType;
    typedef itk::Statistics::Histogram< HistogramMeasurementType,
                           itk::Statistics::DenseFrequencyContainer2 > HistogramType;
    typedef itk::Statistics::SampleToHistogramFilter<ListSampleType, HistogramType> HistogramGeneratorType;

    typedef ObjectList<ListSampleType> ListSampleListType;

    m_InputMinimum.SetSize(inputImage->GetNumberOfComponentsPerPixel());
    m_InputMaximum.SetSize(inputImage->GetNumberOfComponentsPerPixel());
    typename ListSampleListType::Pointer sl =  ListSampleListType::New();

    sl->Reserve(inputImage->GetNumberOfComponentsPerPixel());

    for (unsigned int i = 0; i < m_InputMaximum.GetSize(); ++i)
      {
      sl->PushBack(ListSampleType::New());
      }

    InputIterator it(inputImage, inputImage->GetBufferedRegion());

    it.GoToBegin();

    while (!it.IsAtEnd())
      {
      InputPixelType pixel = it.Get();
      for (unsigned int i = 0; i < m_InputMaximum.GetSize(); ++i)
        {
        sl->GetNthElement(i)->PushBack(pixel[i]);
        }
      ++it;
      }

    for (unsigned int i = 0; i < m_InputMaximum.GetSize(); ++i)
      {
      typename HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
      generator->SetInput(sl->GetNthElement(i));
      typename HistogramGeneratorType::HistogramType::SizeType size;

      // Initialize the size of the SizeType size
      size.SetSize(sl->GetNthElement(i)->GetMeasurementVectorSize());

      if (m_ClampThreshold > 0.)
        size.Fill(static_cast<unsigned int>(vcl_ceil(1 / m_ClampThreshold) * 10));
      else
        size.Fill(256);

      generator->SetHistogramSize(size);

      generator->Update();
      m_InputMinimum[i] =
        static_cast<typename InputImageType::InternalPixelType>(generator->GetOutput()->Quantile(0, m_ClampThreshold));
      m_InputMaximum[i] =
        static_cast<typename InputImageType::InternalPixelType>(generator->GetOutput()->Quantile(0, 1 -
                                                                                                 m_ClampThreshold));
      }
    }

  // set up the functor values
  this->GetFunctor().SetInputMinimum(m_InputMinimum);
  this->GetFunctor().SetInputMaximum(m_InputMaximum);
  this->GetFunctor().SetOutputMaximum(m_OutputMaximum);
  this->GetFunctor().SetOutputMinimum(m_OutputMinimum);
  this->GetFunctor().SetGamma(m_Gamma);
}

/**
 * Printself method
 */
template <class TInputImage, class TOutputImage>
void
VectorRescaleIntensityImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Automatic min/max computation: " << m_AutomaticInputMinMaxComputation << std::endl;
  os << indent << "Clamp threshold: " << m_ClampThreshold << std::endl;
}
} // end namespace otb
#endif
