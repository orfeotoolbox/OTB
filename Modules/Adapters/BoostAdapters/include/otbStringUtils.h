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
#ifndef otbStringUtils_h
#define otbStringUtils_h

#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/utility/enable_if.hpp>

namespace otb
{
namespace Utils
{
template <typename T>
inline
T LexicalCast(std::string const& in, std::string const& kind) {
    try
    {
        return boost::lexical_cast<T>(in);
    }
    catch (boost::bad_lexical_cast &) {
        std::ostringstream oss;
        oss << "Cannot decode " << in << " as this is not a valid value for " << kind;
        throw std::runtime_error(oss.str());
    }
}

/**\ingroup Utils
 * Converts a delimiter separated string into a collection of \c T's.
 * \tparam Collection type. It shall define \c value_type and \c push_back()
 * \param[out] ret output collection.
 * \param[in] str  input string
 * \param[in] errmsg a msg complement used to build the error message to
be shown if there is \c lexical_cast exception. See \c
otb:Utils::LexicalCast().
 * \param delims  delimitter characters (space is default)
 * \throw std::bad_alloc in case of exhausted memory
 * \throw std::runtime_error in case an element from the input string
cannot be converted into a valid \c T instance.
 * \pre delims shall not be null (untested)
 *
 * \see \c otb::Utils::LexicalCast()
 */
template<typename T>
void
ConvertStringToVector(std::string const &str, T& ret, std::string const& errmsg, const char *  delims=" ")
{
  std::vector< std::string > splitted;

  boost::split(splitted, str, boost::is_any_of(delims));

  for(size_t i = 0; i < splitted.size(); i++)
    {
    typename T::value_type value = LexicalCast<typename T::value_type> (splitted[i], errmsg);
    ret.push_back(value);
    }
}

/**\ingroup Utils
 * split a given std::string of into key value based on given delimitter
 * string. default delimitter is '='. If the string does not have a delimitter
 * the key is set to input string and value is set to defValue.
 * \param[in] str  input string
 * \param key An std::string reference where key will be stored
 * \param value a reference of \cT where value will be stored
 * \param defValue a default value if there is no delimitter fo
 * \param doTrim check to perform boost::trim() ob key and values. Default is true.
 * \param[in] errmsg a msg complement used to build the error message to
be shown if there is \c lexical_cast exception. See \cotb:Utils::LexicalCast().
 * \param delims  delimitter characters (space is default)
 * \throw std::bad_alloc in case of exhausted memory
 * \throw std::runtime_error in case an element from the input string
cannot be converted into a valid \c T instance.
 * \pre delims shall not be null (untested)
 *
 * \see \c otb::Utils::LexicalCast()
 */
template<typename T>
void SplitStringToSingleKeyValue(const std::string& str,
                                 std::string& key, T& value, const T& defValue,
                                 std::string const& errmsg, const std::string delims="=", bool doTrim=true)
{
  std::size_t pos = str.find(delims);
  if (pos == std::string::npos)
    {
    key = str;
    value = defValue;
    }
  key = str.substr(0, pos);

  std::string value_ = str.substr(pos+delims.size(), str.size() - 1 );

  //do optional trim
  if(doTrim)
    {
    boost::trim_left(key);
    boost::trim_right(value_);
    }

  if(typeid(value) != typeid(key))
    {
    value = LexicalCast<T>(value_, errmsg);
    }
  else
    {
    value.swap(value_);
    }
}
} // end namespace Utils
} // end namespace otb

#endif //otbStringUtils_h
