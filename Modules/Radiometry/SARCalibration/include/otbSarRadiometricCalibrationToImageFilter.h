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
#ifndef otbSarRadiometricCalibrationToImageFilter_h
#define otbSarRadiometricCalibrationToImageFilter_h

#include "otbSarRadiometricCalibrationFunction.h"
#include "otbFunctionToImageFilter.h"

namespace otb
{

/** \class SarRadiometricCalibrationToImageFilter
 * \brief Evaluates the SarRadiometricCalibrationFunction onto a source image
 * The BeforeThreadedGenerateData create a SarImageMetadataInterface based on
 * input metadata dictionary. The nature of product(TerrSARX, Sentinel1, etc..)
 * are thus detected  automatically from this. The filter then reads necessary
 * parameters required to perform SarCalibration in a generic way.
 *
 * BeforeThreadedGenerateData() instantiate a SarRadiometricCalibrationFunction
 * and pass the values taken from SarImageMetadataInterface instance to it. This
 * is where the actual computation of sigma (backscatter) occurs.
 *
 * Noise, Antenna pattern gain (old && new), range spread loss, incidence angle
 * data members used in this class are all instances of SarPrametricFunction
 * class. Each have a Evaluate() method and a special
 * EvaluateParametricCoefficient() which computes the actual value.
 *
 * The technical details and more discussion of SarCalibration can be found in jira
 * story #863.
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
  ~SarRadiometricCalibrationToImageFilter() ITK_OVERRIDE {}

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE;

  /** Update the function list and input parameters*/
  void BeforeThreadedGenerateData() ITK_OVERRIDE;

private:

  SarRadiometricCalibrationToImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented


  short m_LookupSelected;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarRadiometricCalibrationToImageFilter.txx"
#endif

#endif
