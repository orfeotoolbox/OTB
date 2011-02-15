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
  // just decorators around vector/matrix types

  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
  this->itk::ProcessObject::SetNthOutput(3, this->MakeOutput(3).GetPointer());

  cublasStatus status = cublasInit();
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasInit failed" );
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
    otbMsgDevMacro( "cublasShutdown failed" );
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


  if (m_GPUFirstOrderAccumulator == 0)
    {
    cublasStatus status;
    status = cublasAlloc(numberOfComponent, sizeof(float), (void**) &m_GPUFirstOrderAccumulator);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasAlloc m_GPUFirstOrderAccumulator failed" );
      }

    status = cublasSetVector(numberOfComponent, sizeof(float),
                             m_FirstOrderAccumulator.GetDataPointer(), 1,
                             m_GPUFirstOrderAccumulator, 1);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasSetMatrix m_GPUFirstOrderAccumulator failed" );
      }
    }


  if (m_GPUSecondOrderAccumulator == 0)
    {
    cublasStatus status;
    status = cublasAlloc(numberOfComponent * numberOfComponent, sizeof(float),
                         (void**) &m_GPUSecondOrderAccumulator);

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
}

template<class TInputImage>
void PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage>::Synthetize()
{
  TInputImage * inputPtr = const_cast<TInputImage *> (this->GetInput());
  const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();
  const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();

  RealPixelType cpuFirstOrderAccumulator(numberOfComponent);
  MatrixType cpuSecondOrderAccumulator(numberOfComponent, numberOfComponent);
  cublasStatus status;
  status = cublasGetMatrix(numberOfComponent, numberOfComponent, sizeof(float), m_GPUSecondOrderAccumulator,
                           numberOfComponent, cpuSecondOrderAccumulator.GetVnlMatrix().data_block(), numberOfComponent);

  // TODO : check status, throw exception on error & clean up GPU memory
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasGetMatrix m_GPUSecondOrderAccumulator failed");
    }


  status = cublasGetVector(numberOfComponent, sizeof(float), m_GPUFirstOrderAccumulator, 1,
                           const_cast<float*>(cpuFirstOrderAccumulator.GetDataPointer()), 1);
  // TODO : check status, throw exception on error & clean up GPU memory
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasGetMatrix m_GPUSecondOrderAccumulator failed");
    }

  if (m_GPUImage)
    {
    status = cublasFree(m_GPUImage);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasFree m_GPUImage failed" );
      }
    }

  if (m_GPUFirstOrderAccumulator)
    {
    status = cublasFree(m_GPUFirstOrderAccumulator);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasFree m_GPUFirstOrderAccumulator failed" );
      }
    }

  if (m_GPUOnesVector)
    {
    status = cublasFree(m_GPUOnesVector);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasFree m_GPUOnesVector failed" );
      }
    }

  if (m_GPUSecondOrderAccumulator)
    {
    status = cublasFree(m_GPUSecondOrderAccumulator);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasFree m_GPUSecondOrderAccumulator failed" );
      }
    }

  this->GetMeanOutput()->Set(cpuFirstOrderAccumulator / nbPixels);
  const RealPixelType& mean = this->GetMeanOutput()->Get();

  MatrixType cor = cpuSecondOrderAccumulator / nbPixels;
  for (unsigned int r = 0; r < numberOfComponent; ++r)
    {
    for (unsigned int c = r + 1; c < numberOfComponent; ++c)
      {
      cor(r, c) = cor(c, r);
      }
    }

  this->GetCorrelationOutput()->Set(cor);

  const float regul = static_cast<float>(nbPixels) / (nbPixels - 1);
  MatrixType cov(numberOfComponent, numberOfComponent);
  for (unsigned int r = 0; r < numberOfComponent; ++r)
    {
    for (unsigned int c = 0; c < numberOfComponent; ++c)
      {
      cov(r, c) = regul * (cor(r, c) - mean[r] * mean[c]);
      }
    }

  this->GetCovarianceOutput()->Set(cov);
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
    if (m_GPUImage)
      {
      status = cublasFree(m_GPUImage);

      // TODO : check status, throw exception on error & clean up GPU memory
      if (status != CUBLAS_STATUS_SUCCESS)
        {
        otbMsgDevMacro( "cublasFree m_GPUImage failed" );
        }

      }
    if (m_GPUOnesVector)
      {
      status = cublasFree(m_GPUOnesVector);

      // TODO : check status, throw exception on error & clean up GPU memory
      if (status != CUBLAS_STATUS_SUCCESS)
        {
        otbMsgDevMacro( "cublasFree m_GPUOnesVector failed" );
        }
      }

    otbMsgDevMacro( "Allocating " << nbPixels << " pixels on GPU (" << numberOfComponent * nbPixels * sizeof(float) / 1024
        / 1024 << " Mo)" );

    status = cublasAlloc(numberOfComponent * nbPixels, sizeof(float), (void**) &m_GPUImage);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasAlloc m_GPUImage failed" );
      }

    m_GPUImageSize = inputPtr->GetBufferedRegion().GetSize();


    status = cublasAlloc(nbPixels, sizeof(float), (void**) &m_GPUOnesVector);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasAlloc m_GPUOnesVector failed" );
      }

    RealPixelType onesRealPixel;
    onesRealPixel.SetSize(nbPixels);
    onesRealPixel.Fill(itk::NumericTraits<RealType>::One);

    status = cublasSetVector(nbPixels, sizeof(float),
                             onesRealPixel.GetDataPointer(), 1,
                             m_GPUOnesVector, 1);

    // TODO : check status, throw exception on error & clean up GPU memory
    if (status != CUBLAS_STATUS_SUCCESS)
      {
      otbMsgDevMacro( "cublasSetMatrix m_GPUOnesVector failed" );
      }
    }

  status = cublasSetMatrix(numberOfComponent, nbPixels, sizeof(float),
                           inputPtr->GetBufferPointer(), numberOfComponent, m_GPUImage, numberOfComponent);
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasSetMatrix m_GPUImage failed" );
    }

  cublasSgemv('n', numberOfComponent, nbPixels, 1.0f, m_GPUImage, numberOfComponent, m_GPUOnesVector, 1, 1.0f,
              m_GPUFirstOrderAccumulator, 1);

  status = cublasGetError();
  // TODO : check status, throw exception on error & clean up GPU memory
  if (status != CUBLAS_STATUS_SUCCESS)
    {
    otbMsgDevMacro( "cublasSgemv failed" );
    }


  cublasSsyrk('u', 'n', numberOfComponent, nbPixels, 1.0f, m_GPUImage,
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

  os << indent << "Mean: "        << this->GetMeanOutput()->Get()        << std::endl;
  os << indent << "Covariance: "  << this->GetCovarianceOutput()->Get()  << std::endl;
  os << indent << "Correlation: " << this->GetCorrelationOutput()->Get() << std::endl;
}

} // end namespace otb

#endif
