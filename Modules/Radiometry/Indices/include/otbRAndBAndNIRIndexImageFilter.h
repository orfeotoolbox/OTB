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

#ifndef otbRAndBAndNIRIndexImageFilter_h
#define otbRAndBAndNIRIndexImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class RAndBAndNIRIndexImageFilter
   * \brief TODO
   *
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBIndices
 */

template <class TInputImageR, class TInputImageB, class TInputImageNIR, class TOutputImage,
    class TFunction = Functor::ARVI<typename TInputImageR::PixelType,
        typename TInputImageB::PixelType,
        typename TInputImageNIR::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT RAndBAndNIRIndexImageFilter :  public itk::TernaryFunctorImageFilter<TInputImageR, TInputImageB,
      TInputImageNIR, TOutputImage,
      TFunction>
{
public:

  /** Standard typedefs */
  typedef RAndBAndNIRIndexImageFilter Self;
  typedef itk::TernaryFunctorImageFilter<TInputImageR, TInputImageB, TInputImageNIR, TOutputImage,
      TFunction> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RAndBAndNIRIndexImageFilter, TernaryFunctorImageFilter);

  void SetInputR(const TInputImageR * image);
  void SetInputB(const TInputImageB * image);
  void SetInputNIR(const TInputImageNIR * image);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType RInputImageType;
  typedef typename Superclass::Input2ImageType BInputImageType;
  typedef typename Superclass::Input3ImageType NIRInputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::FunctorType     FunctorType;

protected:
  RAndBAndNIRIndexImageFilter();
  ~RAndBAndNIRIndexImageFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  RAndBAndNIRIndexImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndBAndNIRIndexImageFilter.hxx"
#endif

#endif
