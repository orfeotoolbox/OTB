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

#ifndef otbOGRHelpers_h
#define otbOGRHelpers_h

#include "itkMacro.h"
#include <string>
#include <vector>
#include <cassert>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "ogr_feature.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/range/size.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/range/size.hpp>
#endif
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#if !defined(NDEBUG)
#include "cpl_string.h" // CSLCount
#endif

#include "OTBGdalAdaptersExport.h"

class GDALDataset;

namespace otb
{
namespace ogr
{
/**\ingroup gGeometry
 * \class StringListConverter
 * \brief Helper class to convert a set of standard C++ string into <tt>char**</tt> as OGR API expects.
 * <p>Usage:</p>
 * \code
 * std::vector<std::string> options;
 * options.push_back(....);
 * new otb::ogr::DataSource("name.shp", StringListConverter(options).to_ogr());
 * \endcode
 * \since OTB v 3.14.0
 * \todo Have \c DataSource constructor receive a
 * <tt>std::vector<std::string></tt>, or even more precide types.
 * \internal
 * This helper is implemented as a class (and in consequence as an object) in
 * order to ensure the temporary vector of <tt>char*</tt> survives the call of
 * \c to_ogr(). Even when used as suggested by the example above, the vector
 * survives till the <tt>; </tt> at the end of the instruction.
 *
 * \ingroup OTBGdalAdapters
 */
struct OTBGdalAdapters_EXPORT StringListConverter
{

  template <class ContainerType>
  /**
   * Init constructor.
   * \tparam    ContainerType Any container matching C++ convention for
   * standard containers (have a \c value_type, \c begin(), \c end()).
   * \param[in] strings  list of strings to convert
   *
   * \throw std::bad_alloc
   * Prepares \c m_raw with to contain a list of strings that can be seen as a
   * 0-terminated array of <tt>char*</tt>.
   */
  explicit StringListConverter(ContainerType const& strings)
  {
    BOOST_MPL_ASSERT((boost::is_same<typename ContainerType::value_type, std::string>));
    m_raw.reserve(boost::size(strings) + 1);
    for (typename ContainerType::const_iterator b = boost::begin(strings), e = boost::end(strings); b != e; ++b)
    {
      m_raw.push_back(b->c_str());
    }
    m_raw.push_back(nullptr);
    assert(CSLCount(const_cast<char**>(&m_raw[0])) == static_cast<int>(boost::size(strings)));
  }
  /**
   * Access to the OGR compliant list of strings.
   */
  char** to_ogr() const
  {
    return m_raw.size() == 1 ? nullptr : const_cast<char**>(&m_raw[0]);
  }

private:
  std::vector<char const*> m_raw;
};

/**
  * Return the list of available drivers.
  *
  * \return A vector of string containing the list of available drivers.
*/
OTBGdalAdapters_EXPORT std::vector<std::string> GetAvailableDriversAsStringVector();

/**
  * Return the list of files composing the dataset.
  *
  * \param dataset Pointer to the dataset to get the file list from. Will not be
  * checked for null pointer.
  *
  * \return A vector of string containing the list of files.
*/

OTBGdalAdapters_EXPORT std::vector<std::string> GetFileListAsStringVector(GDALDataset* dataset);

/**
  * Returns true if the field 'index' is set and not-null in the given feature
  *
  * Before gdal 2.2, it calls OGRFeature::IsFieldSet().
  * After gdal 2.2, it calls OGRFeature::IsFieldSetAndNotNull()
*/
OTBGdalAdapters_EXPORT bool IsFieldSetAndNotNull(OGRFeature* feat, int index);

} // ogr namespace
} // end namespace otb

#endif // otbOGRHelpers_h
