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

#ifndef otbConvertPixelBuffer_h
#define otbConvertPixelBuffer_h

#include <complex>
#include "itkObject.h"
#include "OTBImageBaseExport.h"

namespace otb
{
/**
 * \class ConvertPixelBuffer
 *  \brief Class to convert blocks of data from one type to another.
 *
 * ConvertPixelBuffer uses itk::ConvertPixelBuffer to perform most of the
 * conversions. The monoband to complex conversion is done in this class
 * (different rule than in ITK). New conversions methods are also added in
 * this class :
 *   ConvertComplexVectorImageToVectorImage
 *   ConvertComplexVectorImageToVectorImageComplex
 *   ConvertComplexToGray
 *
 * \ingroup OTBImageBase
 */
template <typename InputPixelType, typename OutputPixelType, class OutputConvertTraits>
class OTBImageBase_EXPORT_TEMPLATE ConvertPixelBuffer
{
public:
  /** Determine the output data type. */
  typedef typename OutputConvertTraits::ComponentType OutputComponentType;

  /** General method converts from one type to another. */
  static void Convert(InputPixelType* inputData, int inputNumberOfComponents, OutputPixelType* outputData, size_t size);
  static void ConvertVectorImage(InputPixelType* inputData, int inputNumberOfComponents, OutputPixelType* outputData, size_t size);
  static void ConvertComplexVectorImageToVectorImage(std::complex<InputPixelType>* inputData, int inputNumberOfComponents, OutputPixelType* outputData,
                                                     size_t size);
  static void ConvertComplexVectorImageToVectorImageComplex(std::complex<InputPixelType>* inputData, int inputNumberOfComponents, OutputPixelType* outputData,
                                                            size_t size);

  static void ConvertComplexToGray(std::complex<InputPixelType>* inputData, int inputNumberOfComponents, OutputPixelType* outputData, size_t size);

protected:
  /** Conversions related to complex */
  static void ConvertGrayToComplex(InputPixelType* inputData, OutputPixelType* OutputData, size_t size);

private:
  ConvertPixelBuffer();
  ~ConvertPixelBuffer();
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConvertPixelBuffer.hxx"
#endif

#endif // otbConvertPixelBuffer_h
