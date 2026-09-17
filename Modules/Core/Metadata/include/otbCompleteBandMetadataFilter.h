/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCompleteBandMetadataFilter_h
#define otbCompleteBandMetadataFilter_h

#include "OTBMetadataExport.h"
#include "itkInPlaceImageFilter.h"
#include "otbImageMetadata.h"
#include "otbNoDataHelper.h"
#include "otbNewMacro.h"
#include <string>
#include <vector>
#include <functional>

namespace otb
{

namespace MetaData
{

using NewBands = std::vector<std::string>;

}  // MetaData namespace

/**
 * In-place Image Filter that updates band meta-data information.
 * Pixels are un-altered by this image filter. However,
 * `GenerateOutputInformation()` is overridden is order to update image
 * metadata.
 *
 * This filters will permit to set nodata value and band names.
 * \tparam TInputImage  Type of the input image
 * \tparam TOutputImage Type of the output image -- expected to match `TInputImage`.
 *
 * \note Unlike most other filters, this filter has a constructor with
 * parameters that is called through `New()` overload that forwards its
 * parameters.
 */
template <typename TInputImage, typename TOutputImage = TInputImage>
class OTBMetadata_EXPORT CompleteBandMetadataFilter
: public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /**\name Standard class typedefs */
  //@{
  using Self         = CompleteBandMetadataFilter;
  using Superclass   = itk::InPlaceImageFilter<TInputImage, TOutputImage>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<Self const>;
  //@}

  /**\name Image typedef support */
  //@{
  using InputImageType          = typename Superclass::InputImageType;
  using InputImagePointer       = typename Superclass::InputImagePointer;
  using InputImageConstPointer  = typename Superclass::InputImageConstPointer;

  using RegionType              = typename Superclass::InputImageRegionType;
  using IndexType               = typename RegionType::IndexType;
  using SizeType                = typename RegionType::SizeType;

  using InputPixelType          = typename InputImageType::PixelType;

  using OutputImageType         = typename Superclass::OutputImageType;
  using OutputImagePointer      = typename OutputImageType::Pointer;
  using OutputImageConstPointer = typename OutputImageType::ConstPointer;

  using OutputImageRegionType   = typename OutputImageType::RegionType;
  //@}

  otbNewMacro(CompleteBandMetadataFilter);
  itkTypeMacro(CompleteBandMetadataFilter, unused);

protected:
  CompleteBandMetadataFilter() = delete;

  // As std::initializer_list cannot be forwarded easily (and because ITK/OTB
  // `New()` intanciation model, let's introduce a constructor based on
  // variadic template parameters
  template <typename... Strings>
  CompleteBandMetadataFilter(
      double dst_nodata,
      Strings&&... band_names)
  : m_dst_nodata(dst_nodata)
  , m_band_names({band_names...})
  {}

  template <typename ExtraOperation>
  CompleteBandMetadataFilter(
      double             dst_nodata,
      MetaData::NewBands band_names,
      ExtraOperation     extra_operation
  )
  : m_dst_nodata(dst_nodata)
  , m_band_names(std::move(band_names))
  , m_extra_operation(extra_operation)
  {}

  ~CompleteBandMetadataFilter() = default;

  /**
   * Register a new band with the specified name.
   * This function can be used in the situations where the exact bands are not
   * known before hand, but registered one and the other.
   * \param[in] name  name of the new band
   */
  void RegisterNewBand(std::string name) { m_band_names.push_back(std::move(name)); }

  /**
   * Specialization of `GenerateOutputInformation()` to update the metadata.
   */
  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation();

    InputImageConstPointer inputPtr  = this->GetInput();
    OutputImagePointer     outputPtr = this->GetOutput();

    assert(inputPtr);
    assert(outputPtr);
    assert(outputPtr->GetNumberOfComponentsPerPixel() == m_band_names.size());

    auto & meta = inputPtr->GetImageMetadata();

    if (m_extra_operation)
    {
      m_extra_operation(meta);
    }

    BandInformation bands(meta, BandInformation::clear);
    for (auto const& name : m_band_names)
    {
      bands.add_new(name, m_dst_nodata);
    }

    outputPtr->SetImageMetadata(std::move(meta));
  }

  // We still need to override these functions, to do nothing within them.
  void ThreadedGenerateData(OutputImageRegionType const&, itk::ThreadIdType) override
  {}
  void DynamicThreadedGenerateData(OutputImageRegionType const&) override
  {}

private:
  double                              m_dst_nodata;
  std::vector<std::string>            m_band_names;
  std::function<void(ImageMetadata&)> m_extra_operation;
};

}  // otb namespace

#endif  // otbCompleteBandMetadataFilter_h
