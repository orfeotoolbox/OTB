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

#ifndef otbRadiometricIndex_h
#define otbRadiometricIndex_h

#include "itkVariableLengthVector.h"
#include "otbBandName.h"
#include <array>
#include <set>
#include <string>
#include <map>
#include <stdexcept>

using namespace otb::BandName;

namespace otb
{
namespace Functor
{
/**
 * \class RadiometricIndex
 * \brief Base class for all radiometric indices
 *
 * This class is the base class for all radiometric indices.
 *
 * It offers services to:
 * - Indicate which band are required among the list provided by
 * TBandNameEnum
 * - Set indices of each required band
 * - Compute the indice response to a pixel by subclassing the pure
 * virtual operator()
 *
 * This class is designed for performance on the critical path. For
 * best performances use the Value() method when implementing
 * operator() to avoid branches.
 *
 * TBandName enum should end with a MAX value that will be used to
 * derive the number of bands.
 *
 * \ingroup OTBIndices
 */
template <typename TInput, typename TOutput, typename TBandNameEnum = CommonBandNames>
class RadiometricIndex
{
public:
  /// Types for input/output
  using InputType  = TInput;
  using PixelType  = itk::VariableLengthVector<InputType>;
  using OutputType = TOutput;

  /// Enum Among which bands are used
  using BandNameType = TBandNameEnum;

  /// The number of bands, derived from the Enum MAX value
  static constexpr size_t NumberOfBands = static_cast<size_t>(BandNameType::MAX);

  static constexpr double Epsilon = 0.0000001;

  /**
   * \param requiredBands the set<TBandNameEnum> of required bands
   * \throw runtime_error if requiredBands contains TBandNameEnum::MAX
   */
  RadiometricIndex(const std::set<BandNameType>& requiredBands) : m_RequiredBands(), m_BandIndices()
  {
    if (requiredBands.find(BandNameType::MAX) != requiredBands.end())
    {
      throw std::runtime_error("TBandNameEnum::MAX can not be used as a required band");
    }

    // Fill the required bands array
    m_RequiredBands.fill(false);
    m_BandIndices.fill(0);

    for (auto b : requiredBands)
    {
      m_RequiredBands[static_cast<size_t>(b)] = true;
    }
  }

  /**
   * \return a set<TBandNameEnum> containing the required bands for
   * this indice.
   */
  std::set<BandNameType> GetRequiredBands() const
  {
    std::set<BandNameType> resp;
    for (size_t i = 0; i < NumberOfBands; ++i)
    {
      if (m_RequiredBands[i])
      {
        resp.insert(static_cast<BandNameType>(i));
      }
    }

    return resp;
  }

  /**
   * \param band The band to set (value in TBandNameEnum)
   * \param index The index of the band to set (starts at 1 for first band)
   * \throw runtime_error if band is TBandNameEnum::MAX
   */
  void SetBandIndex(BandNameType band, size_t index)
  {
    if (band == BandNameType::MAX)
    {
      throw std::runtime_error("Can not set index for TBandNameEnum::MAX");
    }
    m_BandIndices[static_cast<size_t>(band)] = index;
  }

  /**
   * \param indicesMap a std::map<TBandNameEnum,size_t> containing all
   * bands indices to set  (starts at 1 for first band)
   * \throw runtime_error if indicesMap contains TBandNameEnum::MAX
   */
  void SetBandsIndices(const std::map<BandNameType, size_t>& indicesMap)
  {
    for (auto it : indicesMap)
    {
      SetBandIndex(it.first, it.second);
    }
  }

  /**
   * \param band The band for which to retrieve indice
   * \return The indices of the band
   * \throw runtime_error if band is TBandNameEnum::MAX
   */
  size_t GetBandIndex(BandNameType band) const
  {
    if (band == BandNameType::MAX)
    {
      throw std::runtime_error("Can not get index for TBandNameEnum::MAX");
    }
    return m_BandIndices[static_cast<size_t>(band)];
  }

  /**
   * Astract method which will compute the radiometric indice
   * \param input A itk::VariableLengthVector<TInput> holding the
   * pixel values for each band
   * \return The indice value as TOutput  (starts at 1 for first band)
   */
  virtual TOutput operator()(const itk::VariableLengthVector<TInput>& input) const = 0;

protected:
  /**
   * Helper method to retrieve index for band name. With respect to
   * the public method, this method will not throw an exception if
   * TBandNameEnum::MAX is used as a parameter. Since it is meant for
   * internal use in the critical path and not for client code, it
   * will only assert that band is not TBandNameEnum::MAX in debug
   * mode.
   *
   * \param band The band for which to retrieve indice
   * \return The indices of the band
   */
  size_t UncheckedBandIndex(BandNameType band) const
  {
    assert(band != BandNameType::MAX && "Can not retrieve index for band TBandNameEnum::MAX");
    return m_BandIndices[static_cast<size_t>(band)];
  }

  /**
   * Helper method to parse input  itk::VariableLengthVector<TInput>
   * and get the corresponding band value.
   * For instance:
   * \snippet auto red   = this->Value(CommonBandNames::RED,input);
   *
   * As this function is on the critical performance path, no checks
   * are made to see wether this band is really required for this
   * indice. However an assertion will be raised in debug mode.
   *
   * \param band The band for which to retrieve the value
   * \param input A itk::VariableLengthVector<TInput> holding the
   * pixel values for each band
   * \return The value of the band as double
   *
   */
  double Value(BandNameType band, const itk::VariableLengthVector<TInput>& input) const
  {
    assert(m_RequiredBands[band] && "Retrieving value for a band that is not in the required bands list");
    return static_cast<double>(input[UncheckedBandIndex(band) - 1]);
  }

private:
  // Explicitely disable default constructor
  RadiometricIndex() = delete;

  /// An array storing the required status for each band
  using RequiredBandsContainer = std::array<bool, NumberOfBands>;
  RequiredBandsContainer m_RequiredBands;

  /// An array storing the indice for each band
  using BandIndicesContainer = std::array<size_t, NumberOfBands>;
  BandIndicesContainer m_BandIndices;
};

} // namespace Functor
} // End namespace otb

#endif
