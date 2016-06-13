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
#ifndef otbSarRadiometricCalibrationFunction_h
#define otbSarRadiometricCalibrationFunction_h

#include "otbSarParametricMapFunction.h"
#include "otbSarCalibrationLookupData.h"
#include "otbMath.h"
namespace otb
{
/**
 * \class SarRadiometricCalibrationFunction
 * \brief Calculate the backscatter for the given pixel
 *
 * Calculate the backscatter for the given pixel
 *
 * If called with a ContinuousIndex or Point, the calculation is performed
 * at the nearest neighbor.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double ).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBSARCalibration
 */

template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT SarRadiometricCalibrationFunction :
  public itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::AbsType,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef SarRadiometricCalibrationFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::AbsType,
      TCoordRep>                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarRadiometricCalibrationFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Datatype used for the evaluation */
  typedef double                                                 RealType;
//  typedef otb::Functor::SarRadiometricCalibrationFunctor<RealType, RealType>           FunctorType;
//  typedef typename FunctorType::RealType                        FunctorRealType;

  typedef otb::SarParametricMapFunction<InputImageType>               ParametricFunctionType;
  typedef typename ParametricFunctionType::Pointer                    ParametricFunctionPointer;
  typedef typename ParametricFunctionType::ConstPointer               ParametricFunctionConstPointer;

  /** Evalulate the function at specified index */
  OutputType EvaluateAtIndex(const IndexType& index) const ITK_OVERRIDE;

  /** Evaluate the function at non-integer positions */
  OutputType Evaluate(const PointType& point) const ITK_OVERRIDE
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }

  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const ITK_OVERRIDE
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Set the input image.
   * \warning this method caches BufferedRegion information.
   * If the BufferedRegion has changed, user must call
   * SetInputImage again to update cached values. */
  void SetInputImage( const InputImageType * ptr ) ITK_OVERRIDE;


  /** Get/Set the Scale value */
  itkSetMacro(Scale, RealType);
  itkGetMacro(Scale, RealType);

  /** Get/Set the Noise value */
  itkSetObjectMacro(Noise, ParametricFunctionType);
  itkGetConstObjectMacro(Noise, ParametricFunctionType);
  itkGetObjectMacro(Noise, ParametricFunctionType);

  /** Get/Set the flag which indicate if we use the Offset value */
  itkSetMacro(EnableNoise, bool);
  itkGetMacro(EnableNoise, bool);

  /** Get/Set the AntennaPatternNewGain value */
  itkSetObjectMacro(AntennaPatternNewGain, ParametricFunctionType);
  itkGetConstObjectMacro(AntennaPatternNewGain, ParametricFunctionType);
  itkGetObjectMacro(AntennaPatternNewGain, ParametricFunctionType);

  /** Get/Set the AntennaPatternOldGain value */
  itkSetObjectMacro(AntennaPatternOldGain, ParametricFunctionType);
  itkGetObjectMacro(AntennaPatternOldGain, ParametricFunctionType);
  itkGetConstObjectMacro(AntennaPatternOldGain, ParametricFunctionType);

  /** Get/Set the IncidenceAngle value */
  itkSetObjectMacro(IncidenceAngle, ParametricFunctionType);
  itkGetConstObjectMacro(IncidenceAngle, ParametricFunctionType);
  itkGetObjectMacro(IncidenceAngle, ParametricFunctionType);

  /** Get/Set the RangeSpreadLoss value */
  itkSetObjectMacro(RangeSpreadLoss, ParametricFunctionType);
  itkGetConstObjectMacro(RangeSpreadLoss, ParametricFunctionType);
  itkGetObjectMacro(RangeSpreadLoss, ParametricFunctionType);

  /** Set the RescalingFactor value */
  itkSetMacro(RescalingFactor, RealType);

  /** Get/Set flag to indicate if these are used */
  itkSetMacro(ApplyAntennaPatternGain, bool);
  itkGetMacro(ApplyAntennaPatternGain, bool);

  itkSetMacro(ApplyIncidenceAngleCorrection, bool);
  itkGetMacro(ApplyIncidenceAngleCorrection, bool);

  itkSetMacro(ApplyRangeSpreadLossCorrection, bool);
  itkGetMacro(ApplyRangeSpreadLossCorrection, bool);

  itkSetMacro(ApplyLookupDataCorrection, bool);
  itkGetMacro(ApplyLookupDataCorrection, bool);

  itkSetMacro(ApplyRescalingFactor, bool);
  itkGetMacro(ApplyRescalingFactor, bool);

    typedef SarCalibrationLookupData::Pointer LookupDataPointer;

  /** Set SetCalibrationLookupData instance */
  void SetCalibrationLookupData(LookupDataPointer lut)
  {
    m_Lut = lut;
  }

protected:

  /** ctor */
  SarRadiometricCalibrationFunction();

  /** default, empty, virtual dtor */
  ~SarRadiometricCalibrationFunction() ITK_OVERRIDE{}

  /** print method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Flags to indiciate if these values needs to be applied in calibration*/

private:
  SarRadiometricCalibrationFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  RealType             m_Scale;
  bool                        m_EnableNoise;
  RealType             m_RescalingFactor;
  bool                        m_ApplyAntennaPatternGain;
  bool                        m_ApplyIncidenceAngleCorrection;
  bool                        m_ApplyRangeSpreadLossCorrection;
  bool                        m_ApplyLookupDataCorrection;
  bool                        m_ApplyRescalingFactor;
  ParametricFunctionPointer   m_Noise;
  ParametricFunctionPointer   m_AntennaPatternNewGain;
  ParametricFunctionPointer   m_AntennaPatternOldGain;
  ParametricFunctionPointer   m_IncidenceAngle;
  ParametricFunctionPointer   m_RangeSpreadLoss;
  LookupDataPointer   m_Lut;


};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbSarRadiometricCalibrationFunction.txx"
#endif

#endif
