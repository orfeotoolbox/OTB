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

#ifndef otbBSplineDecompositionImageFilter_h
#define otbBSplineDecompositionImageFilter_h

#include <vector>

#include "itkImageLinearIteratorWithIndex.h"
#include "vnl/vnl_matrix.h"

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class BSplineDecompositionImageFilter
 *  \brief This class is an evolution of the itk::BSplineDecompositionImageFilter to handle
 * huge images with this interpolator. For more documentation, please refer to the original
 * class.
 * \sa itk::BSplineDecompositionImageFilter
 * \ingroup ImageFilters
 *
 * \ingroup OTBInterpolation
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT BSplineDecompositionImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BSplineDecompositionImageFilter                    Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSplineDecompositionImageFilter, ImageToImageFilter);

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Inherit input and output image types from Superclass. */
  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::InputImagePointer      InputImagePointer;
  typedef typename Superclass::InputImageConstPointer InputImageConstPointer;
  typedef typename Superclass::OutputImagePointer     OutputImagePointer;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Iterator typedef support */
  typedef itk::ImageLinearIteratorWithIndex<TOutputImage> OutputLinearIterator;

  /** Get/Sets the Spline Order, supports 0th - 5th order splines. The default
   *  is a 3rd order spline. */
  void SetSplineOrder(unsigned int SplineOrder);
  itkGetMacro(SplineOrder, int);

protected:
  BSplineDecompositionImageFilter();
  ~BSplineDecompositionImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;

  /** These are needed by the smoothing spline routine. */
  std::vector<double> m_Scratch;             // temp storage for processing of Coefficients
  typename TInputImage::SizeType m_DataLength;    // Image size
  unsigned int m_SplineOrder;                // User specified spline order (3rd or cubic is the default)
  double       m_SplinePoles[3];             // Poles calculated for a given spline order
  int          m_NumberOfPoles;              // number of poles
  double       m_Tolerance;                  // Tolerance used for determining initial causal coefficient
  unsigned int m_IteratorDirection;              // Direction for iterator incrementing

private:
  BSplineDecompositionImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Determines the poles given the Spline Order. */
  virtual void SetPoles();

  /** Converts a vector of data to a vector of Spline coefficients. */
  virtual bool DataToCoefficients1D();

  /** Converts an N-dimension image of data to an equivalent sized image
   *    of spline coefficients. */
  void DataToCoefficientsND();

  /** Determines the first coefficient for the causal filtering of the data. */
  virtual void SetInitialCausalCoefficient(double z);

  /** Determines the first coefficient for the anti-causal filtering of the data. */
  virtual void SetInitialAntiCausalCoefficient(double z);

  /** Used to initialize the Coefficients image before calculation. */
  void CopyImageToImage();

  /** Copies a vector of data from the Coefficients image to the m_Scratch vector. */
  void CopyCoefficientsToScratch(OutputLinearIterator&);

  /** Copies a vector of data from m_Scratch to the Coefficients image. */
  void CopyScratchToCoefficients(OutputLinearIterator&);

};

} // namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBSplineDecompositionImageFilter.hxx"
#endif

#endif
