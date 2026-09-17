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

#ifndef otbImageSourceGeometryDecorator_h
#define otbImageSourceGeometryDecorator_h

#include "OTBCommonExport.h"
#include "otbMacro.h"
#include "itkSmartPointer.h"
#include "itkMacro.h"

namespace otb
{

/**
 * Helper decorator class that propagates image geometry from `ApplicationWithoutInputImage` to
 * image sources.
 * \tparam TImageSource  An ITK image source
 *
 * This class takes care of forwarding the 3 following properties from an
 * `ApplicationWithoutInputImage` to an `itk::ImageSource`
 * - Origin
 * - Spacing
 * - Region Size
 *
 * The geometry propagation is done in the decorated `GenerateOutputInformation()` function.
 *
 * __Usage:__
 *
 * @code
 * // A new filter:
 * class ComputeNormalsOnEllipsoidImageSource
 * : public ImageSourceProjectionReferenceDecorator
 * <        ImageSourceGeometryDecorator
 * <        itk::ImageSource<otb::VectorImage<double>>>>
 * { ... };
 *
 * // From an application, just inherit from `ApplicationWithoutInputImage`:
 * class ExtractNormalVectorToEllipsoid
 * : public ApplicationWithoutInputImage<>
 * {
 *   void DoExecute() override
 *   {
 *     this->DoUpdateParameters_NoInputImage();
 *
 *     // can use auto because a GeneratorType::Pointer is returned
 *     auto generator = this->InstantiateImageGenerator<otb::ComputeNormalsOnEllipsoidImageSource>();
 *     ...
 * @endcode
 *
 * \sa `otb::Wrapper::ApplicationWithoutInputImage`
 * \author Luc Hermitte
 * \copyright CS Group
 */
template <typename TImageSource>
class OTBCommon_EXPORT_TEMPLATE ImageSourceGeometryDecorator
: public TImageSource
{
public:
  /**\name Standard class typedefs */
  //@{
  using Self                  = ImageSourceGeometryDecorator;
  using Superclass            = TImageSource;
  using Pointer               = itk::SmartPointer<Self>;
  using ConstPointer          = itk::SmartPointer<Self const>;

  using OutputImageType       = typename TImageSource::OutputImageType;

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
  void GenerateOutputInformation() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  PointType   m_OutputOrigin{0.0};
  SpacingType m_OutputSpacing{0.0};
  SizeType    m_OutputSize{0, 0};

public:
  otbGetMacro(OutputOrigin);
  otbGetMacro(OutputSpacing);
  otbGetMacro(OutputSize);

  otbSetMacro(OutputOrigin);
  otbSetMacro(OutputSpacing);
  otbSetMacro(OutputSize);
};

}  // otb namespace

#ifndef OTB_MANUAL_INSTANTIATION
#  include "otbImageSourceGeometryDecorator.hxx"
#endif


#endif  // otbImageSourceGeometryDecorator_h
