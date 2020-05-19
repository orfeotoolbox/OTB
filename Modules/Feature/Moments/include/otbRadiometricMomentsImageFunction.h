/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRadiometricMomentsImageFunction_h
#define otbRadiometricMomentsImageFunction_h

#include "itkImageFunction.h"
#include "otbRadiometricMomentsFunctor.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkFixedArray.h"

namespace otb
{

/**
 * \class RadiometricMomentsImageFunction
 * \brief Calculate the radiometric moments.
 *
 * Calculate the radiometric moments over a specified neighborhood
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBMoments
 */


template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT RadiometricMomentsImageFunction
    : public itk::ImageFunction<TInputImage, itk::FixedArray<typename itk::NumericTraits<typename TInputImage::PixelType>::RealType, 4>, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef RadiometricMomentsImageFunction Self;
  typedef itk::ImageFunction<TInputImage, itk::FixedArray<typename itk::NumericTraits<typename TInputImage::PixelType>::RealType, 4>, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RadiometricMomentsImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef typename Superclass::OutputType OutputType;
  typedef typename OutputType::ValueType  ScalarRealType;

  typedef TCoordRep CoordRepType;

  typedef Functor::RadiometricMomentsFunctor<itk::ConstNeighborhoodIterator<InputImageType>, ScalarRealType> FunctorType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  OutputType EvaluateAtIndex(const IndexType& index) const override;

  /** Evaluate the function at non-integer positions */
  OutputType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the radius of the neighborhood over which the
   *  statistics are evaluated
   */
  itkSetMacro(NeighborhoodRadius, unsigned int);
  itkGetConstReferenceMacro(NeighborhoodRadius, unsigned int);

protected:
  RadiometricMomentsImageFunction();
  ~RadiometricMomentsImageFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  RadiometricMomentsImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int m_NeighborhoodRadius;
  FunctorType  m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometricMomentsImageFunction.hxx"
#endif

#endif
