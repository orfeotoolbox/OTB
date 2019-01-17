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

#ifndef otbImageListToRCC8GraphFilter_h
#define otbImageListToRCC8GraphFilter_h

#include "otbRCC8GraphSource.h"
#include "otbImageList.h"

namespace otb
{
/** \class ImageListToRCC8GraphFilter
 * \brief Base class for filters taking an images list as input to
 * produce a RCC8 graph.
 *
 * \ingroup OTBRCC8
 */
template <class TInputImage, class TOutputGraph>
class ITK_EXPORT ImageListToRCC8GraphFilter
  : public RCC8GraphSource<TOutputGraph>
{
public:
  /** Standard typedefs */
  typedef ImageListToRCC8GraphFilter    Self;
  typedef RCC8GraphSource<TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageListToRCC8GraphFilter, RCC8GraphSource);
  /** Template input parameters typedefs */
  typedef          TInputImage                      InputImageType;
  typedef typename InputImageType::ConstPointer     InputImagePointer;
  typedef typename InputImageType::RegionType       InputImageRegionType;
  typedef typename InputImageType::PixelType        InputImagePixelType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef ImageList<InputImageType>                 InputImageListType;
  typedef typename InputImageListType::Pointer      InputImageListPointerType;
  typedef typename InputImageListType::ConstPointer InputImageListConstPointer;
  /** Template output parameters typedefs */
  typedef TOutputGraph                                OutputGraphType;
  typedef typename Superclass::OutputGraphPointerType OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType        VertexType;
  typedef typename VertexType::Pointer                VertexPointerType;
  typedef typename OutputGraphType::RCC8ValueType     RCC8ValueType;
  /** InputImage dimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Overriding the SetInput() and GetInput() methods */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageListType * imageList);
  virtual InputImageListType * GetInput(void);

protected:
  /** Constructor */
  ImageListToRCC8GraphFilter();
  /** Destructor */
  ~ImageListToRCC8GraphFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageListToRCC8GraphFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToRCC8GraphFilter.hxx"
#endif

#endif
