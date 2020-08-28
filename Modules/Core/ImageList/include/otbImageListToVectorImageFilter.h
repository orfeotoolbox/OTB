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

#ifndef otbImageListToVectorImageFilter_h
#define otbImageListToVectorImageFilter_h

#include "otbImageListToImageFilter.h"

namespace otb
{
/** \class ImageListToVectorImageFilter
 *  \brief Converts an ImageList to a VectorImage.
 *
 * The output VectorImage has n band, where n is the number of images in the image list.
 *
 * This filter assumes that the images in the input ImageList have all the same size.
 *
 * Casting is done through standard cast operation.
 *
 * \ingroup Streamed
 *
 * \ingroup OTBImageList
 */
template <class TImageList, class TVectorImage>
class ITK_EXPORT ImageListToVectorImageFilter : public ImageListToImageFilter<typename TImageList::ImageType, TVectorImage>
{
public:
  /** Standard typedefs */
  typedef ImageListToVectorImageFilter Self;
  typedef ImageListToImageFilter<typename TImageList::ImageType, TVectorImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageListToVectorImageFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TVectorImage                            OutputVectorImageType;
  typedef typename OutputVectorImageType::Pointer OutputVectorImagePointerType;
  typedef TImageList                              InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename InputImageListType::ImageType  InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;

protected:
  /** Main computation method */
  void GenerateData(void) override;

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  void GenerateOutputInformation(void) override;

  /**
   * GenerateInputRequestedRegion
   * Set the requested region of each image in the list.
   */
  void GenerateInputRequestedRegion(void) override;

  /** Constructor */
  ImageListToVectorImageFilter(){};
  /** Destructor */
  ~ImageListToVectorImageFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageListToVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToVectorImageFilter.hxx"
#endif

#endif
