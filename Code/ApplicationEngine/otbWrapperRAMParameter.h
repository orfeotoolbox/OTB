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
#ifndef __otbWrapperRAMParameter_h
#define __otbWrapperRAMParameter_h

#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class RAMParameter
 *  \brief This class represent a RAM parameter for the wrapper framework
 */
class RAMParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef RAMParameter                  Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(RAMParameter, Parameter);

  /** typedef */
  typedef NumericalParameter<unsigned int>     UnsignedIntParameter;
  typedef UnsignedIntParameter::ScalarType     ScalarType;
  
  bool HasValue() const
  {
    return m_UnsignedIntParam->HasValue();
  }

  // Set/Get Value
  otbSetObjectMemberMacro(UnsignedIntParam, Value , ScalarType);
  otbSetObjectMemberMacro(UnsignedIntParam, Value , std::string);
  otbGetObjectMemberMacro(UnsignedIntParam, Value , ScalarType);

  // Set/Get extremums
  otbSetObjectMemberMacro(UnsignedIntParam,  DefaultValue, ScalarType);
  otbGetObjectMemberMacro(UnsignedIntParam,  DefaultValue, ScalarType);
  otbSetObjectMemberMacro(UnsignedIntParam,  MinimumValue, ScalarType);
  otbGetObjectMemberMacro(UnsignedIntParam,  MinimumValue, ScalarType);
  otbSetObjectMemberMacro(UnsignedIntParam,  MaximumValue, ScalarType);
  otbGetObjectMemberMacro(UnsignedIntParam,  MaximumValue, ScalarType);

  // Clear Value
  void ClearValue()
  {
    m_UnsignedIntParam->ClearValue();
  }

  // Reset
  virtual void Reset()
  {
    m_UnsignedIntParam->Reset();
  }

  // Reimplement the SetActive method
  void SetActive(  const bool value )
  {
    Superclass::SetActive( value );
    m_UnsignedIntParam->SetActive( value );
  }

  otbGetObjectMemberConstMacro(UnsignedIntParam, Active, bool);

protected:
  /** Constructor */
  RAMParameter()
  {
    this->SetName("RAM");
    this->SetDescription("Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default).");
    this->SetKey("ram");

    // Initialize the unsigned int NumericalParam
    m_UnsignedIntParam = UnsignedIntParameter::New();

    // 0 RAM is not allowed, make the minimum to 1 by default
    this->SetMinimumValue(1);
  }

  /** Destructor */
  virtual ~RAMParameter()
  {}

private:
  RAMParameter(const RAMParameter &); //purposely not implemented
  void operator =(const RAMParameter&); //purposely not implemented

  UnsignedIntParameter::Pointer         m_UnsignedIntParam;
};

} // End namespace Wrapper
} // End namespace otb

#endif
