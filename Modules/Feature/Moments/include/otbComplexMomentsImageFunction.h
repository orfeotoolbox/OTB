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

#ifndef otbComplexMomentsImageFunction_h
#define otbComplexMomentsImageFunction_h

#include "itkImageFunction.h"

#include <complex>

namespace otb
{

/**
 * \class ComplexMomentsImageFunction
 * \brief Calculate the complex moment values in the specified
 * neighborhood.
 *
 * Calculate the complex moment values over a specified neighborhood.
 * The implemented equation is:
 *
 *  \f[  c_{p, q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x, y) \cdot
 dx \cdot dy \f]
 *
 * With:
 *
 *   - \f$ (x, y) \f$ pixel localization;
 *   - \f$ f(x, y) \f$  the pixel value over the \f$(x, y) \f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT ComplexMomentsImageFunction : public itk::ImageFunction<TInputImage, std::vector<std::vector<std::complex<double>>>, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentsImageFunction Self;
  typedef itk::ImageFunction<TInputImage, std::vector<std::vector<std::complex<double>>>, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentsImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::OutputType          OutputType;

  typedef double                                ScalarRealType;
  typedef typename std::complex<ScalarRealType> ScalarComplexType;

  typedef TCoordRep CoordRepType;

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

  itkSetMacro(Pmax, unsigned int);
  itkGetConstReferenceMacro(Pmax, unsigned int);
  itkSetMacro(Qmax, unsigned int);
  itkGetConstReferenceMacro(Qmax, unsigned int);

protected:
  ComplexMomentsImageFunction();
  ~ComplexMomentsImageFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ComplexMomentsImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int m_Pmax;
  unsigned int m_Qmax;
  unsigned int m_NeighborhoodRadius;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentsImageFunction.hxx"
#endif

#endif
