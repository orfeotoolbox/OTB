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

#ifndef otbVectorImageToImageListFilter_h
#define otbVectorImageToImageListFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/** \class VectorImageToImageListFilter
 *  \brief This class aims at converting a multi-band image to a list of scalar images.
 *
 * This class takes a multi-band image represented as an otb::VectorImage and produces a list
 * of scalar images corresponding to each band of the input image.
 *
 * Casting is done through standard cast operation.
 *
 * The output of this filter didn't support the use of iterator on it, use a selector filter
 * to avoid break the output
 *
 * \ingroup Streamed
 *
 * \ingroup OTBObjectList
 */
template <class TVectorImageType, class TImageList>
class ITK_EXPORT VectorImageToImageListFilter
  : public ImageToImageListFilter<TVectorImageType, typename TImageList::ImageType>
{
public:
  /** Standard typedefs */
  typedef VectorImageToImageListFilter Self;
  typedef ImageToImageListFilter<TVectorImageType,
      typename TImageList::ImageType>         Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToImageListFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TVectorImageType                        InputVectorImageType;
  typedef typename InputVectorImageType::Pointer  InputVectorImagePointerType;
  typedef TImageList                              OutputImageListType;
  typedef typename OutputImageListType::Pointer   OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointerType;

  /** Generate the input requested region from the first element in the list. */
  void GenerateInputRequestedRegion(void) override;

  /** Generate the output information by building the output list. */
  void GenerateOutputInformation(void) override;

protected:

  /** Main computation method */
  void GenerateData(void) override;

  /** Constructor */
  VectorImageToImageListFilter() {};
  /** Destructor */
  ~VectorImageToImageListFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VectorImageToImageListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageToImageListFilter.hxx"
#endif

#endif
