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

#ifndef otbImageListToSingleImageFilter_h
#define otbImageListToSingleImageFilter_h

#include "otbImageListToImageFilter.h"

namespace otb
{
/** \class ImageListToSingleImageFilter
 *  \brief This class aims at converting a image list to one (scalar or vector) image.
 *
 * This class takes a image list image represented as an otb::ImageList and produces a
 * output which contains of scalar/vector images corresponding to the element selected.
 * This filter avoid to break the pipeline after the use of an filter which derive
 * from ImageToImagelistFilter class.
 *
 *
 * \ingroup Streamed
 *
 * \ingroup OTBObjectList
 */

template <class TInputImageType>
class ITK_EXPORT ImageListToSingleImageFilter : public ImageListToImageFilter<TInputImageType, TInputImageType>
{
public:
  /** Standard typedefs */
  typedef ImageListToSingleImageFilter Self;
  typedef ImageListToImageFilter<TInputImageType, TInputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputImageType                   InputImageType;
  typedef typename InputImageType::Pointer  InputImagePointerType;
  typedef ImageList<InputImageType>         InputImageListType;
  typedef TInputImageType                   OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageListToSingleImageFilter, ImageListToImageFilter);

  /** Set/Get the position of the extracted image from the image list */
  itkGetMacro(ExtractedImagePosition, unsigned int);
  itkSetMacro(ExtractedImagePosition, unsigned int);

  InputImagePointerType GetOutput(void);

protected:
  /** Constructor */
  ImageListToSingleImageFilter();
  /** Destructor */
  ~ImageListToSingleImageFilter() override
  {
  }

  /** Main computation method */
  void GenerateData() override;

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  ImageListToSingleImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int m_ExtractedImagePosition;

}; // end of class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToSingleImageFilter.hxx"
#endif

#endif
