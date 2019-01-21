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

#ifndef otbImageToPathListFilter_h
#define otbImageToPathListFilter_h

#include "otbPathListSource.h"
namespace otb
{
/** \class ImageToPathListFilter
 * \brief Base class used to implement filters that have a image as input and a paths list as output.
 *
 *
 * \ingroup PathListFilters
 * \ingroup PathLists
 *
 * \ingroup OTBPath
 */

template <class TInputImage, class TOutputPath>
//class ITK_EXPORT ImageToPathListFilter : public itk::PathSource<TOutputPath>
class ITK_EXPORT ImageToPathListFilter : public PathListSource<TOutputPath>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPathListFilter         Self;
  typedef PathListSource<TOutputPath>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ProcessObject ProcessObjectType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPathListFilter, PathListSource);

  /** Some convenient typedefs. */
  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::OutputPathType        OutputPathType;
  typedef typename Superclass::OutputPathListType    OutputPathListType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Set/Get the image input of this process object. */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType * image);
  const InputImageType * GetInput(void);

protected:
  ImageToPathListFilter();
  ~ImageToPathListFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageToPathListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathListFilter.hxx"
#endif

#endif
