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

#ifndef otbBinaryImageDensityFunction_h
#define otbBinaryImageDensityFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * \class BinaryImageDensityFunction
 * \brief Calculate the density pixel of  in the neighborhood of a pixel
 *
 * Calculate the variance pixel value over the standard 8, 26, etc. connected
 * neighborhood.  This calculation uses a ZeroFluxNeumannBoundaryCondition.
 *
 * If called with a ContinuousIndex or Point, the calculation is performed
 * at the nearest neighbor.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double ).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT BinaryImageDensityFunction
    : public itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef BinaryImageDensityFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryImageDensityFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::SizeType        RadiusType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Datatype used for the density */
  typedef typename itk::NumericTraits<typename InputImageType::PixelType>::RealType RealType;

  /** Evalulate the function at specified index */
  RealType EvaluateAtIndex(const IndexType& index) const override;

  /** Evaluate the function at non-integer positions */
  RealType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  RealType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro(NeighborhoodRadius, RadiusType);
  itkGetConstReferenceMacro(NeighborhoodRadius, RadiusType);
  void SetNeighborhoodRadius(unsigned int rad)
  {
    m_NeighborhoodRadius.Fill(rad);
    this->Modified();
  }

protected:
  BinaryImageDensityFunction();
  ~BinaryImageDensityFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  BinaryImageDensityFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  RadiusType m_NeighborhoodRadius;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryImageDensityFunction.hxx"
#endif

#endif
