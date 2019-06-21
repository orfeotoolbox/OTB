/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef otbCastImageFilter_h
#define otbCastImageFilter_h

#if ITK_VERSION_MAJOR >= 5
#include "itkUnaryFunctorImageFilter.h"
#include "itkProgressReporter.h"

#include <type_traits>

namespace otb
{

/** \class CastImageFilter
 *
 * \brief Casts input pixels to output pixel type.
 *
 * This filter is templated over the input image type
 * and the output image type.
 *
 * A typical use is to cast a
   \code
   itk::Image<type1, dim>
   \endcode
 * to a
   \code
   itk::Image<type2, dim>
   \endcode
 *
 * This filter can also be used to cast a
   \code
   itk::VectorImage<type1, dim>
   \endcode
 * to a
   \code
   itk::VectorImage<type2, dim>
   \endcode
 *
 * This class is copied from the itkCastImageFilter, but it doesn't have
 * a static assertion requiring the input and output pixel dimension to match,
 * thus this filter can be used to convert to and from variableLengthVector.
 *
 * If you need to perform a dimensionaly reduction, you may want
 * to use the ExtractImageFilter instead of the CastImageFilter.
 *
 */
template< typename TInputImage, typename TOutputImage >
class ITK_TEMPLATE_EXPORT CastImageFilter:
    public itk::InPlaceImageFilter< TInputImage, TOutputImage >
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(CastImageFilter);

  /** Standard class type aliases. */
  using Self = CastImageFilter;

  using Superclass = itk::InPlaceImageFilter< TInputImage, TOutputImage >;

  using Pointer = itk::SmartPointer< Self >;
  using ConstPointer = itk::SmartPointer< const Self >;


  using OutputImageRegionType = typename Superclass::OutputImageRegionType;

  using InputPixelType = typename TInputImage::PixelType;
  using OutputPixelType = typename TOutputImage::PixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CastImageFilter, InPlaceImageFilter);

protected:
  CastImageFilter();
  virtual ~CastImageFilter() = default;

  void GenerateOutputInformation() override;

  void GenerateData() override;

  void DynamicThreadedGenerateData(const OutputImageRegionType & outputRegionForThread) override;

  template<typename TInputPixelType>
    void DynamicThreadedGenerateDataDispatched(const OutputImageRegionType & outputRegionForThread, std::true_type isConvertible);

  template<typename TInputPixelType>
    void DynamicThreadedGenerateDataDispatched(const OutputImageRegionType & outputRegionForThread, std::false_type isConvertible);
};
} // end namespace otb

#endif

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCastImageFilter.hxx"
#endif

#endif // ITK_VERSION_MAJOR
