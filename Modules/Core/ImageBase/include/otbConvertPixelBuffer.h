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

#include "itkConvertPixelBuffer.h"
#include <complex>

namespace otb
{
/**
 * \class ConvertPixelBuffer
 *  \brief Class to convert blocks of data from one type to another.
 *
 * ConvertPixelBuffer extends the static methods in itk::ConvertPixelBuffer.
 * It adds several methods for complex pixels conversions :
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
class ConvertPixelBuffer : public itk::ConvertPixelBuffer<InputPixelType,OutputPixelType,OutputConvertTraits>
{
public:
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

private:
  ConvertPixelBuffer();
  ~ConvertPixelBuffer();
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConvertPixelBuffer.txx"
#endif

#endif // __otbConvertPixelBuffer_h
