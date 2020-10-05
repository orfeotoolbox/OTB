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

#ifndef otbImageToHessianDeterminantImageFilter_h
#define otbImageToHessianDeterminantImageFilter_h

#include "itkProcessObject.h"
#include "itkImageToImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"
#include "itkFixedArray.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class HessianDeterminant
 * \brief This functor computes the determinant of  symmetric 2*2 matrix .
 *
 * A Hessian matrix is symmetric, we compute only the 3 first parameters
 * The First  component is  : Dxx
 * The Second component is  : Dyy
 * The Third  component is  : Dxy
 *
 * \ingroup OTBDescriptors
 */
template <class TInput, class TOutput>
class HessianDeterminant
{
public:
  HessianDeterminant()
  {
  }
  virtual ~HessianDeterminant()
  {
  }

  /*
   *
   * \param input : A symmetric Hessian Matrix
   * \return  Determinant of the Hessian Matrix
   */
  inline TOutput operator()(const TInput& input)
  {
    return static_cast<TOutput>(input[0] * input[1] - input[2] * input[2]);
  }

  bool operator!=(const HessianDeterminant) const
  {
    return false;
  }

  bool operator==(const HessianDeterminant& other) const
  {
    return !(*this != other);
  }
};
}
/** \class ImageToHessianDeterminantImageFilter
 *  \brief This class compute the Hessian determinant of each pixel of an input image
 *
 *  For an input image, the Hessian matrix of each pixel is computed an then the determinant
 *  of each pixel's matrix is computed.
 *
 *  \sa itk::HessianRecursiveGaussianImageFilter
 *
 * \ingroup OTBDescriptors
 */
template <class TInputImage, class TOutputImage, class TPrecision = double>
class ITK_EXPORT ImageToHessianDeterminantImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:
  /** Standard class typedefs. */
  typedef ImageToHessianDeterminantImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Set/Get the number of iterations */
  itkSetMacro(Sigma, double);
  itkGetMacro(Sigma, double);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToHessianDeterminantImageFilter, ImageToImageFilter);

  /* Initialization of Input & Output Images*/
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef TOutputImage                        OutputImageType;
  typedef typename OutputImageType::Pointer   OutputImagePointerType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef itk::FixedArray<TPrecision, 3> VectorType;
  typedef otb::Image<VectorType, 2>      VectorImageType;
  typedef typename VectorImageType::Pointer VectorImagePointerType;

  /** Internal filters typedefs */
  typedef itk::HessianRecursiveGaussianImageFilter<InputImageType, VectorImageType> ImageToHessianImageFilterType;
  typedef typename ImageToHessianImageFilterType::Pointer ImageToHessianImageFilterPointerType;
  typedef Functor::HessianDeterminant<VectorType, OutputPixelType> DeterminantFunctorType;
  typedef itk::UnaryFunctorImageFilter<VectorImageType, OutputImageType, DeterminantFunctorType> DeterminantFilterType;
  typedef typename DeterminantFilterType::Pointer DeterminantFilterPointerType;

protected:
  /**
   * Constructor.
   */
  ImageToHessianDeterminantImageFilter();
  /**
   * Destructor.
   */
  ~ImageToHessianDeterminantImageFilter() override;
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Main computation method.
   */
  void GenerateData() override;

private:
  /** Hessian filter */
  ImageToHessianImageFilterPointerType m_HessianFilter;
  /** Hessian Determinant Computation filter */
  DeterminantFilterPointerType m_DeterminantFilter;

  /** Width of the Gaussian used in the HessianRecursiveGaussianImageFilter */
  double m_Sigma;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToHessianDeterminantImageFilter.hxx"
#endif

#endif
