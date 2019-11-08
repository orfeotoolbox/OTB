/*
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

#ifndef otbGroundSpacingImageFunction_h
#define otbGroundSpacingImageFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"

#include "otbGenericRSTransform.h"

namespace otb
{

/**
 * \class GroundSpacingImageFunction
 * \brief Calculate the approximate ground spacing in X and Y directions
 *
 * This uses the 'Haversine' formula to calculate great-circle distances between
 * the two points (that is, the shortest distance over the Earth's surface)
 * giving an 'as-the-crow-flies' distance between the points (ignoring any hills!).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBProjection
 */
template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT GroundSpacingImageFunction : public itk::ImageFunction<TInputImage, itk::Vector<float, 2>, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef GroundSpacingImageFunction Self;

  /** Datatype used for the density */
  typedef float ValueType;
  // typedef std::pair<ValueType, ValueType> FloatType;
  typedef itk::Vector<ValueType, 2> FloatType;

  typedef itk::ImageFunction<TInputImage, FloatType, TCoordRep> Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GroundSpacingImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef otb::GenericRSTransform<double> TransformType;

  typedef typename IndexType::IndexValueType IndexValueType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  FloatType EvaluateAtIndex(const IndexType& index) const override;

  /** Evaluate the function at non-integer positions */
  FloatType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }

  FloatType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  PointType GetPixelLocation(const IndexType& index) const;

protected:
  GroundSpacingImageFunction();
  ~GroundSpacingImageFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GroundSpacingImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  ValueType m_R;
  ValueType m_Deg2radCoef;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGroundSpacingImageFunction.hxx"
#endif

#endif
