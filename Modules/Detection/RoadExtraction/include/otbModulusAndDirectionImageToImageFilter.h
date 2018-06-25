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

#ifndef otbModulusAndDirectionImageToImageFilter_h
#define otbModulusAndDirectionImageToImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ModulusAndDirectionImageToImageFilter
 *
 * \brief Base class for modulus and direction image filters.
 *
 * This is the base class for alls class' generate an image dulus and
 * by using an modulus and a direction images intputs.
 * SetInput() method set the modulus image input and
 * SetInputDirection() set the image direction input.
 *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TInputImage,
    class TInputImageDirection,
    class TOutputImage>
class ITK_EXPORT ModulusAndDirectionImageToImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef ModulusAndDirectionImageToImageFilter              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(ModulusAndDirectionImageToImageFilter, itk::ImageToImageFilter);

  typedef typename Superclass::InputImageType  InputImageType;
  typedef TInputImageDirection                 InputImageDirectionType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Set/Get the image input of this process object.  */
  using Superclass::SetInput;
  void SetInput(const InputImageType *input) override;
  virtual void SetInputDirection(const InputImageDirectionType *direction);
  const InputImageType * GetInput(void);
  const InputImageDirectionType * GetInputDirection(void);

protected:
  ModulusAndDirectionImageToImageFilter();
  ~ModulusAndDirectionImageToImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ModulusAndDirectionImageToImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbModulusAndDirectionImageToImageFilter.hxx"
#endif

#endif
