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

#ifndef otbSinclairImageFilter_h
#define otbSinclairImageFilter_h

#include "otbQuaternaryFunctorImageFilter.h"
#include "otbSinclairToCovarianceMatrixFunctor.h"
#include <complex>

namespace otb
{

/** \class SinclairImageFilter
 * \brief Computes the Covariance matrix for the sinclair one.
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image. It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 * The output image has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 * The class is templated by the 4 input image (HH, HV, VH and VV) and the used functor.
 * Available functors are :
 * - SinclairToCircularCovarianceMatrixFunctor (default one)
 * - SinclairToCoherencyMatrixFunctor
 * - SinclairToCovarianceMatrixFunctor
 * - SinclairToMuelleMatrixrFunctor
 *
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuelleMatrixrFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */

template <class TInputImageHH, class TInputImageHV,
          class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction = Functor::SinclairToCovarianceMatrixFunctor<
        typename TInputImageHH::PixelType,
        typename TInputImageHV::PixelType,
        typename TInputImageVH::PixelType,
        typename TInputImageVV::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT SinclairImageFilter :  public otb::QuaternaryFunctorImageFilter<TInputImageHH,
      TInputImageHV, TInputImageVH,
      TInputImageVV, TOutputImage,
      TFunction>
{
public:

  /** Standard typedefs */
  typedef SinclairImageFilter Self;
  typedef otb::QuaternaryFunctorImageFilter<TInputImageHH, TInputImageHV,
      TInputImageVH, TInputImageVV, TOutputImage, TFunction>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SinclairImageFilter, QuaternaryFunctorImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType     HHInputImageType;
  typedef typename Superclass::Input1ImagePointer  HHInputImagePointer;
  typedef typename Superclass::Input2ImageType     HVInputImageType;
  typedef typename Superclass::Input2ImagePointer  HVInputImagePointer;
  typedef typename Superclass::Input3ImageType     VHInputImageType;
  typedef typename Superclass::Input3ImagePointer  VHInputImagePointer;
  typedef typename Superclass::Input4ImageType     VVInputImageType;
  typedef typename Superclass::Input4ImagePointer  VVInputImagePointer;
  typedef typename Superclass::OutputImageType     OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename Superclass::FunctorType         FunctorType;

  void SetInputHH(const TInputImageHH * image);
  void SetInputHV(const TInputImageHV * image);
  void SetInputVH(const TInputImageVH * image);
  void SetInputVV(const TInputImageVV * image);


protected:
  /**  Constructor */
  SinclairImageFilter() {}
  /**  Destructor */
  ~SinclairImageFilter() override {}

  void GenerateOutputInformation() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:

  SinclairImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSinclairImageFilter.hxx"
#endif

#endif
