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

#ifndef otbSarDeburstImageFilter_h
#define otbSarDeburstImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkSmartPointer.h"

namespace otb
{
/** \class SarDeburstImageFilter 
 * \brief Performs a deburst operation by removing redundant lines
 * 
 * This filter performs a deburst operation by removing redundant
 * lines between burst. This operation is useful when dealing with
 * Sentinel1 IW SLC products, where each subswath is composed of
 * several overlapping burst separated by black lines. Lines to remove
 * are computed by SAR sensor model in OSSIM plugins. The output image
 * is smaller in azimuth direction than the input line, because of
 * removed lines. Note that the output sensor model is updated
 * accordingly. This deburst filter is the perfect preprocessing step
 * to orthorectify S1 IW SLC product with OTB without suffering from
 * artifacts caused by bursts separation.
 * 
 * Note that currently only Sentinel1 IW SLC products are supported.
 * 
 * \ingroup OTBSARCalibration
 */

template <class TImage> class ITK_EXPORT SarDeburstImageFilter :
    public itk::ImageToImageFilter<TImage,TImage>
{
public:
  // Standard class typedefs
  typedef SarDeburstImageFilter                  Self;
  typedef itk::ImageToImageFilter<TImage,TImage> Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(SarDeburstImageFilter,ImageToImageFilter);

  typedef TImage                                 ImageType;
  typedef typename ImageType::RegionType         RegionType;
  typedef typename ImageType::PointType          PointType;

  typedef std::pair<unsigned long, unsigned long> RecordType;
  typedef std::vector<RecordType>                 LinesRecordVectorType;
	
  // Setter
  itkSetMacro(OnlyValidSample, bool);
  
protected:
  // Constructor
  SarDeburstImageFilter();

  // Destructor
  virtual ~SarDeburstImageFilter() override {};

  // Needs to be re-implemented since size of output is modified
  virtual void GenerateOutputInformation() override;

  // Needs to be re-implemented since size of output is modified
  virtual void GenerateInputRequestedRegion() override;

  // Actual processing
  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void ThreadedGenerateDataWithAllSamples(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);
  void ThreadedGenerateDataWithOnlyValidSamples(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

  RegionType OutputRegionToInputRegion(const RegionType& outputRegion) const;
  
private:
  SarDeburstImageFilter(const Self&) = delete;
  void operator=(const Self &) = delete;

  // Vector of line records
  LinesRecordVectorType m_LinesRecord;

  // Pair for sample valid selection
  RecordType m_SamplesRecord;

  bool m_OnlyValidSample;
 
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarDeburstImageFilter.hxx"
#endif



#endif
