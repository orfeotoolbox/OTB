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
#ifndef __otbSarRadiometricCalibrationFunction_h
#define __otbSarRadiometricCalibrationFunction_h

#include "otbSarRadiometricCalibrationFunctor.h"
#include "otbSarParametricMapFunction.h"
#include "otbSarCalibrationLookupData.h"

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
  typedef otb::Functor::SarRadiometricCalibrationFunctor<RealType, RealType>           FunctorType;
  typedef typename FunctorType::RealType                        FunctorRealType;

  typedef otb::SarParametricMapFunction<InputImageType>               ParametricFunctionType;
  typedef typename ParametricFunctionType::Pointer                    ParametricFunctionPointer;
  typedef typename ParametricFunctionType::ConstPointer               ParametricFunctionConstPointer;

  /** Evalulate the function at specified index */
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

  /** Evaluate the function at non-integer positions */
  virtual OutputType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }

  virtual OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Set the input image.
   * \warning this method caches BufferedRegion information.
   * If the BufferedRegion has changed, user must call
   * SetInputImage again to update cached values. */
  virtual void SetInputImage( const InputImageType * ptr );


  /** Get/Set the Scale value */
  itkSetMacro(Scale, FunctorRealType);
  itkGetMacro(Scale, FunctorRealType);

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
  itkSetMacro(RescalingFactor, FunctorRealType);

  /** Get/Set flag to indicate if these are used */
  itkSetMacro(ApplyAntennaPatternGain, bool);
  itkGetMacro(ApplyAntennaPatternGain, bool);

  itkSetMacro(ApplyIncidenceAngleCorrection, bool);
  itkGetMacro(ApplyIncidenceAngleCorrection, bool);

  itkSetMacro(ApplyRangeSpreadingLossCorrection, bool);
  itkGetMacro(ApplyRangeSpreadingLossCorrection, bool);

  itkSetMacro(ApplyLookupDataCorrection, bool);
  itkGetMacro(ApplyLookupDataCorrection, bool);

  itkSetMacro(ApplyRescalingFactor, bool);
  itkGetMacro(ApplyRescalingFactor, bool);

  /** Set SetCalibrationLookupData instance */
  void SetCalibrationLookupData(SarCalibrationLookupData* lut)
  {
    m_Lut = lut;
  }

protected:

  /** ctor */
  SarRadiometricCalibrationFunction();

  /** default, empty, virtual dtor */
  virtual ~SarRadiometricCalibrationFunction(){}

  /** print method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Flags to indiciate if these values needs to be applied in calibration*/
  bool m_ApplyAntennaPatternGain;
  bool m_ApplyIncidenceAngleCorrection;
  bool m_ApplyRangeSpreadingLossCorrection;
  bool m_ApplyLookupDataCorrection;
  bool m_ApplyRescalingFactor;

private:
  SarRadiometricCalibrationFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  FunctorRealType             m_Scale;
  FunctorRealType             m_RescalingFactor;
  ParametricFunctionPointer   m_Noise;
  bool                        m_EnableNoise;
  ParametricFunctionPointer   m_AntennaPatternNewGain;
  ParametricFunctionPointer   m_AntennaPatternOldGain;
  ParametricFunctionPointer   m_IncidenceAngle;
  ParametricFunctionPointer   m_RangeSpreadLoss;
  SarCalibrationLookupData*   m_Lut;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbSarRadiometricCalibrationFunction.txx"
#endif

#endif
