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
#ifndef __otbWrapperParameterGroup_h
#define __otbWrapperParameterGroup_h

#include "itkObject.h"
#include "otbWrapperParameter.h"
#include <vector>

namespace otb
{
namespace Wrapper
{

/**
 * \class Group
 */
class ITK_EXPORT ParameterGroup
  : public Parameter
{
public:
  typedef ParameterGroup                       Self;
  typedef Parameter                            Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ParameterList,Parameter);

  void AddParameter(Parameter::Pointer p);

  /** Add a new choice value to an existing choice parameter */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Add a new parameter to the parameter group
   * the parent key of paramKey can be the path to a parameter group
   * or the path to a choice value */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

  Parameter::Pointer GetParameterByIndex(unsigned int i);

  Parameter::Pointer GetParameterByKey(std::string name);

  unsigned int GetNumberOfParameters();

  std::vector<std::string> GetParametersKeys(bool recursive = true);

protected:
  ParameterGroup();
  virtual ~ParameterGroup();

  typedef std::vector<Parameter::Pointer> ParameterListType;
  ParameterListType m_ParameterList;

private:
  ParameterGroup(const ParameterGroup &); //purposely not implemented
  void operator =(const ParameterGroup&); //purposely not implemented

};

}
}

#endif
