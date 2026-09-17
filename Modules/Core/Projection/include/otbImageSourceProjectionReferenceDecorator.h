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

#ifndef otbImageSourceProjectionReferenceDecorator_h
#define otbImageSourceProjectionReferenceDecorator_h

#include "OTBProjectionExport.h"
#include "otbCoordinateTransformation.h"
#include "otbMacro.h"
#include <itkMacro.h>
#include <itkSmartPointer.h>
#include <utility>

namespace otb
{

namespace details
{
/**
 * Internal implementation class for defining helper decorator classes that propagates pixel
 * coordinates transformation in WGS84 lon/lat to image sources or image filters.
 *
 * \tparam TImageProducer  An ITK image source, or an image filter.
 * \tparam TActual         Actual type, injected through CTRP. It's expected to provide a
 *                         `GetProjectionRef` member function.
 *
 * This class takes care of converting efficiently the conversion of pixel coordinates.
 */
template <typename TImageProducer, typename TActual>
class OTBProjection_EXPORT_TEMPLATE ProjectionReferenceDecorator
: public TImageProducer
{
public:

  /**\name Standard class typedefs */
  //@{
  using Self                  = ProjectionReferenceDecorator;
  using Superclass            = TImageProducer;
  using Pointer               = itk::SmartPointer<Self>;
  using ConstPointer          = itk::SmartPointer<Self const>;

  using OutputImageType       = typename TImageProducer::OutputImageType;

  using OutputImagePointer    = typename OutputImageType::Pointer;
  using SpacingType           = typename OutputImageType::SpacingType;
  using SizeType              = typename OutputImageType::SizeType;
  using PointType             = typename OutputImageType::PointType;
  using IndexType             = typename OutputImageType::IndexType;
  using PixelType             = typename OutputImageType::PixelType;
  using OutputImageRegionType = typename Superclass::OutputImageRegionType;
  //@}

protected:
  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Unused);

protected:
  /**
   * Updates spatial reference in output image.
   */
  void GenerateOutputInformation() override;

  /**
   * Specialized loop meant to be called from leaf specialization of `ThreadedGenerateData().
   * This functions takes care of line-wise iteration over the current output region, and of
   * converting pixel coordinates into WGS84 lon/lat coordinates, efficiently.
   *
   * \tparam TCommandOnLine   The command applied once per line, before iterating on the pixels.
   * It's expected to be compatible with the following signature:
   * `void (lonlat_view_t, ScanLineIterator out_it, ScanLineIterator in_its...)`
   *
   * \tparam TCommandOnPixel  The command applied on pixel is expected to be compatible with
   * `void (Point2DType lon_lat, ptrdiff_t col_index, ScanLineIterator out_it, ScanLineIterator in_its...)`.
   *
   * \tparam TOtherIterators  Compatible to `ScanLineIterator`
   *
   * \param[in]     do_on_line      Command to execute once per line, before iteration on pixels
   * \param[in]     do_on_pixel     Command to execute on the current pixel
   * \param[in]     outputRegionForThread Output region describing the tile
   * \param[in]     threadId        Current thread id (ITK 4.x)
   * \param[in,out] other_iterators Variadic list of iterators in case we need to loop over multiple
   *                                images simultaneously.
   *
   * \throw Neutral throws whatever `do_on_pixel` throws
   *
   * __Usage:__
   *
   * @code
   *
   * template <typename TImageIn, typename TImageOut>
   * class SARComputeGroundAndSatPositionsOnDEMImageFilter
   * : public AddExtraMetaDecorator<ImageFilterProjectionReferenceDecorator<itk::ImageToImageFilter<TImageIn, TImageOut>>>
   * { ... };
   *
   * template <class TImageIn, class TImageOut>
   * void
   * SARComputeGroundAndSatPositionsOnDEMImageFilter<TImageIn, TImageOut>
   * ::ThreadedGenerateData(
   *     ImageOutRegionType const& outputRegionForThread,
   *     itk::ThreadIdType threadId
   * )
   * {
   *   ImageInRegionType inputRegionForThread = static_cast<ImageInRegionType>(outputRegionForThread);
   *
   *   using InputIterator                    = itk::ImageScanlineConstIterator<ImageInType>;
   *   InputIterator InIt(this->GetInput(), inputRegionForThread);
   *
   *   using OutputIterator = itk::ImageScanlineIterator<ImageOutType>;
   *
   *   auto compute_pixel = [&, srcnodata = m_srcNoData](
   *       Point2DType lonlat, std::ptrdiff_t idx, OutputIterator& OutIt, InputIterator const& InIt)
   *   {
   *     // Compute pixelOutput from:
   *     // - InIt.Get()
   *     // - lonlat
   *     // - pixel column index in the line
   *     // - captured context
   *     ...
   *     OutIt.Set(pixelOutput);
   *   };
   *
   *   this->DoGenerateTileDataLineWise(compute_pixel, outputRegionForThread, threadId, InIt);
   * }
   * @endcode
   */
  template <typename TCommandOnLine, typename TCommandOnPixel, typename... TOtherIterators>
  void DoGenerateTileDataLineWise(
      TCommandOnLine               do_on_line,
      TCommandOnPixel              do_on_pixel,
      OutputImageRegionType const& outputRegionForThread,
      itk::ThreadIdType            threadId,
      TOtherIterators        &&... other_iterators
  );

