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
#include "itkSubtractImageFilter.h"

#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/algo/vnl_generalized_eigensystem.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

#include "otbImageFileWriter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
MaximumAutocorrelationFactorImageFilter<TInputImage,TOutputImage>
::MaximumAutocorrelationFactorImageFilter()
{
  m_CovarianceEstimator = CovarianceEstimatorType::New();
}

template <class TInputImage, class TOutputImage>
void
MaximumAutocorrelationFactorImageFilter<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Retrieve input images pointers
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  TOutputImage * outputPtr = this->GetOutput();

  // TODO: set the number of output components
  unsigned int nbComp = inputPtr->GetNumberOfComponentsPerPixel();

  // Compute Dh and Dv

  typedef otb::MultiChannelExtractROI<typename InputImageType::InternalPixelType,typename InputImageType::InternalPixelType> ExtractFilterType;
  typedef itk::SubtractImageFilter<InputImageType,InputImageType,InputImageType> DifferenceFilterType;

  InputImageRegionType largestInputRegion = inputPtr->GetLargestPossibleRegion();
  InputImageRegionType referenceRegion;
  InputImageSizeType size = largestInputRegion.GetSize();
  size[0]-=1;
  size[1]-=1;
  referenceRegion.SetSize(size);
  InputImageIndexType index = largestInputRegion.GetIndex();
  referenceRegion.SetIndex(index);
  
  std::cout<<"Reference region: "<<std::endl;
  std::cout<<referenceRegion<<std::endl;

  InputImageRegionType dhRegion;
  InputImageRegionType dvRegion;
  
  index[0]+=1;
  
  dhRegion.SetSize(size);
  dhRegion.SetIndex(index);

  std::cout<<"Dh region:"<<std::endl;
  std::cout<<dhRegion<<std::endl;

  index[0]-=1;
  index[1]+=1;

  dvRegion.SetSize(size);
  dvRegion.SetIndex(index);

  std::cout<<"Dv region:"<<std::endl;
  std::cout<<dvRegion<<std::endl;

  
  typename ExtractFilterType::Pointer referenceExtract = ExtractFilterType::New();
  referenceExtract->SetInput(inputPtr);
  referenceExtract->SetExtractionRegion(referenceRegion);

  typename ExtractFilterType::Pointer dhExtract = ExtractFilterType::New();
  dhExtract->SetInput(inputPtr);
  dhExtract->SetExtractionRegion(dhRegion);

  typename ExtractFilterType::Pointer dvExtract = ExtractFilterType::New();
  dvExtract->SetInput(inputPtr);
  dvExtract->SetExtractionRegion(dvRegion);

  typename DifferenceFilterType::Pointer diffh = DifferenceFilterType::New();
  diffh->SetInput1(referenceExtract->GetOutput());
  diffh->SetInput2(dhExtract->GetOutput());

  typedef otb::ImageFileWriter<InputImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetInput(diffh->GetOutput());
  writer->SetFileName("diffh.tif");
  writer->Update();

  std::cout<<"Diffh ok"<<std::endl;

  typename DifferenceFilterType::Pointer diffv = DifferenceFilterType::New();
  diffv->SetInput1(referenceExtract->GetOutput());
  diffv->SetInput2(dvExtract->GetOutput());

  std::cout<<"Diffv ok"<<std::endl;

  //Compute pooled sigma (using sigmadh and sigmadv)
  m_CovarianceEstimator->SetInput(diffh->GetOutput());
  m_CovarianceEstimator->Update();
  VnlMatrixType sigmadh = m_CovarianceEstimator->GetCovariance().GetVnlMatrix();

  std::cout<<"Sigmadh: "<<std::endl;
  std::cout<<sigmadh<<std::endl;

  m_CovarianceEstimator = CovarianceEstimatorType::New();
  m_CovarianceEstimator->SetInput(diffv->GetOutput());
  m_CovarianceEstimator->Update();
  VnlMatrixType sigmadv = m_CovarianceEstimator->GetCovariance().GetVnlMatrix(
);

  std::cout<<"Sigmadv: "<<std::endl;
  std::cout<<sigmadv<<std::endl;

  // Simple pool
  VnlMatrixType sigmad = 0.5*(sigmadh+sigmadv);

  std::cout<<"Sigmad: "<<std::endl;
  std::cout<<sigmad<<std::endl;

  // Compute the original image covariance
  m_CovarianceEstimator = CovarianceEstimatorType::New();
  m_CovarianceEstimator->SetInput(inputPtr);
  m_CovarianceEstimator->Update();
  VnlMatrixType sigma = m_CovarianceEstimator->GetCovariance().GetVnlMatrix();

  m_Mean = VnlVectorType(nbComp,0);

  for(unsigned int i = 0; i<nbComp;++i)
    {
    m_Mean[i] = m_CovarianceEstimator->GetMean()[i];
    }

  std::cout<<"Sigma: "<<std::endl;
  std::cout<<sigma<<std::endl;

  vnl_generalized_eigensystem ges(sigmad,sigma);
  VnlMatrixType d = ges.D;
  m_V = ges.V;

  std::cout<<"V: "<<std::endl;
  std::cout<<m_V<<std::endl;
  
  std::cout<<"D: "<<std::endl;
  std::cout<<d<<std::endl;

  VnlMatrixType invstderr = VnlMatrixType(nbComp,nbComp,0);
  invstderr.set_diagonal(sigma.get_diagonal());
  invstderr = invstderr.apply(&vcl_sqrt);
  invstderr = invstderr.apply(&InverseValue);

  std::cout<<"invstderr: "<<std::endl;
  std::cout<<invstderr<<std::endl;

  VnlMatrixType invstderrmaf = VnlMatrixType(nbComp,nbComp,0);
  invstderrmaf.set_diagonal((m_V.transpose() * sigma * m_V).get_diagonal());
  invstderrmaf = invstderrmaf.apply(&vcl_sqrt);
  invstderrmaf = invstderrmaf.apply(&InverseValue);

  std::cout<<"invstderrmaf: "<<std::endl;
  std::cout<<invstderrmaf<<std::endl;
  
  VnlMatrixType aux1 = invstderr * sigma * m_V * invstderrmaf;

  std::cout<<"Aux1: "<<std::endl;
  std::cout<<aux1<<std::endl;
  
  VnlMatrixType sign = VnlMatrixType(nbComp,nbComp,0);

  VnlVectorType aux2 = VnlVectorType(nbComp,0);
  
  for(unsigned int i = 0; i < nbComp;++i)
    {
    aux2=aux2 + aux1.get_row(i);
    }

  sign.set_diagonal(aux2);
  sign = sign.apply(&SignOfValue);

  std::cout<<"sign: "<<std::endl;
  std::cout<<sign<<std::endl;

  m_V = m_V * sign;
  
  std::cout<<"V (unit variance): "<<std::endl;
  std::cout<<m_V<<std::endl;
}

template <class TInputImage, class TOutputImage>
void
MaximumAutocorrelationFactorImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
{
  // Retrieve input images pointers
  const TInputImage * inputPtr = this->GetInput();
  TOutputImage * outputPtr = this->GetOutput();


  typedef itk::ImageRegionConstIterator<InputImageType>  ConstIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> IteratorType;

  IteratorType outIt(outputPtr,outputRegionForThread);
  ConstIteratorType inIt(inputPtr, outputRegionForThread);

  inIt.GoToBegin();
  outIt.GoToBegin();

  // Get the number of components for each image
  unsigned int outNbComp = outputPtr->GetNumberOfComponentsPerPixel();


   itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while(!inIt.IsAtEnd() && !outIt.IsAtEnd())
    {
    VnlVectorType x(outNbComp,0);
    VnlVectorType maf(outNbComp,0);
    
    for(unsigned int i = 0; i < outNbComp;++i)
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
