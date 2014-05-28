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
#ifndef __otbWrapperOutputFilenameParameter_h
#define __otbWrapperOutputFilenameParameter_h

#include <string>
#include "otbWrapperStringParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class OutputFilenameParameter
 *  \brief This class represent a string parameter for the wrapper framework
 */
class ITK_ABI_EXPORT OutputFilenameParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputFilenameParameter        Self;
  typedef StringParameter               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputFilenameParameter, Parameter);


  bool HasValue() const
  {
    return m_StringParam->HasValue();
  }

  // Get Value
  otbGetObjectMemberMacro(StringParam, Value , std::string);

  // Set Value
  virtual void SetValue(const std::string value)
    {
    itkDebugMacro("setting member m_StringParam to " << value);
    this->m_StringParam->SetValue(value);
    SetActive(true);
    this->Modified();
    }

  // Clear Value
  void ClearValue()
  {
    m_StringParam->ClearValue();
  }

protected:
  /** Constructor */
  OutputFilenameParameter()
  {
    // Initialize the String Parameter
    m_StringParam= StringParameter::New();


  }

  /** Destructor */
  virtual ~OutputFilenameParameter()
  {}

private:
  OutputFilenameParameter(const OutputFilenameParameter &); //purposely not implemented
  void operator =(const OutputFilenameParameter&); //purposely not
                                                  //implemented

  StringParameter::Pointer m_StringParam;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
