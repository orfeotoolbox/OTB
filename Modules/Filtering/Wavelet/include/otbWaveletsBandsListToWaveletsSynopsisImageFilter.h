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

#ifndef otbWaveletsBandsListToWaveletsSynopsisImageFilter_h
#define otbWaveletsBandsListToWaveletsSynopsisImageFilter_h


#include "otbImageListToImageFilter.h"

namespace otb
{

/** \class WaveletsBandsListToWaveletsSynopsisImageFilter
 *  \brief Converts a list of wavelets bands to the traditional multi-resolution wavelets view
 *
 *  The decimation ratio is taken into account.
 *
 * \ingroup OTBWavelet
 * \sa WaveletsSynopsisImageToWaveletsBandsListFilter
 */

template <class TImageList, class TImage>
class ITK_EXPORT WaveletsBandsListToWaveletsSynopsisImageFilter : public ImageListToImageFilter<typename TImageList::ImageType, TImage>
{
public:
  /** Standard typedefs */
  typedef WaveletsBandsListToWaveletsSynopsisImageFilter Self;
  typedef ImageListToImageFilter<typename TImageList::ImageType, TImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Useful typedefs */
  typedef TImageList                             InputImageListType;
  typedef typename InputImageListType::ImageType InputImageType;
  typedef TImage                                 OutputImageType;
  typedef typename OutputImageType::RegionType   RegionType;

  /** Creation through object factory macro */
  itkTypeMacro(WaveletsBandsListToWaveletsSynopsisImageFilter, ImageToImageListFilter);

  /** Set the decimation ratio */
  itkSetMacro(DecimationRatio, unsigned int);

  /** Get the decimation ratio */
  itkGetMacro(DecimationRatio, unsigned int);

protected:
  /** Main computation method */
  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  virtual void GenerateOutputInformation(void) override;

  /**
   * GenerateInputRequestedRegion
   * Set the requested region of each image in the list.
   */
  virtual void GenerateInputRequestedRegion(void) override;

  /** Constructor */
  WaveletsBandsListToWaveletsSynopsisImageFilter();

  /** Destructor */
  virtual ~WaveletsBandsListToWaveletsSynopsisImageFilter();

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  WaveletsBandsListToWaveletsSynopsisImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The decimation ratio used in the decomposition */
  unsigned int m_DecimationRatio;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletsBandsListToWaveletsSynopsisImageFilter.hxx"
#endif

#endif
