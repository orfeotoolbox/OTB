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
#ifndef __otbWrapperDirectoryParameter_h
#define __otbWrapperDirectoryParameter_h

#include "otbWrapperStringParameter.h"
#include "otbMacro.h"

#include <string>

namespace otb
{
namespace Wrapper
{

/** \class DirectoryParameter
 *  \brief This class represent a string parameter for the wrapper framework
 */
class ITK_ABI_EXPORT DirectoryParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef DirectoryParameter            Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(DirectoryParameter, Parameter);

  bool HasValue() const
  {
    return m_StringParam->HasValue();
  }

  // Set/Get Value
  otbSetObjectMemberMacro(StringParam, Value , std::string);
  otbGetObjectMemberMacro(StringParam, Value , std::string);

  // Clear Value
  void ClearValue()
  {
    m_StringParam->ClearValue();
  }

  // Reimplement the SetActive method
  void SetActive(  const bool value )
  {
    Superclass::SetActive( value );
    m_StringParam->SetActive( value );
  }

  // GetActive method
  otbGetObjectMemberConstMacro(StringParam, Active, bool);

protected:
  /** Constructor */
  DirectoryParameter()
  {
    m_StringParam = StringParameter::New();
  }

  /** Destructor */
  virtual ~DirectoryParameter()
  {}

private:
  DirectoryParameter(const DirectoryParameter &); //purposely not implemented
  void operator =(const DirectoryParameter&); //purposely not
                                              //implemented

  StringParameter::Pointer   m_StringParam;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
