/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbConvertPixelBuffer_hxx
#define otbConvertPixelBuffer_hxx
#include "otbConvertPixelBuffer.h"

#include "itkConvertPixelBuffer.h"

namespace otb
{
template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
void ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::Convert(InputPixelType* inputData, int inputNumberOfComponents,
                                                                                       OutputPixelType* outputData, size_t size)
{
  if ((OutputConvertTraits::GetNumberOfComponents() == 2) && inputNumberOfComponents == 1)
  {
    // OTB patch : monoband to complex
    ConvertGrayToComplex(inputData, outputData, size);
  }
  else
  {
    // use ITK pixel buffer converter
    itk::ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::Convert(inputData, inputNumberOfComponents, outputData, size);
  }
}

// Convert Grayscale to Complex
template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
void ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::ConvertGrayToComplex(InputPixelType* inputData, OutputPixelType* outputData,
                                                                                                    size_t size)
{
  InputPixelType* endInput = inputData + size;
  while (inputData != endInput)
  {
    OutputConvertTraits::SetNthComponent(0, *outputData, static_cast<OutputComponentType>(*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, static_cast<OutputComponentType>(0.));
    inputData++;
    outputData++;
  }
}

/* To be able to convert transparently*/

template <typename InputType, typename OutputType>
OutputType ComplexCast(const std::complex<InputType>& in, const OutputType& itkNotUsed(dummy))
{
  typedef typename itk::NumericTraits<std::complex<InputType>>::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<InputType>>::ScalarRealType ScalarRealType;

  RealType inReal(static_cast<ScalarRealType>(in.real()), static_cast<ScalarRealType>(in.imag()));

  return static_cast<OutputType>(std::abs(inReal));
}

template <typename InputType, typename OutputType>
std::complex<OutputType> ComplexCast(const std::complex<InputType>& in, const std::complex<OutputType>& itkNotUsed(dummy))
{
  typedef typename itk::NumericTraits<std::complex<InputType>>::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<InputType>>::ScalarRealType ScalarRealType;

  RealType inReal(static_cast<ScalarRealType>(in.real()), static_cast<ScalarRealType>(in.imag()));

  return static_cast<std::complex<OutputType>>(inReal);
}

template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
void ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::ConvertComplexToGray(std::complex<InputPixelType>* inputData,
                                                                                                    int              itkNotUsed(inputNumberOfComponents),
                                                                                                    OutputPixelType* outputData, size_t size)
{
  OutputPixelType               dummy;
  std::complex<InputPixelType>* endInput = inputData + size;
  while (inputData != endInput)
  {
    OutputConvertTraits::SetNthComponent(0, *outputData, static_cast<OutputPixelType>(ComplexCast(*inputData, dummy)));
    inputData++;
    outputData++;
  }
}

template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
void ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::ConvertVectorImage(InputPixelType* inputData, int inputNumberOfComponents,
                                                                                                  OutputPixelType* outputData, size_t size)
{
  itk::ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::ConvertVectorImage(inputData, inputNumberOfComponents, outputData, size);
}

template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
void ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::ConvertComplexVectorImageToVectorImage(std::complex<InputPixelType>* inputData,
                                                                                                                      int              inputNumberOfComponents,
                                                                                                                      OutputPixelType* outputData, size_t size)
{
  size_t length = size * (size_t)inputNumberOfComponents;
  for (size_t i = 0; i < length / 2; i++)
  {
    OutputConvertTraits::SetNthComponent(0, *outputData, (*inputData).real());
    ++outputData;
    OutputConvertTraits::SetNthComponent(0, *outputData, (*inputData).imag());
    ++outputData;
    ++inputData;
  }
}

template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
void ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>::ConvertComplexVectorImageToVectorImageComplex(
    std::complex<InputPixelType>* inputData, int inputNumberOfComponents, OutputPixelType* outputData, size_t size)
{
  size_t          length = size * (size_t)inputNumberOfComponents;
  OutputPixelType dummy;
  for (size_t i = 0; i < length; i++)
  {
    OutputConvertTraits::SetNthComponent(0, *outputData, ComplexCast(*inputData, dummy));
    ++outputData;
    ++inputData;
  }
}

} // end namespace otb

#endif
