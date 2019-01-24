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

#ifndef otbFlusserMomentsImageFunction_h
#define otbFlusserMomentsImageFunction_h

#include "itkImageFunction.h"
#include "itkFixedArray.h"

namespace otb
{

/**
 * \class FlusserMomentsImageFunction
 * \brief Calculate the Flusser's invariant parameters.
 *
 * Calculate the Flusser's invariants over a specified neighborhood
 * defined as :
 *
 * - \f$ \psi_{1} = c_{11} \f$
 * - \f$ \psi_{2} = c_{21} c_{12} \f$
 * - \f$ \psi_{3} = Re (c_{20} c_{12}^{2} )\f$
 * - \f$ \psi_{4} = Im (c_{20} c_{12}^{2} )\f$
 * - \f$ \psi_{5} = Re (c_{30} c_{12}^{3} )\f$
 * - \f$ \psi_{6} = Im (c_{30} c_{12}^{3} )\f$
 * - \f$ \psi_{7} = c_{22} \f$
 * - \f$ \psi_{8} = Re (c_{31} c_{12}^{2} )\f$
 * - \f$ \psi_{9} = Im (c_{31} c_{12}^{2} )\f$
 * - \f$ \psi_{10} = Re (c_{40} c_{12}^{4} )\f$
 * - \f$ \psi_{11} = Im (c_{40} c_{12}^{4} )\f$
 *
 * With :
 *
 *  \f[  c_{p, q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x, y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x, y)\f$ pixel localization;
 *  - \f$ f(x, y)\f$ the pixel value over the \f$(x, y)\f$ coordinate.
 *
 * Note that the output is defined as:
 * \f$ \psi_{i} = output[i-1] \f$
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputImage, class TCoordRep = double >
class ITK_EXPORT FlusserMomentsImageFunction :
  public itk::ImageFunction< TInputImage,
    itk::FixedArray<
    typename itk::NumericTraits<typename TInputImage::PixelType>::RealType,
    11 >,
    TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef FlusserMomentsImageFunction                                     Self;
  typedef itk::ImageFunction< TInputImage,
                   itk::FixedArray<
                   typename itk::NumericTraits<
                   typename TInputImage::PixelType>::RealType,
                   11 >,
                   TCoordRep >                                            Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FlusserMomentsImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef typename Superclass::OutputType          OutputType;
  typedef typename OutputType::ValueType           ScalarRealType;

  typedef TCoordRep                                CoordRepType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

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

  /** Get/Set the radius of the neighborhood over which the
   *  statistics are evaluated
   */
  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );

protected:
  FlusserMomentsImageFunction();
  ~FlusserMomentsImageFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  FlusserMomentsImageFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  unsigned int m_NeighborhoodRadius;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlusserMomentsImageFunction.hxx"
#endif

#endif

