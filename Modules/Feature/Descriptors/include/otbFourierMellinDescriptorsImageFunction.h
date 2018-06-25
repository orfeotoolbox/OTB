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

#ifndef otbFourierMellinDescriptorsImageFunction_h
#define otbFourierMellinDescriptorsImageFunction_h

#include "itkImageFunction.h"

#include "otbImage.h"

#include "otbMath.h"
#include <complex>

namespace otb
{

/**
 * \class FourierMellinDescriptorsImageFunction
 * \brief Calculate the Fourier-Mellin descriptors in the specified
 * neighborhood.
 *
 * Calculate the Fourier-Mellin descriptors over a specified neighborhood.
 * The implemented equation is:
 *
 * D_{p, q}=|M_{p, q}|
 *
 * With:
 *
 * \f[  M_{p, q}=\frac{1}{2\pi} \int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot
 (x^{2}+y^{2})^{\frac{p-2+\sigma-iq}{2}} \cdot f(x, y) \cdot dx \cdot dy \f]
 *
 * And:
 *  - \f$(x, y)\f$ pixel localization;
 *  - \f$ f(x, y)\f$ the pixel value over the \f$(x, y)\f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBDescriptors
 */

template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT FourierMellinDescriptorsImageFunction :
    public itk::ImageFunction <TInputImage,
                               std::vector< std::vector<
                               typename itk::NumericTraits<
                               typename TInputImage::PixelType>::RealType > >,
                               TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef FourierMellinDescriptorsImageFunction                                Self;
  typedef itk::ImageFunction<TInputImage,
                             std::vector< std::vector<
                             typename itk::NumericTraits<
                             typename TInputImage::PixelType>::RealType > >,
                             TCoordRep>                                        Superclass;
  typedef itk::SmartPointer<Self>                                              Pointer;
  typedef itk::SmartPointer<const Self>                                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FourierMellinDescriptorsImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::OutputType          OutputType;

  typedef double                                   ScalarRealType;
  typedef typename std::complex<ScalarRealType>    ScalarComplexType;
  typedef typename std::vector< std::vector< ScalarComplexType > >  ComplexType;

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

  itkSetMacro(Pmax, unsigned int);
  itkGetConstReferenceMacro(Pmax, unsigned int);
  itkSetMacro(Qmax, unsigned int);
  itkGetConstReferenceMacro(Qmax, unsigned int);

protected:
  FourierMellinDescriptorsImageFunction();
  ~FourierMellinDescriptorsImageFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  FourierMellinDescriptorsImageFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  unsigned int m_Pmax;
  unsigned int m_Qmax;
  unsigned int m_NeighborhoodRadius;
  double       m_Sigma;


};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFourierMellinDescriptorsImageFunction.hxx"
#endif

#endif
