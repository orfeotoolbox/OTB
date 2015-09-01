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
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>


namespace boost {

/* I want raw performance and I don't want to use strtod, atoi at different
* places. so here we are. */
template<>
inline int lexical_cast(const std::string& arg)
{
  char* stop;
  int res = std::strtol( arg.c_str(), &stop, 10);
    if ( *stop != 0 ) throw_exception(bad_lexical_cast(typeid(int), typeid(std::string)));
    return res;
}

template<>
inline float lexical_cast(const std::string& arg)
{
    char* stop;
    float res = std::strtof( arg.c_str(), &stop);
    if ( *stop != 0 ) throw_exception(bad_lexical_cast(typeid(float), typeid(std::string)));
    return res;
}

template<>
inline double lexical_cast(const std::string& arg)
{
    char* stop;
    double res = std::strtod( arg.c_str(), &stop);
    if ( *stop != 0 ) throw_exception(bad_lexical_cast(typeid(double), typeid(std::string)));
    return res;
}


/* there could also be specialised vice-versa conversion below */
// template<>
// inline std::string lexical_cast(const int& arg)
// {
//     char buffer[20]; // large enough for arg < 2^200

//     ltoa( arg, buffer, 10 );
//     return std::string( buffer ); // RVO will take place here
// }

}//namespace boost


namespace otb
{
namespace Utils
{

template<typename T>
void split(std::string const& s, char const* delims, T& ret)
{
  T output;
  char const* p = s.c_str();
  char const* q = strpbrk(p+1, delims);
  for( ; q != NULL; q = strpbrk(p, delims) )
  {
    output.push_back(typename T::value_type(p, q));
    p = q + 1;
  }

  output.swap(ret);
}

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
  std::vector<std::string> splitted;

  boost::split(splitted, str, boost::is_any_of(delims));

//  split(str, delims, splitted);

  for(size_t i = 0; i < splitted.size(); i++)
    {
    ret.push_back(boost::lexical_cast<typename T::value_type> (splitted[i]));
    }

}

} // end namespace Utils
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbStringUtils.txx"
#endif

#endif //otbStringUtils_h
