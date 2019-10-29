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


#ifndef otbROIdataConversion_h
#define otbROIdataConversion_h

#include <vector>
#include "otbImage.h"
#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ROIdataConversion
 * \brief image data to vector conversion.
 *
 * A class that extract pixel value from an input image according to a ROI
 * image and outputs it on a vector of PixelType.
 *
 * \ingroup OTBProjection
 */
template <class TInputImage, class TInputROIImage>
class ROIdataConversion : public itk::ImageToImageFilter<TInputImage, otb::Image<typename TInputImage::PixelType, 1>>
{
public:
  typedef ROIdataConversion Self;
  typedef itk::ImageToImageFilter<TInputImage, otb::Image<typename TInputImage::PixelType, 1>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(ROIdataConversion, itk::ImageToImageFilter);

  typedef TInputImage                          InputImageType;
  typedef typename TInputImage::PixelType      InputImagePixelType;
  typedef typename TInputImage::Pointer        InputImagePointerType;
  typedef TInputROIImage                       InputROIImageType;
  typedef typename TInputROIImage::Pointer     InputROIImagePointerType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  typedef typename OutputImageType::SizeType::SizeValueType SizeValueType;

  /** Gets/Sets the input image */
  const InputImageType* GetInputImage()
  {
    return this->Superclass::GetInput();
  }
  void SetInputImage(const InputImageType* img)
  {
    this->Superclass::SetInput(img);
  }

  /** Gets/Sets the ROI image */
  InputROIImageType* GetROIImage()
  {
    return static_cast<InputROIImageType*>(this->itk::ProcessObject::GetInput(1));
  }
  void SetROIImage(const InputROIImageType* img)
  {
    this->itk::ProcessObject::SetNthInput(1, const_cast<InputROIImageType*>(img));
  }

protected:
  ROIdataConversion();
  ~ROIdataConversion() override
  {
  }
  void GenerateOutputInformation() override;
  void GenerateInputRequestedRegion() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

  /** Performs its job! */
  void GenerateData() override;

  /** Count the number for sample in the training area */
  SizeValueType GetNumberOfSample();
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbROIdataConversion.hxx"
#endif

#endif
