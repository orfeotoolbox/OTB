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

#ifndef otbImageToNoDataMaskFilter_h
#define otbImageToNoDataMaskFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbNoDataHelper.h"

namespace otb
{
namespace Functor
{
/** \class NoDataFunctor
 *  \brief Functor used by ImageToNoDataMaskFilter
 *
 *  See ImageToNoDataMaskFilter for complete documentation.
 *  \ingroup OTBImageManipulation
 */
template <typename TInputPixel, typename TOutputPixel>
class NoDataFunctor
{
public:
  NoDataFunctor() : m_Flags(), m_Values(), m_OutsideValue(0), m_InsideValue(1), m_NaNIsNoData(false)
  {
  }
  virtual ~NoDataFunctor()
  {
  }

  inline TOutputPixel operator()(const TInputPixel& in) const
  {
    return otb::IsNoData(in, m_Flags, m_Values, m_NaNIsNoData) ? m_OutsideValue : m_InsideValue;
  }

  std::vector<bool>   m_Flags;
  std::vector<double> m_Values;
  TOutputPixel        m_OutsideValue;
  TOutputPixel        m_InsideValue;
  bool                m_NaNIsNoData;
};

} // End namespace Functor


/** \class ImageToNoDataMaskFilter
 *  \brief Builds a no-data mask image from no-data flags and values
 *
 *  This filter reads the no-data flags (a boolean vector indicating
 *  for each band if a no-data value exists) and values (the actual
 *  value to be used as no-data for each band) from
 *  MetaDataDictionary, and builds a binary mask indicating presence
 *  or absence of no-data for each pixel.
 *
 *  If NaNIsNoData is true, NaN pixels will also be considered as
 *  no-data pixels.
 *
 * \ingroup Streamed
 * \ingroup MultiThreaded
 * \ingroup OTBImageManipulation
 */
template <typename TInputImage, typename TOutputImage>
class ImageToNoDataMaskFilter
    : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::NoDataFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  typedef Functor::NoDataFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;

  typedef ImageToNoDataMaskFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SpectralAngleDistanceImageFilter, itk::ImageToImageFilter);

  /**
   * Set inside value of output mask. This value will be used to
   * indicate absence of no-data for the pixel in the output mask
   */
  void SetInsideValue(const typename TOutputImage::PixelType& value)
  {
    this->GetFunctor().m_InsideValue = value;
  }
  /**
   * Set outside value of output mask. This value will be used to
   * indicate presence of no-data for the pixel in the output mask
   */
  void SetOutsideValue(const typename TOutputImage::PixelType& value)
  {
    this->GetFunctor().m_OutsideValue = value;
  }
  /**
   * Set the NaN is no data flags
   * \param nanIsNoData If true, NaN values will be considered as
   * no-data as well (default is false)
   */
  void SetNaNIsNoData(bool nanIsNoData)
  {
    this->GetFunctor().m_NaNIsNoData = nanIsNoData;
  }

protected:
  ImageToNoDataMaskFilter()
  {
  }

  ~ImageToNoDataMaskFilter() override
  {
  }

  void BeforeThreadedGenerateData() override
  {
    std::vector<bool>   noDataValueAvailable;
    std::vector<double> noDataValues;

    ReadNoDataFlags(this->GetInput()->GetMetaDataDictionary(), noDataValueAvailable, noDataValues);

    // don't pass empty containers to the functor
    if (noDataValueAvailable.empty() || noDataValues.empty())
    {
      const TInputImage* input = this->GetInput();
      noDataValueAvailable.assign(input->GetNumberOfComponentsPerPixel(), false);
      noDataValues.assign(input->GetNumberOfComponentsPerPixel(), 0.0);
    }

    this->GetFunctor().m_Flags  = noDataValueAvailable;
    this->GetFunctor().m_Values = noDataValues;
  }

private:
  ImageToNoDataMaskFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // End namespace otb


#endif
