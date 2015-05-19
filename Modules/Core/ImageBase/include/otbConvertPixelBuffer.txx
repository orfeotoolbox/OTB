/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvertPixelBuffer.txx,v $
  Language:  C++
  Date:      $Date: 2009-11-22 19:35:57 $
  Version:   $Revision: 1.27 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbConvertPixelBuffer_txx
#define __otbConvertPixelBuffer_txx
#include "otbConvertPixelBuffer.h"

#include <cstddef> // ptrdiff_t
#include "itkRGBPixel.h"

namespace otb
{

/* To be able to convert transparently*/

template<typename InputType, typename OutputType>
OutputType
ComplexCast(const std::complex<InputType>& in, const OutputType& itkNotUsed(dummy))
{
  typedef typename itk::NumericTraits<std::complex<InputType> >::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<InputType> >::ScalarRealType ScalarRealType;

  RealType    inReal( static_cast<ScalarRealType>(in.real()), static_cast<ScalarRealType>(in.imag()) );

  return static_cast < OutputType >( vcl_abs(inReal) );
}

template<typename InputType, typename OutputType>
std::complex<OutputType>
ComplexCast(const std::complex<InputType>& in, const std::complex<OutputType>& itkNotUsed(dummy))
{
  typedef typename itk::NumericTraits<std::complex<InputType> >::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<InputType> >::ScalarRealType ScalarRealType;

  RealType    inReal( static_cast<ScalarRealType>(in.real()), static_cast<ScalarRealType>(in.imag()) );

  return static_cast < std::complex<OutputType> >( inReal );
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexToGray(std::complex<InputPixelType>* inputData,
                      int itkNotUsed(inputNumberOfComponents),
                      OutputPixelType* outputData , size_t size)
{
  OutputPixelType dummy;
  std::complex<InputPixelType>* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData,
      static_cast<OutputPixelType> (ComplexCast(*inputData,dummy)));
    inputData++;
    outputData++;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexVectorImageToVectorImage(std::complex<InputPixelType>* inputData,
                     int inputNumberOfComponents,
                     OutputPixelType* outputData , size_t size)
{
  size_t length = size* (size_t)inputNumberOfComponents;
  for( size_t i=0; i< length/2; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData, (*inputData).real());
    ++outputData;
    OutputConvertTraits::SetNthComponent( 0, *outputData, (*inputData).imag());
    ++outputData;
    ++inputData;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexVectorImageToVectorImageComplex(std::complex<InputPixelType>* inputData,
                     int inputNumberOfComponents,
                     OutputPixelType* outputData , size_t size)
{
  size_t length = size* (size_t)inputNumberOfComponents;
  OutputPixelType dummy;
  for( size_t i=0; i< length; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData,
                                          ComplexCast(*inputData, dummy));
    ++outputData;
    ++inputData;
    }
}

}// end namespace otb

#endif
