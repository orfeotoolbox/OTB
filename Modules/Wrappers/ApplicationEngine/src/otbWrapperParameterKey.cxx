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
#include "otbWrapperParameterKey.h"

#include <boost/algorithm/string.hpp>

namespace otb
{
namespace Wrapper
{

ParameterKey::ParameterKey() : m_Key("")
{
}

ParameterKey::ParameterKey( const char * val )
{
  this->SetKey(std::string(val));
}

ParameterKey::ParameterKey( const std::string & val )
{
  this->SetKey(val);
}

ParameterKey::~ParameterKey()
{
}

std::string
ParameterKey::GetLastElement()
{
  return m_Key.substr( m_Key.find_last_of(".")+1, m_Key.size() );
}

std::string
ParameterKey::GetFirstElement()
{
  return m_Key.substr( 0, m_Key.find_first_of(".") );
}

std::string
ParameterKey::GetRoot()
{
  return m_Key.substr( 0, m_Key.find_last_of(".") );
}


std::vector<std::string>
ParameterKey::Split()
{
  std::vector<std::string> res;
  boost::split(res, m_Key, boost::is_any_of("."), boost::token_compress_on);

  return res;
}

void
ParameterKey::Append( const std::string & val )
{
  m_Key.append(".");
  m_Key.append(val);
}


void
ParameterKey::Append( const ParameterKey & pKey )
{
  this->Append( pKey.GetKey() );
}

} // end namespace Wrapper
} //end namespace otb
