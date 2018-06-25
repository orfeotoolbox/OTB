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

#ifndef otbImageListToImageListFilter_h
#define otbImageListToImageListFilter_h

#include "otbImageListSource.h"

namespace otb
{
/** \class ImageListToImageListFilter
 *  \brief Base class for all the filters taking an image list as input
 *  to produce an image list.
 * \ingroup Images
 * \ingroup Lists
 *
 * \ingroup OTBObjectList
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageListToImageListFilter
  : public ImageListSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ImageListToImageListFilter    Self;
  typedef ImageListSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageListToImageListFilter, ImageListSource);
  /** Template parameters typedefs */
  typedef          TInputImage                      InputImageType;
  typedef typename InputImageType::ConstPointer     InputImagePointer;
  typedef typename InputImageType::RegionType       InputImageRegionType;
  typedef typename InputImageType::PixelType        InputImagePixelType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef ImageList<InputImageType>                 InputImageListType;
  typedef typename InputImageListType::Pointer      InputImageListPointerType;
  typedef typename InputImageListType::ConstPointer InputImageListConstPointer;
  /** Derived typedefs */
  typedef typename Superclass::OutputImageType            OutputImageType;
  typedef typename Superclass::OutputImageListType        OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType     OutputImagePointer;
  /** InputImage dimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Overriding the SetInput() and GetInput() methods */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageListType * imageList);
  virtual InputImageListType * GetInput(void);

protected:
  /** Constructor */
  ImageListToImageListFilter();
  /** Destructor */
  ~ImageListToImageListFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageListToImageListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToImageListFilter.hxx"
#endif

#endif
