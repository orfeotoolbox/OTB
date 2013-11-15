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
#ifndef __otbMultivariateAlterationDetectorImageFilter_txx
#define __otbMultivariateAlterationDetectorImageFilter_txx

#include "otbMultivariateAlterationDetectorImageFilter.h"
#include "otbMath.h"

#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/algo/vnl_generalized_eigensystem.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::MultivariateAlterationDetectorImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_CovarianceEstimator = CovarianceEstimatorType::New();
}

template <class TInputImage, class TOutputImage>
void
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::SetInput1(const TInputImage * image1)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputImage *>(image1));
}

template <class TInputImage, class TOutputImage>
const typename MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::InputImageType *
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::GetInput1()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputImage>
void
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::SetInput2(const TInputImage * image2)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputImage *>(image2));
}

template <class TInputImage, class TOutputImage>
const typename MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::InputImageType *
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::GetInput2()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}


template <class TInputImage, class TOutputImage>
void
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Retrieve input images pointers
  const TInputImage * input1Ptr = this->GetInput1();
  const TInputImage * input2Ptr = this->GetInput2();
  TOutputImage * outputPtr = const_cast<TOutputImage *>(this->GetOutput());

  // Get the number of components for each image
  unsigned int nbComp1 = input1Ptr->GetNumberOfComponentsPerPixel();
  unsigned int nbComp2 = input2Ptr->GetNumberOfComponentsPerPixel();
  unsigned int outNbComp = std::max(nbComp1, nbComp2);

  outputPtr->SetNumberOfComponentsPerPixel(outNbComp);

  if(input1Ptr->GetLargestPossibleRegion() != input2Ptr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Input images does not have the same size!");
    }

  // First concatenate both images
  ConcatenateImageFilterPointer concatenateFilter = ConcatenateImageFilterType::New();
  concatenateFilter->SetInput1(input1Ptr);
  concatenateFilter->SetInput2(input2Ptr);

  // The compute covariance matrix
  // TODO: implement switch off of this computation
  m_CovarianceEstimator->SetInput(concatenateFilter->GetOutput());
  m_CovarianceEstimator->Update();
  m_CovarianceMatrix = m_CovarianceEstimator->GetCovariance();
  m_MeanValues = m_CovarianceEstimator->GetMean();

  // Extract sub-matrices of the covariance matrix
  VnlMatrixType s11 = m_CovarianceMatrix.GetVnlMatrix().extract(nbComp1, nbComp1);
  VnlMatrixType s22 = m_CovarianceMatrix.GetVnlMatrix().extract(nbComp2, nbComp2, nbComp1, nbComp1);
  VnlMatrixType s12 = m_CovarianceMatrix.GetVnlMatrix().extract(nbComp1, nbComp2, 0, nbComp1);
  VnlMatrixType s21 = s12.transpose();

  // Extract means
  m_Mean1 = VnlVectorType(nbComp1, 0);
  m_Mean2 = VnlVectorType(nbComp2, 0);

  for(unsigned int i = 0; i<nbComp1; ++i)
    {
    m_Mean1[i] = m_MeanValues[i];
    }

  for(unsigned int i = 0; i<nbComp2; ++i)
    {
    m_Mean2[i] = m_MeanValues[nbComp1+i];
    }

    if(nbComp1 == nbComp2)
    {
    // Case where nbbands1 == nbbands2

    VnlMatrixType invs22 = vnl_matrix_inverse<RealType>(s22);

    // Build the generalized eigensystem
    VnlMatrixType s12s22is21 = s12 * invs22 *s21;

    vnl_generalized_eigensystem ges(s12s22is21, s11);

    m_V1 = ges.V;

    // Compute cannonical correlation matrix
    m_Rho = ges.D.get_diagonal();
    m_Rho = m_Rho.apply(&vcl_sqrt);

    // We do not need to scale v1 since the
    // vnl_generalized_eigensystem already gives unit variance

    VnlMatrixType invstderr1 = s11.apply(&vcl_sqrt);
    invstderr1 = invstderr1.apply(&InverseValue);
    VnlVectorType diag1 = invstderr1.get_diagonal();
    invstderr1.fill(0);
    invstderr1.set_diagonal(diag1);

    VnlMatrixType sign1 = VnlMatrixType(nbComp1, nbComp1, 0);

    VnlMatrixType aux4 = invstderr1 * s11 * m_V1;

    VnlVectorType aux5 = VnlVectorType(nbComp1, 0);

    for(unsigned int i = 0; i < nbComp1; ++i)
      {
      aux5=aux5 + aux4.get_row(i);
      }

    sign1.set_diagonal(aux5);
    sign1 = sign1.apply(&SignOfValue);

    m_V1 = m_V1 * sign1;

    m_V2 = invs22*s21*m_V1;

    // Scale v2 for unit variance
    VnlMatrixType aux1 = m_V2.transpose() * (s22 * m_V2);
    VnlVectorType aux2 = aux1.get_diagonal();
    aux2 = aux2.apply(&vcl_sqrt);
    aux2 = aux2.apply(&InverseValue);
    VnlMatrixType aux3 = VnlMatrixType(aux2.size(), aux2.size(), 0);
    aux3.fill(0);
    aux3.set_diagonal(aux2);
    m_V2 =  m_V2 * aux3;
    }
  else
    {
    VnlMatrixType sl(nbComp1+nbComp2, nbComp1+nbComp2, 0);
    VnlMatrixType sr(nbComp1+nbComp2, nbComp1+nbComp2, 0);

    sl.update(s12, 0, nbComp1);
    sl.update(s21, nbComp1, 0);
    sr.update(s11, 0, 0);
    sr.update(s22, nbComp1, nbComp1);

    vnl_generalized_eigensystem ges(sl, sr);

    VnlMatrixType V = ges.V;

    V.fliplr();

    m_V1 = V.extract(nbComp1, nbComp1);
    m_V2 = V.extract(nbComp2, nbComp2, nbComp1, 0);

    m_Rho = ges.D.get_diagonal().flip().extract(std::max(nbComp1, nbComp2), 0);

    //Scale v1 to get a unit variance
    VnlMatrixType aux1 = m_V1.transpose() * (s11 * m_V1);

    VnlVectorType aux2 = aux1.get_diagonal();
    aux2 = aux2.apply(&vcl_sqrt);
    aux2 = aux2.apply(&InverseValue);

    VnlMatrixType aux3 = VnlMatrixType(aux2.size(), aux2.size(), 0);
    aux3.set_diagonal(aux2);
    m_V1 = m_V1 * aux3;

    VnlMatrixType invstderr1 = s11.apply(&vcl_sqrt);
    invstderr1 = invstderr1.apply(&InverseValue);
    VnlVectorType diag1 = invstderr1.get_diagonal();
    invstderr1.fill(0);
    invstderr1.set_diagonal(diag1);

    VnlMatrixType sign1 = VnlMatrixType(nbComp1, nbComp1, 0);

    VnlMatrixType aux4 = invstderr1 * s11 * m_V1;

    VnlVectorType aux5 = VnlVectorType(nbComp1, 0);

    for(unsigned int i = 0; i < nbComp1; ++i)
      {
      aux5=aux5 + aux4.get_row(i);
      }

    sign1.set_diagonal(aux5);
    sign1 = sign1.apply(&SignOfValue);

    m_V1 = m_V1 * sign1;

    // Scale v2 for unit variance
    aux1 = m_V2.transpose() * (s22 * m_V2);
    aux2 = aux1.get_diagonal();
    aux2 = aux2.apply(&vcl_sqrt);
    aux2 = aux2.apply(&InverseValue);
    aux3 = VnlMatrixType(aux2.size(), aux2.size(), 0);
    aux3.fill(0);
    aux3.set_diagonal(aux2);
    m_V2 =  m_V2 * aux3;

    VnlMatrixType sign2 = VnlMatrixType(nbComp2, nbComp2, 0);

    aux5 = (m_V1.transpose() * s12 * m_V2).transpose().get_diagonal();
    sign2.set_diagonal(aux5);
    sign2 = sign2.apply(&SignOfValue);
    m_V2 = m_V2 * sign2;
    }
}

