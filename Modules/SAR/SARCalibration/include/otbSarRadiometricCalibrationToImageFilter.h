/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSarRadiometricCalibrationToImageFilter_h
#define otbSarRadiometricCalibrationToImageFilter_h

#include "otbSarRadiometricCalibrationFunction.h"
#include "otbFunctionToImageFilter.h"

namespace otb
{

/** \class SarRadiometricCalibrationToImageFilter
 * \brief Evaluates the SarRadiometricCalibrationFunction onto a source image
 * The nature of product(TerrSARX, Sentinel1, etc..)
 * are thus detected  automatically from the metadata. The filter then reads necessary
 * parameters required to perform SarCalibration in a generic way.
 *
 * BeforeThreadedGenerateData() instantiate a SarRadiometricCalibrationFunction
 * and pass the values taken from ImageMetadata instance to it. This
 * is where the actual computation of sigma (backscatter) occurs.
 *
 * Noise, Antenna pattern gain (old && new), range spread loss, incidence angle
 * data members used in this class are all instances of SarPrametricFunction
 * class. Each have a Evaluate() method and a special
 * EvaluateParametricCoefficient() which computes the actual value.
 *
 * \see \c otb::SarParametricFunction
 * \see \c otb::SarCalibrationLookupBase
 * References (Retrieved on 08-Sept-2015)
 * Sentinel1 - https://sentinel.esa.int/web/sentinel/sentinel-1-sar-wiki/-/wiki/Sentinel%20One/Application+of+Radiometric+Calibration+LUT
 * Radarsat2 - http://gs.mdacorporation.com/products/sensor/radarsat2/RS2_Product_Description.pdf
 *
 * \ingroup ImageFilters
 *
 * \ingroup OTBSARCalibration
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT SarRadiometricCalibrationToImageFilter
    : public FunctionToImageFilter<TInputImage, TOutputImage, SarRadiometricCalibrationFunction<TInputImage>>
{
public:
  /** Standard class typedefs. */
  typedef SarRadiometricCalibrationToImageFilter Self;
  typedef FunctionToImageFilter<TInputImage, TOutputImage, SarRadiometricCalibrationFunction<TInputImage>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarRadiometricCalibrationToImageFilter, FunctionToImageFilter);

  /** Some typedefs. */
  /** Image size typedef. */
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointer     InputImagePointer;
  typedef typename Superclass::InputImageRegionType  InputImageRegionType;
  typedef typename Superclass::InputImagePixelType   InputImagePixelType;
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;
  /** Type of function. */

  typedef typename Superclass::FunctionType                     FunctionType;
  typedef typename Superclass::FunctionPointer                  FunctionPointer;
  typedef typename Superclass::FunctionValueType                FunctionValueType;
  typedef typename Superclass::FunctionPositionType             FunctionPositionType;
  typedef typename FunctionType::ParametricFunctionPointer      ParametricFunctionPointer;
  typedef typename FunctionType::ParametricFunctionConstPointer ParametricFunctionConstPointer;
  typedef typename FunctionType::ParametricFunctionType         ParametricFunctionType;


  /** Enable/disable the noise flag in SarRadiometricCalibrationFunction */
  void SetEnableNoise(bool inArg)
  {
    this->GetFunction()->SetEnableNoise(inArg);
  }

  itkSetMacro(LookupSelected, short);
  itkGetConstMacro(LookupSelected, short);

protected:
  /** Default ctor */
  SarRadiometricCalibrationToImageFilter();

  /** Empty, default virtual dtor */
  ~SarRadiometricCalibrationToImageFilter() override
  {
  }

  /** Generate output information */
  void GenerateOutputInformation() override;

  /** Update the function list and input parameters*/
  void BeforeThreadedGenerateData() override;

private:
  SarRadiometricCalibrationToImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;


  short m_LookupSelected;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarRadiometricCalibrationToImageFilter.hxx"
#endif

#endif
