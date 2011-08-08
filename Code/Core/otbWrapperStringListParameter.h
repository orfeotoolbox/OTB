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
#ifndef __otbWrapperStringListParameter_h
#define __otbWrapperStringListParameter_h

#include <string>
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class StringListParameter
 *  \brief This class represent a list of string parameter for the wrapper framework
 */
class StringListParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef StringListParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef std::vector< boost::any >    StringListType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(StringListParameter, Parameter);

  /** Set the value */
  void SetValue( StringListType sList)
  {
    m_Value = sList;
  }

  void AddValue( std::string value)
  {
    m_Value.push_back( value );
  }

  /** Get the value */
  StringListType GetValue() const
  {
    return m_Value;
  }

  /** Get the value */
  std::string GetNthElement( unsigned int i ) const
  {
    if( m_Value.size() < i )
      {
        itkExceptionMacro( "Invalid index "<<i<<" the string list has only "<<m_Value.size()<<" elements...")
      }

    return boost::any_cast<std::string>(m_Value[i]);
  }

  bool HasValue() const
  {
    if (m_Value.empty() == true )
      {
        return false;
      }
    else
      {
        return !m_Value[0].empty();
      }
  }

  void ClearValue()
  {
    m_Value.clear();
  }

protected:
  /** Constructor */
  StringListParameter()
  {}

  /** Destructor */
  virtual ~StringListParameter()
  {}

  StringListType m_Value;

private:
  StringListParameter(const StringListParameter &); //purposely not implemented
  void operator =(const StringListParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
