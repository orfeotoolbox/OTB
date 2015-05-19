/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvertPixelBuffer.h,v $
  Language:  C++
  Date:      $Date: 2009-08-11 12:45:11 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbConvertPixelBuffer_h
#define __otbConvertPixelBuffer_h

#include <complex>
#include "itkObject.h"

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
template <
  typename InputPixelType,
  typename OutputPixelType,
  class OutputConvertTraits
  >
class ConvertPixelBuffer
{
public:
  /** Determine the output data type. */
  typedef typename OutputConvertTraits::ComponentType OutputComponentType;

  /** General method converts from one type to another. */
  static void Convert(InputPixelType* inputData,
                      int inputNumberOfComponents,
                      OutputPixelType* outputData , size_t size);
  static void ConvertVectorImage(InputPixelType* inputData,
                      int inputNumberOfComponents,
                      OutputPixelType* outputData , size_t size);
  static void ConvertComplexVectorImageToVectorImage(std::complex<InputPixelType>* inputData,
                      int inputNumberOfComponents,
                      OutputPixelType* outputData , size_t size);
  static void ConvertComplexVectorImageToVectorImageComplex(std::complex<InputPixelType>* inputData,
                        int inputNumberOfComponents,
                        OutputPixelType* outputData , size_t size);

  static void ConvertComplexToGray(std::complex<InputPixelType>* inputData,
                      int inputNumberOfComponents,
                      OutputPixelType* outputData , size_t size);

protected:
  /** Convertions related to complex */
  static void ConvertGrayToComplex(InputPixelType * inputData,
                                OutputPixelType * OutputData, size_t size);
  
private:
  ConvertPixelBuffer();
  ~ConvertPixelBuffer();
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConvertPixelBuffer.txx"
#endif

#endif // __otbConvertPixelBuffer_h
