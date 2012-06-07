/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOGRHelpers_h
#define __otbOGRHelpers_h

#include <string>
#include <vector>
#include <cassert>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#if ! defined(NDEBUG)
#include "cpl_string.h" // CSLCount
#endif

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
 */
struct StringListConverter
{
  template <class ContainerType>
  explicit StringListConverter(ContainerType const& strings)
    {
    BOOST_MPL_ASSERT((boost::is_same<typename ContainerType::value_type, std::string>));
    m_raw.reserve(boost::size(strings)+1);
    for (typename ContainerType::const_iterator b = boost::begin(strings), e = boost::end(strings); b != e; ++b)
      {
      m_raw.push_back(b->c_str());
      }
    m_raw.push_back(0);
    assert(CSLCount(const_cast <char**>(&m_raw[0])) == boost::size(strings));
    }
  char ** to_ogr() const
    {
    return const_cast <char**>(&m_raw[0]);
    }
private:
  std::vector<char const*> m_raw;
};

} // ogr namespace
} // end namespace otb

#endif // __otbOGRHelpers_h
