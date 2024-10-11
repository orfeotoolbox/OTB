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

#ifndef otbSarBurstExtractionImageFilter_h
#define otbSarBurstExtractionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkSmartPointer.h"

namespace otb
{
/** \class SarBurstExtractionImageFilter
 * \brief Performs a burst extraction by keeping only lines and samples
 * of a required Burst
 *
 * This filter performs a burst extraction by keeping only lines and samples
 * of a required burst. This operation is useful when dealing with
 * Sentinel1 IW SLC products, where each subswath is composed of
 * several overlapping burst separated by black lines. The aim is to separate
 * bursts to be able to process each burst independently.
 * The output image represents only one burst and is smaller in azimuth and range
 * direction than the input line. This filter is the perfect preprocessing step
 * to process each burst of S1 IW SLC product with OTB without suffering from
 * artifacts caused by bursts separation.
 *
 * Note that currently only Sentinel1 IW SLC products are supported.
 *
 * \ingroup OTBSARCalibration
 */

template <class TImage>
class ITK_EXPORT SarBurstExtractionImageFilter : public itk::ImageToImageFilter<TImage, TImage>
{
public:
  // Standard class typedefs
  typedef SarBurstExtractionImageFilter Self;
  typedef itk::ImageToImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(SarBurstExtractionImageFilter, ImageToImageFilter);

  typedef TImage                         ImageType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::PointType  PointType;

  typedef std::pair<unsigned long, unsigned long> RecordType;
  typedef std::vector<RecordType> LinesRecordVectorType;

  // Setter
  itkSetMacro(BurstIndex, unsigned int);
  itkSetMacro(AllPixels, bool);

protected:
  // Constructor
  SarBurstExtractionImageFilter();

  // Destructor
  ~SarBurstExtractionImageFilter() override = default;

  // Needs to be re-implemented since size of output is modified
  virtual void GenerateOutputInformation() override;

  // Needs to be re-implemented since size of output is modified
  virtual void GenerateInputRequestedRegion() override;

  // Actual processing
  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  RegionType OutputRegionToInputRegion(const RegionType& outputRegion) const;

private:
  SarBurstExtractionImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Pair for sample valid selection
  RecordType m_LinesRecord;

  // Pair for sample valid selection
  RecordType m_SamplesRecord;

  // Burst index
  unsigned int m_BurstIndex;

  // Mode for extraction :
  // If true : all pixels of the burst are selected
  // If false : only valid pixels are selected
  bool m_AllPixels;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarBurstExtractionImageFilter.hxx"
#endif


#endif
