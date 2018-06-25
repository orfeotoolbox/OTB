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

#ifndef otbHuMomentsImageFunction_h
#define otbHuMomentsImageFunction_h

#include "itkImageFunction.h"
#include "itkFixedArray.h"

namespace otb
{

/**
 * \class HuMomentsImageFunction
 * \brief Calculate the Hu's invariant parameters.
 *
 * Calculate the Hu's invariants over a specified neighbohood defined
 * as :
 *
 * - \f$ \phi_{1} = c_{11} \f$
 * - \f$ \phi_{2} = c_{20} c_{02} \f$
 * - \f$ \phi_{3} = c_{30} c_{03} \f$
 * - \f$ \phi_{4} = c_{21} c_{12} \f$
 * - \f$ \phi_{5} = Re (c_{30} c_{12}^{3}) \f$
 * - \f$ \phi_{6} = Re (c_{20} c_{12}^{2}) \f$
 * - \f$ \phi_{7} = Im (c_{30} c_{12}^{3}) \f$
 *
 * With :
 *
 *  \f[  c_{p, q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x, y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x, y)\f$ pixel localization;
 *  - \f$ f(x, y)\f$  the pixel value over the \f$(x, y)\f$ coordinate.
 *
 * Note that the output is defined as:
 * \f$ \phi_{i} = output[i-1] \f$
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT HuMomentsImageFunction :
public itk::ImageFunction< TInputImage,
    itk::FixedArray<
    typename itk::NumericTraits<typename TInputImage::PixelType>::RealType,
    7 >,
    TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef HuMomentsImageFunction                                          Self;
  typedef itk::ImageFunction< TInputImage,
                   itk::FixedArray<
                   typename itk::NumericTraits<
                   typename TInputImage::PixelType>::RealType,
                   7 >,
                   TCoordRep >                                            Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HuMomentsImageFunction, ImageFunction);

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
  HuMomentsImageFunction();
  ~HuMomentsImageFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  HuMomentsImageFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  unsigned int m_NeighborhoodRadius;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHuMomentsImageFunction.hxx"
#endif

#endif
