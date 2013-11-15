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
#ifndef __otbMaximumAutocorrelationFactorImageFilter_txx
#define __otbMaximumAutocorrelationFactorImageFilter_txx

#include "otbMaximumAutocorrelationFactorImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMath.h"
#include "itkSubtractImageFilter.h"

#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/algo/vnl_generalized_eigensystem.h"

#include "itkChangeInformationImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
MaximumAutocorrelationFactorImageFilter<TInputImage, TOutputImage>
::MaximumAutocorrelationFactorImageFilter()
{
  m_CovarianceEstimator = CovarianceEstimatorType::New();
  m_CovarianceEstimatorH = CovarianceEstimatorType::New();
  m_CovarianceEstimatorV = CovarianceEstimatorType::New();
}

template <class TInputImage, class TOutputImage>
void
MaximumAutocorrelationFactorImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Retrieve input images pointers
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  //TOutputImage * outputPtr = this->GetOutput();

  // TODO: set the number of output components
  unsigned int nbComp = inputPtr->GetNumberOfComponentsPerPixel();

  // Compute Dh and Dv
  typedef otb::MultiChannelExtractROI<typename InputImageType::InternalPixelType, RealType> ExtractFilterType;
  typedef itk::SubtractImageFilter<InternalImageType, InternalImageType, InternalImageType>  DifferenceFilterType;
  typedef itk::ChangeInformationImageFilter<InternalImageType> ChangeInformationImageFilter;

  InputImageRegionType largestInputRegion = inputPtr->GetLargestPossibleRegion();
  InputImageRegionType referenceRegion;
  InputImageSizeType size = largestInputRegion.GetSize();
  size[0] -= 1;
  size[1] -= 1;
  referenceRegion.SetSize(size);
  InputImageIndexType index = largestInputRegion.GetIndex();
  referenceRegion.SetIndex(index);

  InputImageRegionType dhRegion;
  InputImageRegionType dvRegion;

  index[0] += 1;

  dhRegion.SetSize(size);
  dhRegion.SetIndex(index);

  index[0] -= 1;
  index[1] += 1;

  dvRegion.SetSize(size);
  dvRegion.SetIndex(index);

  typename ExtractFilterType::Pointer referenceExtract = ExtractFilterType::New();
  referenceExtract->SetInput(inputPtr);
  referenceExtract->SetExtractionRegion(referenceRegion);

  typename ExtractFilterType::Pointer dhExtract = ExtractFilterType::New();
  dhExtract->SetInput(inputPtr);
  dhExtract->SetExtractionRegion(dhRegion);

  typename ChangeInformationImageFilter::Pointer dhExtractShift = ChangeInformationImageFilter::New();
  dhExtractShift->SetInput(dhExtract->GetOutput());
  dhExtractShift->SetReferenceImage(referenceExtract->GetOutput());
  dhExtractShift->SetUseReferenceImage(true);
  dhExtractShift->SetChangeOrigin(true);

  typename ExtractFilterType::Pointer dvExtract = ExtractFilterType::New();
  dvExtract->SetInput(inputPtr);
  dvExtract->SetExtractionRegion(dvRegion);

  typename ChangeInformationImageFilter::Pointer dvExtractShift = ChangeInformationImageFilter::New();
  dvExtractShift->SetInput(dvExtract->GetOutput());
  dvExtractShift->SetReferenceImage(referenceExtract->GetOutput());
  dvExtractShift->SetUseReferenceImage(true);
  dvExtractShift->SetChangeOrigin(true);


  typename DifferenceFilterType::Pointer diffh = DifferenceFilterType::New();
  diffh->SetInput1(referenceExtract->GetOutput());
  diffh->SetInput2(dhExtractShift->GetOutput());

  typename DifferenceFilterType::Pointer diffv = DifferenceFilterType::New();
  diffv->SetInput1(referenceExtract->GetOutput());
  diffv->SetInput2(dvExtractShift->GetOutput());

  //Compute pooled sigma (using sigmadh and sigmadv)
  m_CovarianceEstimatorH->SetInput(diffh->GetOutput());
  m_CovarianceEstimatorH->Update();
  VnlMatrixType sigmadh = m_CovarianceEstimatorH->GetCovariance().GetVnlMatrix();

  m_CovarianceEstimatorV->SetInput(diffv->GetOutput());
  m_CovarianceEstimatorV->Update();
  VnlMatrixType sigmadv = m_CovarianceEstimatorV->GetCovariance().GetVnlMatrix();

  // Simple pool
  VnlMatrixType sigmad = 0.5*(sigmadh+sigmadv);

  // Compute the original image covariance
  referenceExtract->SetExtractionRegion(inputPtr->GetLargestPossibleRegion());
  m_CovarianceEstimator->SetInput(referenceExtract->GetOutput());
  m_CovarianceEstimator->Update();
  VnlMatrixType sigma = m_CovarianceEstimator->GetCovariance().GetVnlMatrix();

  m_Mean = VnlVectorType(nbComp, 0);

  for(unsigned int i = 0; i<nbComp; ++i)
    {
    m_Mean[i] = m_CovarianceEstimator->GetMean()[i];
    }

  vnl_generalized_eigensystem ges(sigmad, sigma);
  VnlMatrixType d = ges.D;
  m_V = ges.V;

  m_AutoCorrelation = VnlVectorType(nbComp, 1.);
  m_AutoCorrelation -= 0.5 *d.get_diagonal();

  VnlMatrixType invstderr = VnlMatrixType(nbComp, nbComp, 0);
  invstderr.set_diagonal(sigma.get_diagonal());
  invstderr = invstderr.apply(&vcl_sqrt);
  invstderr = invstderr.apply(&InverseValue);

  VnlMatrixType invstderrmaf = VnlMatrixType(nbComp, nbComp, 0);
  invstderrmaf.set_diagonal((m_V.transpose() * sigma * m_V).get_diagonal());
  invstderrmaf = invstderrmaf.apply(&vcl_sqrt);
  invstderrmaf = invstderrmaf.apply(&InverseValue);

  VnlMatrixType aux1 = invstderr * sigma * m_V * invstderrmaf;

  VnlMatrixType sign = VnlMatrixType(nbComp, nbComp, 0);

  VnlVectorType aux2 = VnlVectorType(nbComp, 0);

  for(unsigned int i = 0; i < nbComp; ++i)
    {
    aux2=aux2 + aux1.get_row(i);
    }

  sign.set_diagonal(aux2);
  sign = sign.apply(&SignOfValue);

  // There is no need for scaling since vnl_generalized_eigensystem
  // already gives unit variance
  m_V = m_V * sign;
}

