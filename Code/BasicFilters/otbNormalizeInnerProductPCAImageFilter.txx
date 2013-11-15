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
#ifndef __otbNormalizeInnerProductPCAImageFilter_txx
#define __otbNormalizeInnerProductPCAImageFilter_txx

#include "otbNormalizeInnerProductPCAImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
NormalizeInnerProductPCAImageFilter<TInputImage, TOutputImage>
::NormalizeInnerProductPCAImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->InPlaceOff();

  m_UseUnbiasedEstimator = true;
}

/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
NormalizeInnerProductPCAImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
}

/**
 * BeforeThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage>
void
NormalizeInnerProductPCAImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  StreamingStatisticsVectorImageFilterPointer stats = StreamingStatisticsVectorImageFilterType::New();
  stats->SetInput(const_cast<InputImageType*>(this->GetInput()));
  //set the normalization method to compute covariance to the StreamingStatisticsVectorImage filter
  stats->SetUseUnbiasedEstimator(m_UseUnbiasedEstimator);

  stats->Update();

  RealPixelType means = stats->GetMean();
  MatrixType    cov = stats->GetCovariance();
  double        NbPixels = static_cast<double>(
    this->GetInput()->GetLargestPossibleRegion().GetSize()[0] *
    this->GetInput()->GetLargestPossibleRegion().GetSize()[1]);
  if ((cov.Rows() != means.Size()) || (cov.Cols() != means.Size()))
    {
    itkExceptionMacro(<< "Covariance matrix with size (" << cov.Rows() << "," <<
                      cov.Cols() << ") is incompatible with mean vector with size" << means.Size());
    }
  m_CoefNorm.SetSize(means.Size());
  for (unsigned int i = 0; i < m_CoefNorm.Size(); ++i)
    {
    m_CoefNorm[i] = (1. / vcl_sqrt(NbPixels * (cov[i][i] + means[i] * means[i])));
    }
}
/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage>
void
NormalizeInnerProductPCAImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer inputPtr  = this->GetInput();

  // Define the iterators
  itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>      outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  // Null pixel construction
  InputPixelType nullPixel;
  nullPixel.SetSize(inputPtr->GetNumberOfComponentsPerPixel());
  nullPixel.Fill(itk::NumericTraits<OutputInternalPixelType>::Zero);
  while (!inputIt.IsAtEnd())
    {
    InputPixelType  inPixel = inputIt.Get();
    OutputPixelType outPixel;
    outPixel.SetSize(inputPtr->GetNumberOfComponentsPerPixel());
    outPixel.Fill(itk::NumericTraits<OutputInternalPixelType>::Zero);
    //outPixel = m_Means * inPixel;
    for (unsigned int j = 0; j < inputPtr->GetNumberOfComponentsPerPixel(); ++j)
      {
      outPixel[j] = static_cast<OutputInternalPixelType>(m_CoefNorm[j] * static_cast<double>(inPixel[j]));
      }

    outputIt.Set(outPixel);
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}

template <class TInputImage, class TOutputImage>
void
NormalizeInnerProductPCAImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
