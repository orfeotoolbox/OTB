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
#ifndef __otbOverlapSaveConvolutionImageFilter_txx
#define __otbOverlapSaveConvolutionImageFilter_txx

#include "itkConfigure.h"

#include "otbOverlapSaveConvolutionImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

//debug
#include "itkImageRegionIterator.h"
#include "otbMath.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TBoundaryCondition>
OverlapSaveConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition>
::OverlapSaveConvolutionImageFilter()
{
  m_Radius.Fill(1);
  m_Filter.SetSize(3 * 3);
  m_Filter.Fill(1);
  m_NormalizeFilter = false;
}

template <class TInputImage, class TOutputImage, class TBoundaryCondition>
void
OverlapSaveConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition>
::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
#if defined ITK_USE_FFTWD
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr = const_cast<TInputImage *>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // Pad by filter radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
#else
  itkGenericExceptionMacro(
    <<
    "The OverlapSaveConvolutionImageFilter can not operate without the FFTW library (double implementation). Please install it and set it up in the  cmake configuration.");
#endif
  }

template<class TInputImage, class TOutputImage, class TBoundaryCondition>
void
OverlapSaveConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition>
/* TODO commented out since multi-threading is not supported for the moment
 * ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) */
::GenerateData()
{
#if defined ITK_USE_FFTWD
  // Input/Output pointers
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input = this->GetInput();

  /* TODO: This is a patch to switch from GenerateData() to ThreadedGenerateData(). Remove these two lines
  once multi-threading problem is solved */
  this->AllocateOutputs();
  OutputImageRegionType outputRegionForThread = output->GetRequestedRegion();

  // Size of the filter
  typename InputImageType::SizeType sizeOfFilter;
  sizeOfFilter[0] = 2 * m_Radius[0] + 1;
  sizeOfFilter[1] = 2 * m_Radius[1] + 1;

  // Filter normalization
  InputRealType norm;

  // Compute the input region for the given thread
  OutputImageRegionType inputRegionForThread = outputRegionForThread;
  inputRegionForThread.PadByRadius(m_Radius);

  // Compute the piece region for the input thread. Piece region is different
  // from input region on boundaries
  typename InputImageType::RegionType pieceRegion = inputRegionForThread;
  typename InputImageType::SizeType   pieceSize = pieceRegion.GetSize();
  typename InputImageType::IndexType  pieceIndex = pieceRegion.GetIndex();

  // Compute the size of the FFT and the size of the piece
  unsigned int pieceNbOfPixel = pieceRegion.GetNumberOfPixels();
  unsigned int sizeFFT = (pieceSize[0] / 2 + 1) * pieceSize[1];

  // Achieve the computation of the inputRegionForThread
  inputRegionForThread.Crop(input->GetLargestPossibleRegion());
  typename InputImageType::IndexType inputIndex = inputRegionForThread.GetIndex();
  typename InputImageType::SizeType  inputSize =   inputRegionForThread.GetSize();

  //Iterator of input image
  itk::ConstNeighborhoodIterator<InputImageType, BoundaryConditionType>
  inputIt(m_Radius, input, inputRegionForThread);
  inputIt.GoToBegin();

  //Iterator of output image
  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt;
  outputIt = itk::ImageRegionIteratorWithIndex<OutputImageType>(output, outputRegionForThread);

  //variables for loops
  unsigned int i, j, k, l;

  // ITK proxy to the fftw library
  typedef typename itk::fftw::Proxy<double> FFTWProxyType;

  //memory allocation
  InputPixelType* resampledFilterPiece;
  resampledFilterPiece = static_cast<FFTWProxyType::PixelType*>(fftw_malloc(pieceNbOfPixel * sizeof(InputPixelType)));

  FFTWProxyType::ComplexType* filterPieceFFT;
  filterPieceFFT = static_cast<FFTWProxyType::ComplexType*>(fftw_malloc(sizeFFT * sizeof(FFTWProxyType::ComplexType)));

  InputPixelType *inputPiece;
  inputPiece = static_cast<FFTWProxyType::PixelType*>(fftw_malloc(pieceNbOfPixel * sizeof(InputPixelType)));

  FFTWProxyType::ComplexType* inputPieceFFT;
  inputPieceFFT = static_cast<FFTWProxyType::ComplexType*>(fftw_malloc(sizeFFT * sizeof(FFTWProxyType::ComplexType)));

  // Image piece FFT
  FFTWProxyType::PlanType inputPlan = FFTWProxyType::Plan_dft_r2c_2d(pieceSize[1],
                                                                     pieceSize[0],
                                                                     inputPiece,
                                                                     inputPieceFFT,
                                                                     FFTW_MEASURE);

  // left zero padding
  unsigned int leftskip = static_cast<unsigned int>(std::max(0L, inputIndex[0] - pieceIndex[0]));
  unsigned int topskip =  pieceSize[0] * static_cast<unsigned int>(std::max(0L, inputIndex[1] - pieceIndex[1]));

  // Filling the buffer with image values
  for (l = 0; l < inputSize[1]; ++l)
    {
    for (k = 0; k < inputSize[0]; ++k)
      {
      inputPiece[topskip + pieceSize[0] * l + k + leftskip] = inputIt.GetCenterPixel();
      ++inputIt;
      }
    }

  FFTWProxyType::Execute(inputPlan);

  // Resampled filter FFT
  FFTWProxyType::PlanType filterPlan = FFTWProxyType::Plan_dft_r2c_2d(pieceSize[1],
                                                                      pieceSize[0],
                                                                      resampledFilterPiece,
                                                                      filterPieceFFT,
                                                                      FFTW_MEASURE);

  k = 0;
  // Filling the buffer with filter values
  for (j = 0; j < sizeOfFilter[1]; ++j)
    {
    for (i = 0; i < sizeOfFilter[0]; ++i)
      {
      resampledFilterPiece[i + j * pieceSize[0]] = m_Filter.GetElement(k); //Copy values
      ++k;
      }
    }

  FFTWProxyType::Execute(filterPlan);

  // memory allocation for inverse FFT
  FFTWProxyType::ComplexType* multipliedFFTarray;
  multipliedFFTarray = static_cast<FFTWProxyType::ComplexType*>(fftw_malloc(sizeFFT * sizeof(FFTWProxyType::ComplexType)));

  FFTWProxyType::PixelType* inverseFFTpiece;
  inverseFFTpiece = static_cast<FFTWProxyType::PixelType*>(fftw_malloc(pieceNbOfPixel * sizeof(FFTWProxyType::PixelType)));

  // Inverse FFT of the product of FFT (actually do filtering here)
  FFTWProxyType::PlanType outputPlan = FFTWProxyType::Plan_dft_c2r_2d(pieceSize[1],
                                                                      pieceSize[0],
                                                                      multipliedFFTarray,
                                                                      inverseFFTpiece,
                                                                      FFTW_MEASURE);

  // Filling the buffer with complex product values
  for (k = 0; k < sizeFFT; ++k)
    {
    //complex mutiplication
    multipliedFFTarray[k][0] = inputPieceFFT[k][0] * filterPieceFFT[k][0] - inputPieceFFT[k][1] * filterPieceFFT[k][1];
    multipliedFFTarray[k][1] = inputPieceFFT[k][0] * filterPieceFFT[k][1] + inputPieceFFT[k][1] * filterPieceFFT[k][0];
    }

  FFTWProxyType::Execute(outputPlan);

  // Computing the filter normalization

  if (m_NormalizeFilter)
    {
    norm = itk::NumericTraits<InputRealType>::Zero;
    for (i = 0; i < sizeOfFilter[0] * sizeOfFilter[1]; ++i)
      {
      norm += static_cast<InputRealType>(m_Filter(i));
      }
    if (norm == 0.0)
      {
      norm = 1.0;
      }
    else
      {
      norm = 1 / norm;
      }
    }
  else
    {
    norm = 1.0;
    }

  // Fill the ouptut image
  outputIt.GoToBegin();
  while (!outputIt.IsAtEnd())
    {
    typename InputImageType::IndexType index = outputIt.GetIndex();
    unsigned int                       linearIndex =
      (index[1] + sizeOfFilter[1] - 1 -
       outputRegionForThread.GetIndex()[1]) * pieceSize[0] - 1 + index[0] + sizeOfFilter[0] -
      outputRegionForThread.GetIndex()[0];
    outputIt.Set(static_cast<OutputPixelType>((inverseFFTpiece[linearIndex] /
                                               pieceNbOfPixel) * static_cast<double>(norm)));
    ++outputIt;
    }

  // destroy the FFT plans
  FFTWProxyType::DestroyPlan(inputPlan);
  FFTWProxyType::DestroyPlan(filterPlan);
  FFTWProxyType::DestroyPlan(outputPlan);

  //frees memory
  fftw_free(resampledFilterPiece);
  fftw_free(inputPiece);
  fftw_free(filterPieceFFT);
  fftw_free(inputPieceFFT);
  fftw_free(multipliedFFTarray);
  fftw_free(inverseFFTpiece);
#else
  itkGenericExceptionMacro(
    <<
    "The OverlapSaveConvolutionImageFilter can not operate without the FFTW library (double implementation). Please install it and set it up in the  cmake configuration.");
#endif
}

/** Standard "PrintSelf" method */
template <class TInputImage, class TOutput, class TBoundaryCondition>
void
OverlapSaveConvolutionImageFilter<TInputImage, TOutput, TBoundaryCondition>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "Normalize filter: " << m_NormalizeFilter << std::endl;
}
} // end namespace otb

#endif