  /// \overload
  template <typename TCommandOnPixel, typename... TOtherIterators>
  void DoGenerateTileDataLineWise(
      TCommandOnPixel              do_on_pixel,
      OutputImageRegionType const& outputRegionForThread,
      itk::ThreadIdType            threadId,
      TOtherIterators        &&... other_iterators
  )
  {
    DoGenerateTileDataLineWise(
        [](auto&&...){},
        do_on_pixel,
        outputRegionForThread,
        threadId,
        std::forward<TOtherIterators>(other_iterators)...
    );
  }

  // TODO: The day otb::CoordinateTransformation exposes batch transformation,
  // and SpatialReference constructor accepts pointers, we could have directly a
  // vector<CoordinateTransformation>
  // => For the moment, use the right unique_ptr type.
  std::vector<CoordinateTransformation::CoordinateTransformationPtr> m_tlsCTs;
};

}  // details namespace

/**
 * Helper decorator class that propagates pixel coordinates transformation in WGS84 lon/lat to
 * image sources.
 * \tparam TImageSource  An ITK image source.
 *
 * This specialization uses the output projection reference set from the client `otb::Application`
 *
 * This class takes care of converting efficiently the conversion of pixel coordinates.
 * \warning This class requires the filter to be thread aware and thus rely on old ITK 4 API.
 *
 * \sa `otb::ImageFilterProjectionReferenceDecorator`
 */
template <typename TImageSource>
class OTBProjection_EXPORT_TEMPLATE ImageSourceProjectionReferenceDecorator
: public details::ProjectionReferenceDecorator
< TImageSource
, ImageSourceProjectionReferenceDecorator<TImageSource>>
{
public:

  /**\name Standard class typedefs */
  //@{
  using Self                  = ImageSourceProjectionReferenceDecorator;
  using Superclass            = details::ProjectionReferenceDecorator
                              < TImageSource
                              , ImageSourceProjectionReferenceDecorator<TImageSource>>;
  using Pointer               = itk::SmartPointer<Self>;
  using ConstPointer          = itk::SmartPointer<Self const>;

  using OutputImageType       = typename TImageSource::OutputImageType;

  using OutputImagePointer    = typename OutputImageType::Pointer;
  using SpacingType           = typename OutputImageType::SpacingType;
  using SizeType              = typename OutputImageType::SizeType;
  using PointType             = typename OutputImageType::PointType;
  using IndexType             = typename OutputImageType::IndexType;
  using PixelType             = typename OutputImageType::PixelType;
  using OutputImageRegionType = typename TImageSource::OutputImageRegionType;
  //@}

protected:
  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Unused);

private:
  std::string m_OutputProjectionRef;

public:
  otbGetMacro(OutputProjectionRef);
  otbSetMacro(OutputProjectionRef);
};

/**
 * Helper decorator class that propagates pixel coordinates transformation in WGS84 lon/lat to
 * image filters.
 * \tparam TImageFilter  An ITK image filter.
 *
 * This specialization uses the output projection reference found in the (first) input image.
 *
 * This class takes care of converting efficiently the conversion of pixel coordinates.
 * \warning This class requires the filter to be thread aware and thus rely on old ITK 4 API.
 *
 * \sa `otb::ImageSourceProjectionReferenceDecorator`
 */
template <typename TImageFilter>
class OTBProjection_EXPORT_TEMPLATE ImageFilterProjectionReferenceDecorator
: public details::ProjectionReferenceDecorator
< TImageFilter
, ImageFilterProjectionReferenceDecorator<TImageFilter>>
{
public:

  /**\name Standard class typedefs */
  //@{
  using Self                  = ImageFilterProjectionReferenceDecorator;
  using Superclass            = details::ProjectionReferenceDecorator
                              < TImageFilter
                              , ImageFilterProjectionReferenceDecorator<TImageFilter>>;

  using Pointer               = itk::SmartPointer<Self>;
  using ConstPointer          = itk::SmartPointer<Self const>;

  using OutputImageType       = typename TImageFilter::OutputImageType;

  using OutputImagePointer    = typename OutputImageType::Pointer;
  using SpacingType           = typename OutputImageType::SpacingType;
  using SizeType              = typename OutputImageType::SizeType;
  using PointType             = typename OutputImageType::PointType;
  using IndexType             = typename OutputImageType::IndexType;
  using PixelType             = typename OutputImageType::PixelType;
  using OutputImageRegionType = typename TImageFilter::OutputImageRegionType;
  //@}

protected:
  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Unused);

public:
  /** Fetches the output projection reference from the input image. */
  std::string GetOutputProjectionRef() const
  {
    auto input = this->GetInput();
    assert(input);
    return input->GetProjectionRef();
  }
};

}  // otb namespace

#ifndef OTB_MANUAL_INSTANTIATION
#  include "otbImageSourceProjectionReferenceDecorator.hxx"
#endif

#endif  // otbImageSourceProjectionReferenceDecorator_h
