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

#ifndef otbContinuousMinimumMaximumImageCalculator_h
#define otbContinuousMinimumMaximumImageCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkNumericTraits.h"

namespace otb
{
/** \class ContinuousMinimumMaximumImageCalculator
   * \brief Compute the continuous maximum and the minimum of an image
   *
   * To compute the continuous maximum and the minimum of an image, this
   * class uses a simple polynomial interpolation.
   *
   * First traditional discrete maximum and minimum are found. Then a second
   * order polynomial is fitted between these extrema and their neighboring
   * pixels.
   *
   * The continuous extrema is assumed to be at the zero of the first order
   * derivative of this polynom.
   *
   * If we denote \f$ (x_0, y_0) \f$ the extrema and  \f$ (x_{-1}, y_{-1}) \f$ and
   * \f$ (x_1, y_1) \f$ its neighbor, the second degree polynom verify the
   * following equations:
   *
   *
   * \f[
   * y_{-1} = a*x_{-1}^2 + b*x_{-1} +c
   * y_0 = a*x_0^2 + b*x_0 +c
   * y_1 = a*x_1^2 + b*x_1 +c
   * \f]
   *
   * The maximum is at \f$ -b/2a \f$ with is
   * \f$ -\frac{(y_1-y_{-1})}{(2*(y_{-1}+y_1-2*y_0))} \f$
   *
   * Remark: image is assumed to be 2 dimensionnal
   *
   *
 *
 * \ingroup OTBStatistics
 */

template <class TInputImage>
class ITK_EXPORT ContinuousMinimumMaximumImageCalculator :
  public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ContinuousMinimumMaximumImageCalculator Self;
  typedef itk::Object                             Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ContinuousMinimumMaximumImageCalculator, Object);

  /** Type definition for the input image. */
  typedef TInputImage ImageType;

  /** Pointer type for the image. */
  typedef typename TInputImage::Pointer ImagePointer;

  /** Const Pointer type for the image. */
  typedef typename TInputImage::ConstPointer ImageConstPointer;

  /** Type definition for the input image pixel type. */
  typedef typename TInputImage::PixelType PixelType;

  /** Type definition for the input image real pixel type. */
  typedef typename itk::NumericTraits<PixelType>::RealType RealPixelType;

  /** Type definition for the input image index type. */
  typedef typename TInputImage::IndexType IndexType;

  /** Type definition for the input image index type. */
  typedef typename TInputImage::PointType ContinuousIndexType;

  /** Type definition for the input image region type. */
  typedef typename TInputImage::RegionType RegionType;

  /** Set the input image. */
  itkSetConstObjectMacro(Image, ImageType);

  /** Compute the minimum value of intensity of the input image. */
  void ComputeMinimum(void);

  /** Compute the maximum value of intensity of the input image. */
  void ComputeMaximum(void);

  /** Compute the minimum and maximum values of intensity of the input image. */
  void Compute(void);

  /** Return the minimum intensity value. */
  itkGetMacro(Minimum, PixelType);

  /** Return the maximum intensity value. */
  itkGetMacro(Maximum, PixelType);

  /** Return the index of the minimum intensity value. */
  itkGetConstReferenceMacro(IndexOfMinimum, IndexType);

  /** Return the index of the maximum intensity value. */
  itkGetConstReferenceMacro(IndexOfMaximum, IndexType);

  /** Return the index of the minimum intensity value. */
  itkGetConstReferenceMacro(ContinuousIndexOfMinimum, ContinuousIndexType);

  /** Return the index of the maximum intensity value. */
  itkGetConstReferenceMacro(ContinuousIndexOfMaximum, ContinuousIndexType);

  /** Set the region over which the values will be computed */
  void SetRegion(const RegionType& region);

protected:
  ContinuousMinimumMaximumImageCalculator();
  ~ContinuousMinimumMaximumImageCalculator() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  PixelType         m_Minimum;
  PixelType         m_Maximum;
  ImageConstPointer m_ContinuousImage;

  IndexType           m_IndexOfMinimum;
  IndexType           m_IndexOfMaximum;
  ContinuousIndexType m_ContinuousIndexOfMinimum;
  ContinuousIndexType m_ContinuousIndexOfMaximum;
  ImageConstPointer   m_Image;

  RegionType m_Region;
  bool       m_RegionSetByUser;

private:
  ContinuousMinimumMaximumImageCalculator(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbContinuousMinimumMaximumImageCalculator.hxx"
#endif

#endif
