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
#include <cstdlib>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>


namespace otb
{
namespace Utils
{

/**\ingroup Utils
   * - convert a delimitter seperated string to std::vector of type T
   * \tparam vec - std::vector of type T. Specialized boost::lexical_cast is used
   * \tparam str - input string
   * \tparam delims - delimitter space is default
   * \t param T& ret - std::vector of type T
   */

template<typename T>
void
ConvertStringToVector(std::string const &str, T& ret, const char *  delims=" ")
{
  std::vector< boost::iterator_range<std::string::iterator> > splitted;

  boost::split(splitted, str, boost::is_any_of(delims));

  for(size_t i = 0; i < splitted.size(); i++)
    {
    typename T::value_type value;
    try
      {
      value = boost::lexical_cast<typename T::value_type> (splitted[i]);
      }
    catch (boost::bad_lexical_cast &)
      {

      }
    ret.push_back(value);
    }
}

/*
SplitStringToSingleKeyValue - split a given std::string of into key value based
on given delimitter string. default delimitter is '='. If the string doesnot
have a delimitter the key is set to input string and value is set to defValue.

This function is templated over value type. It uses specialized
boost::lexical_cast if the T is not of type std::string. only primitive types

Arugments are:
  str      - reference to input string of type std::string
  key      - reference to std::string where key will be stored
  value    - reference to T where value will be stored
  defValue - default value if there is no delimitter found. (informally TRUE)
  delims   - const std::string which contains the delimtter used. Default is '='
  doTrim   - option to perform boost::trim() over key and values. Default is true.
*/
template<typename T>
void SplitStringToSingleKeyValue(const std::string& str,
                                 std::string& key, T& value, const T& defValue,
                                 const std::string delims="=", bool doTrim=true)
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
    try
      {
      value = boost::lexical_cast<T>(value_);
      }
    catch (boost::bad_lexical_cast &)
      {

      //std::cerr << "Error getting value" << value_ << "  at " << __FILE__ << ":" << __LINE__ << std::endl;
      }
    }
  else
    {
    value.swap(value_);
    }
}
} // end namespace Utils
} // end namespace otb

#endif //otbStringUtils_h
