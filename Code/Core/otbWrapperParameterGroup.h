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

  typedef Parameter                            ParameterType;

  itkNewMacro(Self);

  itkTypeMacro(ParameterList,Parameter);

  void AddParameter(ParameterType::Pointer p)
  {
    m_ParameterList.push_back(p);
  }

  ParameterType::Pointer GetParameter(unsigned int i)
  {
    return m_ParameterList[i];
  }

  unsigned int GetNumberOfParameters()
  {
    m_ParameterList.size();
  }

protected:
  ParameterGroup()
  {}

  virtual ~ParameterGroup()
  {}

  std::vector<Parameter::Pointer> m_ParameterList;

private:
  ParameterGroup(const ParameterGroup &); //purposely not implemented
  void operator =(const ParameterGroup&); //purposely not implemented

};

}
}

#endif
