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

#ifndef otbJoinContainer_h
#define otbJoinContainer_h

// #include <iosfwd>
#include <string>

#include <ostream>
// #include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/empty.hpp>
namespace otb
{
/**\ingroup Common
 * Joins elements from a range into a stream.
 * \tparam TRange range type, see <a
 * href="www.boost.org/doc/libs/release/libs/range/doc/html/">boost.Range</a>
 * to see what qualifies as an \em Range.
 * \param os  destination stream
 * \param[in] range  Range to print
 * \param[in] separator  Separator string to use between elements.
 *
 * \return the stream
 * \throw None At least, this function is exception neutral.
 */
template <typename TRange>
inline std::ostream& Join(std::ostream& os, TRange const& range, std::string const& separator)
{
  if (!boost::empty(range))
  {
    typename boost::range_iterator<TRange const>::type       first = boost::begin(range);
    typename boost::range_iterator<TRange const>::type const last  = boost::end(range);
    os << *first;
    for (++first; first != last; ++first)
    {
      os << separator << *first;
    }
  }
  return os;
}
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbJoinContainer.hxx"
#endif

#endif // otbJoinContainer_h
