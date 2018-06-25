/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSarBrightnessFunction_h
#define otbSarBrightnessFunction_h

#include "otbSarBrightnessFunctor.h"
#include "otbSarParametricMapFunction.h"


namespace otb
{

/**
 * \class SarBrightnessFunction
 * \brief Calculate the backscatter for the given pixel
 *
 * Calculate the brigthness value for the given pixel
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
class ITK_EXPORT SarBrightnessFunction :
  public itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::AbsType,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef SarBrightnessFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::AbsType,
      TCoordRep>                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarBrightnessFunction, itk::ImageFunction);

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
  typedef otb::Functor::SarBrightnessFunctor<RealType, RealType>  FunctorType;
  typedef typename FunctorType::RealType                         FunctorRealType;

  typedef otb::SarParametricMapFunction<InputImageType>               ParametricFunctionType;
  typedef typename ParametricFunctionType::Pointer                    ParametricFunctionPointer;
  typedef typename ParametricFunctionType::ConstPointer               ParametricFunctionConstPointer;

  /** Evalulate the function at specified index */
  OutputType EvaluateAtIndex(const IndexType& index) const override;

  /** Evaluate the function at non-integer positions */
  OutputType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Set the input image.
   * \warning this method caches BufferedRegion information.
   * If the BufferedRegion has changed, user must call
   * SetInputImage again to update cached values. */
  void SetInputImage( const InputImageType * ptr ) override;


  /** Get/Set the Scale value */
  itkSetMacro(Scale, FunctorRealType);
  itkGetMacro(Scale, FunctorRealType);

  /** Get/Set the Offset value */
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

  /** Get/Set the RangeSpreadLoss value */
  itkSetObjectMacro(RangeSpreadLoss, ParametricFunctionType);
  itkGetConstObjectMacro(RangeSpreadLoss, ParametricFunctionType);
  itkGetObjectMacro(RangeSpreadLoss, ParametricFunctionType);


protected:
  SarBrightnessFunction();
  ~SarBrightnessFunction() override{}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SarBrightnessFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  FunctorRealType             m_Scale;
  ParametricFunctionPointer   m_Noise;
  bool                        m_EnableNoise;
  ParametricFunctionPointer   m_AntennaPatternNewGain;
  ParametricFunctionPointer   m_AntennaPatternOldGain;
  ParametricFunctionPointer   m_RangeSpreadLoss;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbSarBrightnessFunction.hxx"
#endif

#endif
