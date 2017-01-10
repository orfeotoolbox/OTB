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
template <typename TRange> inline
std::ostream & Join(std::ostream &os, TRange const& range, std::string const& separator)
{
  if (!boost::empty(range))
    {
    typename boost::range_iterator<TRange const>::type       first = boost::begin(range);
    typename boost::range_iterator<TRange const>::type const last = boost::end(range);
    os << *first;
    for (++first; first!=last; ++first)
      {
      os << separator << *first;
      }
    }
  return os;
}
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbJoinContainer.txx"
#endif

#endif // otbJoinContainer_h
