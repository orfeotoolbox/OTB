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

#ifndef otbImageToPathFilter_h
#define otbImageToPathFilter_h

#include "itkPathSource.h"

namespace otb
{
/**
 * \class ImageToPathFilter
 * \brief Base class for filters taking an image as input, and producing an
 * output of type Path.
 *
 * \ingroup OTBPath
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToPathFilter : public itk::PathSource<TOutputPath>
{
public:
  /// Standard class typedefs
  typedef ImageToPathFilter             Self;
  typedef itk::PathSource<TOutputPath>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::ProcessObject            ProcessObjectType;
  /// Method for creation through the object factory
  itkNewMacro(Self);
  /// Runtime information
  itkTypeMacro(PathToImageFilter, PathSource);
  /// template parameters typedefs
  typedef TInputImage                      InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef TOutputPath                      OutputPathType;
  typedef typename OutputPathType::Pointer OutputPathPointerType;
  /// Input image dimension
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /// Input image Setters and getters
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType* Image);
  const InputImageType* GetInput(void);

protected:
  ImageToPathFilter();
  ~ImageToPathFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageToPathFilter(const Self&);
  void operator=(const Self&);
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathFilter.hxx"
#endif

#endif
