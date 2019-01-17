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

#ifndef otbRAndGAndNIRIndexImageFilter_h
#define otbRAndGAndNIRIndexImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class RAndGAndNIRIndexImageFilter
   * \brief TODO
   *
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBIndices
 */

template <class TInputImageR, class TInputImageG, class TInputImageNIR, class TOutputImage,
    class TFunction = Functor::AVI<typename TInputImageR::PixelType,
        typename TInputImageG::PixelType,
        typename TInputImageNIR::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT RAndGAndNIRIndexImageFilter :  public itk::TernaryFunctorImageFilter<TInputImageR, TInputImageG,
      TInputImageNIR, TOutputImage,
      TFunction>
{
public:

  /** Standard typedefs */
  typedef RAndGAndNIRIndexImageFilter Self;
  typedef itk::TernaryFunctorImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage,
      TFunction> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RAndGAndNIRIndexImageFilter, TernaryFunctorImageFilter);

  void SetInputR(const TInputImageR * image);
  void SetInputG(const TInputImageG * image);
  void SetInputNIR(const TInputImageNIR * image);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType RInputImageType;
  typedef typename Superclass::Input2ImageType GInputImageType;
  typedef typename Superclass::Input3ImageType NIRInputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::FunctorType     FunctorType;

protected:
  RAndGAndNIRIndexImageFilter();
  ~RAndGAndNIRIndexImageFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  RAndGAndNIRIndexImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndGAndNIRIndexImageFilter.hxx"
#endif

#endif