template <class TInputImage, class TOutputImage>
void
MaximumAutocorrelationFactorImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve input images pointers
  const TInputImage * inputPtr = this->GetInput();
  TOutputImage * outputPtr = this->GetOutput();


  typedef itk::ImageRegionConstIterator<InputImageType>  ConstIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>      IteratorType;

  IteratorType outIt(outputPtr, outputRegionForThread);
  ConstIteratorType inIt(inputPtr, outputRegionForThread);

  inIt.GoToBegin();
  outIt.GoToBegin();

  // Get the number of components for each image
  unsigned int outNbComp = outputPtr->GetNumberOfComponentsPerPixel();


   itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while(!inIt.IsAtEnd() && !outIt.IsAtEnd())
    {
    VnlVectorType x(outNbComp, 0);
    VnlVectorType maf(outNbComp, 0);

    for(unsigned int i = 0; i < outNbComp; ++i)
      {
      x[i] = inIt.Get()[i];
      }

    maf = (x-m_Mean)*m_V;

    typename OutputImageType::PixelType outPixel(outNbComp);

    for(unsigned int i = 0; i<outNbComp; ++i)
      {
      outPixel[i]=maf[i];
      }

    outIt.Set(outPixel);

    ++inIt;
    ++outIt;
    progress.CompletedPixel();
    }
}
}

#endif