template <class TInputImage, class TOutputImage>
void
MultivariateAlterationDetectorImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve input images pointers
  const TInputImage * input1Ptr = this->GetInput1();
  const TInputImage * input2Ptr = this->GetInput2();
  TOutputImage * outputPtr = this->GetOutput();


  typedef itk::ImageRegionConstIterator<InputImageType>  ConstIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> IteratorType;

  IteratorType outIt(outputPtr, outputRegionForThread);
  ConstIteratorType inIt1(input1Ptr, outputRegionForThread);
  ConstIteratorType inIt2(input2Ptr, outputRegionForThread);

  inIt1.GoToBegin();
  inIt2.GoToBegin();
  outIt.GoToBegin();

  // Get the number of components for each image
  unsigned int nbComp1 = input1Ptr->GetNumberOfComponentsPerPixel();
  unsigned int nbComp2 = input2Ptr->GetNumberOfComponentsPerPixel();
  unsigned int outNbComp = outputPtr->GetNumberOfComponentsPerPixel();


   itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while(!inIt1.IsAtEnd() && !inIt2.IsAtEnd() && !outIt.IsAtEnd())
    {
    VnlVectorType x1(nbComp1, 0);
    VnlVectorType x1bis(outNbComp, 0);
    VnlVectorType x2(nbComp2, 0);
    VnlVectorType x2bis(outNbComp, 0);
    VnlVectorType mad(outNbComp, 0);

    for(unsigned int i = 0; i < nbComp1; ++i)
      {
      x1[i] = inIt1.Get()[i];
      }

    for(unsigned int i = 0; i < nbComp2; ++i)
      {
      x2[i] = inIt2.Get()[i];
      }

    VnlVectorType first = (x1-m_Mean1)*m_V1;
    VnlVectorType second = (x2-m_Mean2)*m_V2;

    for(unsigned int i = 0; i < nbComp1; ++i)
      {
      x1bis[i] = first[i];
      }

    for(unsigned int i = 0; i < nbComp2; ++i)
      {
      x2bis[i] = second[i];
      }

    mad = x1bis - x2bis;

    typename OutputImageType::PixelType outPixel(outNbComp);

    if(nbComp1 == nbComp2)
      {
      for(unsigned int i = 0; i<outNbComp; ++i)
        {
        outPixel[i]=mad[i];
        }
      }
    else
      {
      for(unsigned int i = 0; i<outNbComp; ++i)
        {
        outPixel[i]=mad[outNbComp - i - 1];

        if(i < outNbComp - std::min(nbComp1, nbComp2))
          {
          outPixel[i]*=vcl_sqrt(2.);
          }
        }
      }

    outIt.Set(outPixel);

    ++inIt1;
    ++inIt2;
    ++outIt;
    progress.CompletedPixel();
    }
}
}

#endif
