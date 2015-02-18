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
#ifndef __otbSarRadiometricCalibrationToImageFilter_h
#define __otbSarRadiometricCalibrationToImageFilter_h

#include "otbSarRadiometricCalibrationFunction.h"
#include "otbFunctionToImageFilter.h"

namespace otb
{

/** \class SarRadiometricCalibrationToImageFilter
  * \brief Evaluates the SarRadiometricCalibrationFunction onto a source image
 *
 * The function has to inherit from itkImageFunction
 *
 * \ingroup ImageFilters
 *
 * \ingroup OTBSARCalibration
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT SarRadiometricCalibrationToImageFilter :
  public FunctionToImageFilter<TInputImage, TOutputImage,
                 SarRadiometricCalibrationFunction<TInputImage> >
{
public:
  /** Standard class typedefs. */
  typedef SarRadiometricCalibrationToImageFilter                      Self;
  typedef FunctionToImageFilter<TInputImage, TOutputImage,
                     SarRadiometricCalibrationFunction<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarRadiometricCalibrationToImageFilter, FunctionToImageFilter);

  /** Some typedefs. */
  /** Image size typedef. */
  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::InputImagePointer        InputImagePointer;
  typedef typename Superclass::InputImageRegionType     InputImageRegionType;
  typedef typename Superclass::InputImagePixelType      InputImagePixelType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImagePointer       OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType    OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType     OutputImagePixelType;
  /** Type of function. */

  typedef typename Superclass::FunctionType                     FunctionType;
  typedef typename Superclass::FunctionPointer                  FunctionPointer;
  typedef typename Superclass::FunctionValueType                FunctionValueType;
  typedef typename Superclass::FunctionPositionType             FunctionPositionType;
  typedef typename FunctionType::ParametricFunctionPointer      ParametricFunctionPointer;
  typedef typename FunctionType::ParametricFunctionConstPointer ParametricFunctionConstPointer;
  typedef typename FunctionType::ParametricFunctionType         ParametricFunctionType;

  void SetEnableNoise(bool inArg)
  {
    this->GetFunction()->SetEnableNoise(inArg);
  }
protected:
  SarRadiometricCalibrationToImageFilter();
  virtual ~SarRadiometricCalibrationToImageFilter() {}

  /** Update the function list and input parameters*/
  virtual void BeforeThreadedGenerateData();
private:
  SarRadiometricCalibrationToImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarRadiometricCalibrationToImageFilter.txx"
#endif

#endif
