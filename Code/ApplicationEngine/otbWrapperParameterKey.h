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
#ifndef __otbWrapperParameterKey_h
#define __otbWrapperParameterKey_h

#include <string>
#include "otbMacro.h"
#include <itksys/RegularExpression.hxx>


namespace otb
{
namespace Wrapper
{

/** \class Application
 *  \brief This class is a the Parameter key representation.
 *
 *  The constructor accepts a string key constructed as follow : parent.me.child.
 *  It implements some key manipulation as described in the code.
 */
class ITK_ABI_EXPORT ParameterKey
{
public:

  typedef ParameterKey Self;

  ParameterKey();
  ParameterKey( const char * val );
  ParameterKey( const std::string & val );
  virtual ~ParameterKey();


  /** Class Methods used to interface with the registered factories */
  /*
last() : return "tata"
   ** parent() : return "toto.titi"
   ** root() : return "toto"
   ** split() : return vector<string> ("toto", "titi", "tata")
   ** append(std::string)
   ** append(const Key&)
   */

  /** Returns the string after the last point separator */
  std::string GetLastElement();

  /** Returns the string before the first point separator */
  std::string GetFirstElement();

  /** Returns the string before the last point separator */
  std::string GetRoot();

   /** Returns the vector of string that contains each element separated by a point. */
  std::vector<std::string> Split();

  /** Append a string at the end of the key. A point separator will be added before the string. */
  void Append( const std::string & val );

  /** Append a string at the end of the key. A point separator will be added before the string. */
  void Append( const ParameterKey & pKey );

  /** Get Key value */
  std::string GetKey()
    {
      return m_Key;
    }

  /** Get Key value */
  std::string GetKey() const
    {
      return m_Key;
    }

  /** Set Key value */
  void SetKey( const std::string & val )
    {
      // Check chain : lowercase, alphanumerical or "."
      itksys::RegularExpression reg;
      reg.compile("([^0-9a-z\\.])");

      if(!reg.find(val))
        {
          m_Key = val;
        }
      else
        {
          itkGenericExceptionMacro( "Invalid key "<<val<<". Must be in lowercase, containing alphanumerical characters or \".\"");
        }
    }

private:
  ParameterKey(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string m_Key;

};

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperParameterKey_h_
