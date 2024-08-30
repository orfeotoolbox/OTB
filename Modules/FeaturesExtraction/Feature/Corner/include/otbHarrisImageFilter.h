/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbHarrisImageFilter_h
#define otbHarrisImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "otbHessianToScalarImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "otbMath.h"

namespace otb
{

/** \class HarrisImageFilter
   \brief This filter performs the computation of the Harris measure as followed.

The derivative computation is performed by a
convolution with the derivative of a Gaussian kernel of
variance \f$\sigma_D\f$ (derivation scale) and
the smoothing of the image is performed by convolving with a
Gaussian kernel of variance \f$\sigma_I\f$ (integration
scale). This allows the computation of the following matrix:
\f[
\mu(\mathbf{x},\sigma_I,\sigma_D) = \sigma_D^2 g(\sigma_I)\star
\left[\begin{array}{cc} L_x^2(\mathbf{x},\sigma_D) &
L_xL_y^2(\mathbf{x},\sigma_D)\\ L_xL_y^2(\mathbf{x},\sigma_D)&
L_y^2(\mathbf{x},\sigma_D) \end{array}\right] \f]
The output of the detector is \f$[det(\mu) - \alpha trace^2(\mu)\f$.

The interest points can then be extracted with a thresholding filter.
 *
 *
 * \ingroup OTBCorner
 */

template <class TInputImage, class TOutputImage>
class HarrisImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef HarrisImageFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(HarrisImageFilter, ImageToImageFilter);

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::SizeType  SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;

  //  typedef typename InputImageType::SizeType SizeType;

  typedef itk::Image<itk::SymmetricSecondRankTensor<typename itk::NumericTraits<InputPixelType>::RealType, InputImageDimension>, InputImageDimension>
      TensorType;

  typedef itk::HessianRecursiveGaussianImageFilter<InputImageType, TensorType> HessianFilterType;

  typedef itk::RecursiveGaussianImageFilter<TensorType, TensorType>          GaussianFilterType;
  typedef otb::HessianToScalarImageFilter<TensorType, OutputImageType>       HessianToScalarFilterType;
  typedef otb::MultiplyByScalarImageFilter<OutputImageType, OutputImageType> MultiplyScalarFilterType;

  itkSetMacro(SigmaD, double);
  itkGetConstReferenceMacro(SigmaD, double);
  itkSetMacro(SigmaI, double);
  itkGetConstReferenceMacro(SigmaI, double);
  itkSetMacro(Alpha, double);
  itkGetConstReferenceMacro(Alpha, double);

protected:
  HarrisImageFilter();
  ~HarrisImageFilter() override
  {
  }

  void GenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  HarrisImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  double m_SigmaD;
  double m_SigmaI;
  double m_Alpha;

  typename HessianFilterType::Pointer         m_HessianFilter;
  typename GaussianFilterType::Pointer        m_GaussianFilter0;
  typename GaussianFilterType::Pointer        m_GaussianFilter1;
  typename HessianToScalarFilterType::Pointer m_HessianToScalarFilter;
  typename MultiplyScalarFilterType::Pointer  m_MultiplyScalarFilter;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHarrisImageFilter.hxx"
#endif

#endif
