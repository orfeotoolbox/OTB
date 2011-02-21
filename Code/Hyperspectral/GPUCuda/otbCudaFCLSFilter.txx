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
     the implied warranty of MERCHANT2ABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbCudaFCLSFilter_txx
#define __otbCudaFCLSFilter_txx

#include "otbCudaFCLSFilter.h"

extern "C" void fclsInit();

extern "C" void fclsMallocEndMembers(
                            float** d_endmembers,
                            float** d_endmembersT,
                            float** d_endmembersInv,
                            int numBands,
                            int nbEndmembers);

extern "C" void fclsMallocImage( float** d_image_vector,
                            float** d_image_unmixed,
                            int imageWidth,
                            int imageHeight,
                            int numBands,
                            int nbEndmembers);

extern "C" void fclsProcessing(float* d_image_vector,
                               float* d_image_unmixed,
                               float* d_endmembers,
                               float* d_endmembersT,
                               float* d_endmembersInv,
                               int numSamples,
                               int numBands,
                               int nbEndmembers,
                               int maxIter,
                               int blockSize);

extern "C" void fclsCopyHostToDevice( float* d_ptr,
                                      const float* h_ptr,
                                      int nb_bytes);

extern "C" void fclsCopyDeviceToHost( float* h_ptr,
                                      const float* d_ptr,
                                      int nb_bytes);

extern "C" void fclsFree( float* d_ptr);

namespace otb
{

template <class TInputPointSet, class TOutputImage>
CudaFCLSFilter<TInputPointSet, TOutputImage>
::CudaFCLSFilter()
{
  fclsInit();
  m_BlockSize = 512;
  m_GPUU = 0;
  m_GPUUt = 0;
  m_GPUUinv = 0;
  m_GPUInputImage = 0;
  m_GPUOutputImage = 0;
  m_GPUImageSize.Fill(0);
  this->SetNumberOfThreads(1);
}

template<class TInputPointSet, class TOutputImage>
void
CudaFCLSFilter<TInputPointSet, TOutputImage>
::ThreadedGenerateData(
    const   OutputImageRegionType&     outputRegionForThread,
    int   threadId)
{
  //std::cout << "Region : " << outputRegionForThread << std::endl;
  typename InputImageType::Pointer inputPtr = dynamic_cast<InputImageType *> (this->itk::ProcessObject::GetInput(0));
  typename OutputImageType::Pointer outputPtr = dynamic_cast<OutputImageType *> (this->itk::ProcessObject::GetOutput(0));

  int numBands = inputPtr->GetNumberOfComponentsPerPixel();
  int numEndmembers = outputPtr->GetNumberOfComponentsPerPixel();

  int imageWidth = outputPtr->GetBufferedRegion().GetSize()[0];
  int imageHeight = outputPtr->GetBufferedRegion().GetSize()[1];

  float * pix = inputPtr->GetBufferPointer();
  float * unmixed = outputPtr->GetBufferPointer();

  if( !m_GPUU )
    {
    fclsMallocEndMembers(&m_GPUU, &m_GPUUt, &m_GPUUinv, numBands, numEndmembers);
    fclsCopyHostToDevice(m_GPUU, this->GetFunctor().GetMatrix().data_block(), numBands * numEndmembers * sizeof(float));
    fclsCopyHostToDevice(m_GPUUt, this->GetFunctor().GetMatrixT().data_block(), numBands * numEndmembers * sizeof(float));
    fclsCopyHostToDevice(m_GPUUinv, this->GetFunctor().GetMatrixInv().data_block(), numBands * numEndmembers * sizeof(float));
    }

  if (m_GPUImageSize != inputPtr->GetBufferedRegion().GetSize())
    {
    if(m_GPUInputImage)
      {
      fclsFree(m_GPUInputImage);
      fclsFree(m_GPUOutputImage);
      }

    fclsMallocImage(&m_GPUInputImage, &m_GPUOutputImage, imageWidth, imageHeight, numBands, numEndmembers);
    m_GPUImageSize = inputPtr->GetBufferedRegion().GetSize();
    }

  fclsCopyHostToDevice(m_GPUInputImage, pix, numBands * imageWidth * imageHeight * sizeof(float));

  fclsProcessing(m_GPUInputImage,
                 m_GPUOutputImage,
                 m_GPUU,
                 m_GPUUt,
                 m_GPUUinv,
                 imageWidth * imageHeight,
                 numBands,
                 numEndmembers,
                 m_MaxIter,
                 m_BlockSize);

  fclsCopyDeviceToHost(unmixed, m_GPUOutputImage, numEndmembers * imageWidth * imageHeight * sizeof(float));
}

}

#endif
