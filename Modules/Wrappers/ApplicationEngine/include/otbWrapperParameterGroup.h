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
#ifndef otbWrapperParameterGroup_h
#define otbWrapperParameterGroup_h

#include "itkObject.h"
#include "otbWrapperParameter.h"
#include <vector>

namespace otb
{
namespace Wrapper
{

/**
 * \class Group
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ParameterGroup
  : public Parameter
{
public:
  typedef ParameterGroup                       Self;
  typedef Parameter                            Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ParameterList, Parameter);

  void AddParameter(Parameter::Pointer p);

  /** Method to add or substitute a parameter in a group. If there is already
   *  a parameter with the given key, the parameter is replaced.
   *  The function returns true on success, false on failure */
  bool SetParameter(Parameter::Pointer p, std::string &key);

  /** Add a new choice value to an existing choice parameter */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Remove choices made in ListViewParameter widget*/
  void ClearChoices(std::string paramKey);

  /** Get the choices made in a ListView Parameter widget*/
  std::vector<int> GetSelectedItems(std::string paramKey);

  /** Add a new parameter to the parameter group
   * the parent key of paramKey can be the path to a parameter group
   * or the path to a choice value */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

  Parameter::Pointer GetParameterByIndex(unsigned int i);

  Parameter::Pointer GetParameterByKey(std::string name);

  /** rashad: Add xml parameters eg: -inxml -outxml */
  void AddInXMLParameter();

  void AddOutXMLParameter();

  void Clear()
  {
    m_ParameterList.clear();
  }

  /** Get the parameter type as string from its ParameterType enum
   * For example if type of parameter is ParameterType_InputImage this
   * function return the string InputImage */
  std::string GetParameterTypeAsString(ParameterType paramType);

  /* Get the parameter type from its string version of ParameterType enum */
  ParameterType GetParameterTypeFromString(std::string paramType);

  unsigned int GetNumberOfParameters();

  std::vector<std::string> GetParametersKeys(bool recursive = true);

  // Always has value
  bool HasValue() const ITK_OVERRIDE
  {
    return true;
  }

  /** Resolve potential proxy parameters by following their targets until
   *  a non-proxy parameter. It will detect cycles and report an error */
  static Parameter* ResolveParameter(Parameter *param);

protected:
  ParameterGroup();
  ~ParameterGroup() ITK_OVERRIDE;

  typedef std::vector<Parameter::Pointer> ParameterListType;
  ParameterListType m_ParameterList;

private:
  ParameterGroup(const ParameterGroup &); //purposely not implemented
  void operator =(const ParameterGroup&); //purposely not implemented

};

}
}

#endif
