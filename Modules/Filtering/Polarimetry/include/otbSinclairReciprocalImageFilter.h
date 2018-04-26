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

#ifndef otbSinclairReciprocalImageFilter_h
#define otbSinclairReciprocalImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "otbSinclairToReciprocalCovarianceMatrixFunctor.h"
#include <complex>

namespace otb
{

/** \class SinclairReciprocalImageFilter
 * \brief Convert the Sinclair reciprocal matrix.
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image. It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 * The output image has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element a sotored from left to right, line by line.
 *
 * The class is templated by the 3 input image (HH, HV_VH and VV) and the used functor.
 * Available functors are :
 * - SinclairToReciprocalCovarianceMatrixFunctor (default one)
 * - SinclairToReciprocalCircularCovarianceMatrixFunctor
 * - SinclairToReciprocalCoherencyMatrixFunctor
 *
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */


template <class TInputImageHH, class TInputImageHV_VH,
          class TInputImageVV, class TOutputImage,
    class TFunction = Functor::SinclairToReciprocalCovarianceMatrixFunctor<
        typename TInputImageHH::PixelType,
        typename TInputImageHV_VH::PixelType,
        typename TInputImageVV::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT SinclairReciprocalImageFilter :  public itk::TernaryFunctorImageFilter<TInputImageHH,
      TInputImageHV_VH, TInputImageVV,
      TOutputImage, TFunction>
{
public:

  /** Standard typedefs */
  typedef SinclairReciprocalImageFilter Self;
  typedef itk::TernaryFunctorImageFilter<TInputImageHH, TInputImageHV_VH,
      TInputImageVV, TOutputImage, TFunction>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SinclairReciprocalImageFilter, itk::TernaryFunctorImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType     HHInputImageType;
  typedef typename Superclass::Input1ImagePointer  HHInputImagePointer;
  typedef typename Superclass::Input2ImageType     HV_VHInputImageType;
  typedef typename Superclass::Input2ImagePointer  HV_VHInputImagePointer;
  typedef typename Superclass::Input3ImageType     VVInputImageType;
  typedef typename Superclass::Input3ImagePointer  VVInputImagePointer;
  typedef typename Superclass::OutputImageType     OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename Superclass::FunctorType         FunctorType;

  void SetInputHH(const TInputImageHH * image);
  // This method set the second input, same as SetInputVH
  void SetInputHV(const TInputImageHV_VH * image);
  // This method set the second input, same as SetInputHV
  void SetInputVH(const TInputImageHV_VH * image);
  // This method set the second input, same as SetInputHV and SetInputHV
  void SetInputHV_VH(const TInputImageHV_VH * image);
  void SetInputVV(const TInputImageVV * image);


protected:
  /**  Constructor */
  SinclairReciprocalImageFilter() {}
  /**  Destructor */
  ~SinclairReciprocalImageFilter() override {}

  void GenerateOutputInformation() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:

  SinclairReciprocalImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSinclairReciprocalImageFilter.txx"
#endif

#endif
