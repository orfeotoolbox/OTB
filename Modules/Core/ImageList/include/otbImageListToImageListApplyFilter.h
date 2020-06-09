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

#ifndef otbImageListToImageListApplyFilter_h
#define otbImageListToImageListApplyFilter_h

#include "otbImageListToImageListFilter.h"

namespace otb
{
/** \class ImageListToImageListApplyFilter
 *  \brief This filter applies the filter given as template to the input image list.
 *
 *  It produces one output image in the output list for one input image in the output list.
 *
 *  It uses the given filter methods to generate output information and input requested region.
 *
 *  The filter can be set up via the GetFilter() accessor.
 *
 *  It also support streaming when the templated filter support it.
 *
 *  No type checking is done : it is up to the user to check the adequation between the input images,
 *  output images and filter IO types.
 *
 * \ingroup Streamed
 *
 * \ingroup OTBObjectList
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
class ITK_EXPORT ImageListToImageListApplyFilter : public ImageListToImageListFilter<typename TInputImageList::ImageType, typename TOutputImageList::ImageType>
{
public:
  /** Standard typedefs */
  typedef ImageListToImageListApplyFilter Self;
  typedef ImageListToImageListFilter<typename TInputImageList::ImageType, typename TOutputImageList::ImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageListToImageListApplyFilter, ImageListToImageListFilter);

  /** Template parameters typedefs */
  typedef TInputImageList                         InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename InputImageListType::ImageType  InputImageType;
  typedef TOutputImageList                        OutputImageListType;
  typedef typename OutputImageListType::Pointer   OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType OutputImageType;
  typedef TFilter                                 FilterType;
  typedef typename FilterType::Pointer            FilterPointerType;

  /** Accessors */
  itkSetObjectMacro(Filter, FilterType);
  itkGetObjectMacro(Filter, FilterType);

  itkGetMacro(OutputIndex, unsigned int);
  itkSetMacro(OutputIndex, unsigned int);

  /** Generate output information for the ImageList and for each image
      in the list. */
  void GenerateOutputInformation(void) override;

  /** Generate input requested region for each image in the list. */
  void GenerateInputRequestedRegion(void) override;

protected:
  /** Main computation method */
  void GenerateData(void) override;
  /** Constructor */
  ImageListToImageListApplyFilter();
  /** Destructor */
  ~ImageListToImageListApplyFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageListToImageListApplyFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The processing filter */
  FilterPointerType m_Filter;
  /** The index of the output of the filter to use */
  unsigned int m_OutputIndex;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToImageListApplyFilter.hxx"
#endif

#endif
