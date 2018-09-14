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

#ifndef otbFusionImageBase_h
#define otbFusionImageBase_h

#include "itkTernaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{
/** \class FusionImageBase
 * Basic class for every Fusion classes.
 * \sa TernaryFunctorImageFilter
 *
 * \ingroup OTBPanSharpening
 */
template <class TInputMultiSpectralImage, class TInputMultiSpectralInterpImage, class TInputPanchroImage,
    class TOutputImage, class TFunctor>
class FusionImageBase :  public itk::TernaryFunctorImageFilter<TInputMultiSpectralImage,
      TInputMultiSpectralInterpImage,
      TInputPanchroImage, TOutputImage, TFunctor>
{
public:
  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputMultiSpectralImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputMultiSpectralImage       InputMultiSpectralImageType;
  typedef TInputMultiSpectralInterpImage InputMultiSpectralInterpImageType;
  typedef TInputPanchroImage             InputPanchroImageType;
  typedef TOutputImage                   OutputImageType;
  typedef TFunctor                       FunctorType;

  /** "typedef" for standard classes. */
  typedef FusionImageBase Self;
  typedef itk::TernaryFunctorImageFilter<InputMultiSpectralImageType,
      InputMultiSpectralInterpImageType,
      InputPanchroImageType,
      OutputImageType,
      FunctorType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(FusionImageBase, TernaryFunctorImageFilter);

  /** Supported images definition. */
  typedef typename InputMultiSpectralImageType::PixelType       InputMultiSpectralPixelType;
  typedef typename InputMultiSpectralInterpImageType::PixelType InputMultiSpectralInterpPixelType;
  typedef typename InputPanchroImageType::PixelType             InputPanchroPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  /** Real class typedef definition. */
  typedef typename itk::NumericTraits<InputMultiSpectralPixelType>::RealType       InputMultiSpectralRealType;
  typedef typename itk::NumericTraits<InputMultiSpectralInterpPixelType>::RealType InputMultiSpectralInterpRealType;
  typedef typename itk::NumericTraits<InputPanchroPixelType>::RealType             InputPanchroRealType;
  typedef typename InputMultiSpectralImageType::RegionType                         InputMultiSpectralImageRegionType;
  typedef typename InputMultiSpectralInterpImageType::RegionType
  InputMultiSpectralInterpImageRegionType;
  typedef typename InputPanchroImageType::RegionType InputPanchroImageRegionType;
  typedef typename OutputImageType::RegionType       OutputImageRegionType;

  /** Image size "typedef" definition. */
  typedef typename InputMultiSpectralImageType::SizeType SizeType;

  void SetMultiSpect(const InputMultiSpectralImageType *multiSpect)
  {
    this->SetInput1(multiSpect);
  }
  void SetMultiSpectInterp(const InputMultiSpectralInterpImageType *multiSpectInterp)
  {
    this->SetInput2(multiSpectInterp);
  }
  void SetPanchro(const InputPanchroImageType *panchro)
  {
    this->SetInput3(panchro);
  }

  const InputMultiSpectralImageType* GetMultiSpect()
  {
    if (this->GetNumberOfInputs() < 1)
      {
      return nullptr;
      }
    else return (static_cast<const InputMultiSpectralImageType *>(this->itk::ProcessObject::GetInput(0)));
  }

  const InputMultiSpectralInterpImageType* GetMultiSpectInterp()
  {
    if (this->GetNumberOfInputs() < 2)
      {
      return nullptr;
      }
    else return (static_cast<const InputMultiSpectralInterpImageType *>(this->itk::ProcessObject::GetInput(1)));
  }

  const InputPanchroImageType* GetPanchro()
  {
    if (this->GetNumberOfInputs() < 3)
      {
      return nullptr;
      }
    else return (static_cast<const InputPanchroImageType *>(this->itk::ProcessObject::GetInput(2)));
  }

};

} // end namespace otb

#endif
