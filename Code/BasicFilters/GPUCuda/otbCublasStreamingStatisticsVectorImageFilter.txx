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
#ifndef __otbCublasStreamingStatisticsVectorImageFilter_txx
#define __otbCublasStreamingStatisticsVectorImageFilter_txx

#include "otbCublasStreamingStatisticsVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::PersistentCublasStreamingStatisticsVectorImageFilter()
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around vector/matric types

  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
  this->itk::ProcessObject::SetNthOutput(3, this->MakeOutput(3).GetPointer());

  cublasStatus status = cublasInit();
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    printf("cublasInit failed");
    }

  m_GPUImage = 0;
  m_GPUImageSize.Fill(0);
  m_GPUSecondOrderAccumulator = 0;
}

template<class TInputImage>
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::~PersistentCublasStreamingStatisticsVectorImageFilter()
{
  cublasStatus status = cublasShutdown();
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    printf("cublasShutdown failed");
    }
}

template<class TInputImage>
itk::DataObject::Pointer PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::MakeOutput(
                                                                                                       unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*> (TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*> (RealPixelObjectType::New().GetPointer());
      break;
    case 2:
    case 3:
      return static_cast<itk::DataObject*> (MatrixObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*> (TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage>
typename PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::RealPixelObjectType*
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GetMeanOutput()
{
  return static_cast<RealPixelObjectType*> (this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::RealPixelObjectType*
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GetMeanOutput() const
{
  return static_cast<const RealPixelObjectType*> (this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::MatrixObjectType*
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GetCorrelationOutput()
{
  return static_cast<MatrixObjectType*> (this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::MatrixObjectType*
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GetCorrelationOutput() const
{
  return static_cast<const MatrixObjectType*> (this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
typename PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::MatrixObjectType*
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GetCovarianceOutput()
{
  return static_cast<MatrixObjectType*> (this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
const typename PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::MatrixObjectType*
PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GetCovarianceOutput() const
{
  return static_cast<const MatrixObjectType*> (this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
      }
    }
}

template<class TInputImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::Reset()
{
  TInputImage * inputPtr = const_cast<TInputImage *> (this->GetInput());
  inputPtr->UpdateOutputInformation();

  unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  RealPixelType zeroRealPixel;
  zeroRealPixel.SetSize(numberOfComponent);
  zeroRealPixel.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetMeanOutput()->Set(zeroRealPixel);

  MatrixType zeroMatrix;
  zeroMatrix.SetSize(numberOfComponent, numberOfComponent);
  zeroMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetCovarianceOutput()->Set(zeroMatrix);
  this->GetCorrelationOutput()->Set(zeroMatrix);

  m_FirstOrderAccumulator = zeroRealPixel;
  m_SecondOrderAccumulator = zeroMatrix;

  if (m_GPUSecondOrderAccumulator == 0)
    {
    otbMsgDevMacro( << "Allocating " << numberOfComponent << " * " << numberOfComponent << " matrix on GPU (" <<
        numberOfComponent * numberOfComponent * sizeof(float) / 1024 / 1024 << " Mo)" );

    cublasStatus status;
    status = cublasAlloc(numberOfComponent * numberOfComponent, sizeof(float), (void**) &m_GPUSecondOrderAccumulator);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasAlloc m_GPUSecondOrderAccumulator failed" );
      }

    status = cublasSetMatrix(numberOfComponent, numberOfComponent, sizeof(float),
                             m_SecondOrderAccumulator.GetVnlMatrix().data_block(), numberOfComponent,
                             m_GPUSecondOrderAccumulator, numberOfComponent);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasSetMatrix m_GPUSecondOrderAccumulator failed" );
      }

    }

  cublasFree(m_GPUImage);
  cublasFree(m_GPUSecondOrderAccumulator);
}

template<class TInputImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::Synthetize()
{
  TInputImage * inputPtr = const_cast<TInputImage *> (this->GetInput());
  const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();
  const unsigned int nbPixels = inputPtr->GetBufferedRegion().GetNumberOfPixels();

  MatrixType cpuSecondOrderAccumulator(numberOfComponent, numberOfComponent);
  cublasStatus status;
  status = cublasGetMatrix(numberOfComponent, numberOfComponent, sizeof(float), m_GPUSecondOrderAccumulator,
                           numberOfComponent, cpuSecondOrderAccumulator.GetVnlMatrix().data_block(), numberOfComponent);
  // TODO : check status, throw exception on error & clean up GPU memory
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasGetMatrix m_GPUSecondOrderAccumulator failed");
    }

  MatrixType cor = cpuSecondOrderAccumulator / nbPixels;
  this->GetCorrelationOutput()->Set(cor);

}

template<class TInputImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::GenerateData()
{
  // Support progress methods/callbacks
  //  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Grab the input
  InputImagePointer inputPtr = const_cast<TInputImage *> (this->GetInput());
  const unsigned int nbPixels = inputPtr->GetBufferedRegion().GetNumberOfPixels();
  otbMsgDevMacro( "buffered region : " << inputPtr->GetBufferedRegion() << " (" << nbPixels << " pixels)" );
  const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  cublasStatus status;

  if (inputPtr->GetBufferedRegion().GetSize() != m_GPUImageSize)
    {
    otbMsgDevMacro( "Allocating " << nbPixels << " pixels on GPU (" << numberOfComponent * nbPixels * sizeof(float) / 1024
        / 1024 << " Mo)" );


    status = cublasAlloc(numberOfComponent * nbPixels, sizeof(float), (void**) &m_GPUImage);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasAlloc m_GPUImage failed" );
      }

    m_GPUImageSize = inputPtr->GetBufferedRegion().GetSize();

    status = cublasSetMatrix(numberOfComponent, inputPtr->GetBufferedRegion().GetNumberOfPixels(), sizeof(float),
                             inputPtr->GetBufferPointer(), numberOfComponent, m_GPUImage, numberOfComponent);
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasSetMatrix m_GPUImage failed" );
      }

    }

  cublasSsyrk('u', 'n', numberOfComponent, inputPtr->GetBufferedRegion().GetNumberOfPixels(), 1.0f, m_GPUImage,
              numberOfComponent, 1, m_GPUSecondOrderAccumulator, numberOfComponent);

  status = cublasGetError();
  // TODO : check status, throw exception on error & clean up GPU memory
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasSsyrk failed" );
    }

}

template<class TImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  //os << indent << "Mean: "        << this->GetMeanOutput()->Get()        << std::endl;
  //os << indent << "Covariance: "  << this->GetCovarianceOutput()->Get()  << std::endl;
  os << indent << "Correlation: " << this->GetCorrelationOutput()->Get() << std::endl;
}

} // end namespace otb
#endif
