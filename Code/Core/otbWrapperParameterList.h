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
#ifndef __otbWrapperParameterList_h
#define __otbWrapperParameterList_h

#include "itkObject.h"
#include "otbWrapperParameter.h"
#include <vector>

namespace otb
{
namespace Wrapper
{

/**
 * \class WrapperParameterList
 */
class ITK_EXPORT ParameterList
  : public Parameter
{
public:
  typedef ParameterList                        Self;
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
  ParameterList()
  {}

  virtual ~ParameterList()
  {}

private:
  ParameterList(const ParameterList &); //purposely not implemented
  void operator =(const ParameterList&); //purposely not implemented

  std::vector<Parameter::Pointer> m_ParameterList;

};

}
}

#endif
