/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageListToImageFilter_h
#define otbImageListToImageFilter_h

#include "itkImageSource.h"
#include "otbImageList.h"

namespace otb
{
/** \class ImageListToImageFilter
 *
 *  \brief Base class for all the filters taking an images list as input to
 *  produce an image.
 *
 * \ingroup Images
 * \ingroup Lists
 *
 * \ingroup OTBImageList
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageListToImageFilter : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ImageListToImageFilter         Self;
  typedef itk::ImageSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageListToImageFilter, ImageSource);
  /** Template parameters typedefs */
  typedef TInputImage                               InputImageType;
  typedef typename InputImageType::ConstPointer     InputImagePointer;
  typedef typename InputImageType::RegionType       InputImageRegionType;
  typedef typename InputImageType::PixelType        InputImagePixelType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef ImageList<InputImageType>                 InputImageListType;
  typedef typename InputImageListType::ConstPointer InputImageListConstPointerType;

  /** Derived typedefs */
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  /** InputImage dimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Overriding the SetInput() and GetInput() methods */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageListType* image);
  InputImageListType* GetInput(void);

protected:
  /** Constructor */
  ImageListToImageFilter();
  /** Destructor */
  ~ImageListToImageFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageListToImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToImageFilter.hxx"
#endif

#endif
