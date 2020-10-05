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

#ifndef otbChangeNoDataValueFilter_h
#define otbChangeNoDataValueFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbNoDataHelper.h"

namespace otb
{
namespace Functor
{
/** \class ChangeNoDataFunctor
 *  \brief Functor used by ChangeNoDataValueFilter
 *
 *  See ChangeNoDataValueFilter for complete documentation.
 *  \ingroup OTBImageManipulation
 */
template <typename TInputPixel, typename TOutputPixel>
class ChangeNoDataFunctor
{
public:
  ChangeNoDataFunctor() : m_Flags(), m_Values(), m_NewValues(), m_NaNIsNoData(false)
  {
  }
  virtual ~ChangeNoDataFunctor()
  {
  }

  inline TOutputPixel operator()(const TInputPixel& in) const
  {
    return otb::ChangeNoData(in, m_Flags, m_Values, m_NewValues, m_NaNIsNoData);
  }

  std::vector<bool>   m_Flags;
  std::vector<double> m_Values;
  std::vector<double> m_NewValues;
  bool                m_NaNIsNoData;
};

} // End namespace Functor

/** \class ChangeNoDataValueFilter
 *  \brief Change no-data flags and values and replace them in image
 *
 *  This filter reads the no-data flags (a boolean vector indicating
 *  for each band if a no-data value exists) and values (the actual
 *  value to be used as no-data for each band) from
 *  MetaDataDictionary, and allows changing this value.
 *
 *  The algorithm is the following: for each pixel, for each channel
 *  in the pixel, if there is a no-data value for this channel
 *  (according to no-data flags) and the current channel value equals
 *  to the no-data value set for this channel, then the value is
 *  changed for the new no-data value specified by the
 *  users. Otherwise, value remains untouched.
 *
 *  If NaNIsNoData is true:
 *  - NaN values will be considered as no data and replaced as well
 *  - Output image will have no-data flags and values for all bands
 *
 *  If NaNIsNoData is false and the input has at least one band with no-data
 *  flag and no-data value :
 *  - Band for which input no-data flags is false will remain
 *    untouched
 *  - Output image will have no-data flags and values only for bands
 *    for which input no-data flag is true.
 *
 *  If NaNIsNoData is false and the input has no band with no-data
 *  flag and no-data value :
 *  - Output image will have no-data flags and values for all bands
 *
 * \ingroup Streamed
 * \ingroup MultiThreaded
 * \ingroup OTBImageManipulation
 */
template <typename TInputImage, typename TOutputImage>
class ChangeNoDataValueFilter
    : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                          Functor::ChangeNoDataFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  typedef Functor::ChangeNoDataFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;

  typedef ChangeNoDataValueFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ChangeNoDataValueFilter, itk::ImageToImageFilter);

  /**
   * Set the new no-data values
   * \param newValues The vector of new no-data values (size should be
   * >= to number of bands)
   */
  void SetNewNoDataValues(std::vector<double>& newValues)
  {
    this->GetFunctor().m_NewValues = newValues;
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
  ChangeNoDataValueFilter()
  {
  }

  ~ChangeNoDataValueFilter() override
  {
  }

  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation();

    std::vector<bool>   noDataValueAvailable;
    std::vector<double> noDataValues;

    bool ret = ReadNoDataFlags(this->GetInput()->GetMetaDataDictionary(), noDataValueAvailable, noDataValues);

    if (!ret)
    {
      noDataValueAvailable.resize(this->GetInput()->GetNumberOfComponentsPerPixel(), false);
      noDataValues.resize(this->GetInput()->GetNumberOfComponentsPerPixel(), 0);
    }

    this->GetFunctor().m_Flags  = noDataValueAvailable;
    this->GetFunctor().m_Values = noDataValues;

    std::vector<bool> flags = noDataValueAvailable;

    if ((this->GetFunctor().m_NaNIsNoData) || (!ret))
    {
      flags = std::vector<bool>(flags.size(), true);
    }

    WriteNoDataFlags(flags, this->GetFunctor().m_NewValues, this->GetOutput()->GetMetaDataDictionary());
  }

private:
  ChangeNoDataValueFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // End namespace otb


#endif
