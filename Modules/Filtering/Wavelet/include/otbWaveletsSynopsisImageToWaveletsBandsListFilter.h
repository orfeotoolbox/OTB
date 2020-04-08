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

#ifndef otbWaveletsSynopsisImageToWaveletsBandsListFilter_h
#define otbWaveletsSynopsisImageToWaveletsBandsListFilter_h

#include "otbImageToImageListFilter.h"
#include "itkRegionOfInterestImageFilter.h"


namespace otb
{

/** \class WaveletsSynopsisImageToWaveletsBandsListFilter
 *  \brief This class convert the standard wavelets view to a list of bands.
 *
 * \ingroup OTBWavelet
 * \sa WaveletsBandsListToWaveletsSynopsisImageFilter
 */

template <class TImage, class TImageList>
class ITK_EXPORT WaveletsSynopsisImageToWaveletsBandsListFilter : public ImageToImageListFilter<TImage, typename TImageList::ImageType>
{
public:
  /** Standard typedefs */
  typedef WaveletsSynopsisImageToWaveletsBandsListFilter Self;
  typedef ImageToImageListFilter<TImage, typename TImageList::ImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletsSynopsisImageToWaveletsBandsListFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TImage                                  InputImageType;
  typedef typename InputImageType::RegionType     RegionType;
  typedef TImageList                              OutputImageListType;
  typedef typename OutputImageListType::ImageType OutputImageType;

  typedef itk::RegionOfInterestImageFilter<InputImageType, InputImageType> ExtractFilterType;

  typedef typename ExtractFilterType::Pointer ExtractFilterPointerType;

  typedef std::vector<ExtractFilterPointerType> ExtractFilterVectorType;

  /** Set the number of levels */
  itkSetMacro(NumberOfLevels, unsigned int);

  /** Get the number of levels */
  itkGetMacro(NumberOfLevels, unsigned int);

  /** Set the decimation ratio */
  itkSetMacro(DecimationRatio, unsigned int);

  /** Get the decimation ratio */
  itkGetMacro(DecimationRatio, unsigned int);

  /** If the filter is modified, the extract list need to be regenerated */
  virtual void Modified() const override;

protected:
  /** Constructor */
  WaveletsSynopsisImageToWaveletsBandsListFilter();

  /** Destructor */
  virtual ~WaveletsSynopsisImageToWaveletsBandsListFilter();

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Generate the input requested region from the first element in the list. */
  virtual void GenerateInputRequestedRegion(void) override;

  /** Generate the output information by building the output list. */
  virtual void GenerateOutputInformation(void) override;

  /** Main computation method */
  virtual void GenerateData(void) override;

private:
  WaveletsSynopsisImageToWaveletsBandsListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The number of levels in the decomposition */
  unsigned int m_NumberOfLevels;

  /** The decimation ratio used in the decomposition */
  unsigned int m_DecimationRatio;

  /** The vector of extraction filters for each band */
  ExtractFilterVectorType m_ExtractFilters;

  /** True if extract list is up-to-date */
  mutable bool m_ExtractFiltersUpToDate;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletsSynopsisImageToWaveletsBandsListFilter.hxx"
#endif

#endif
