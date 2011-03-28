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

// We'll see if we need more than that later...
typedef std::vector<Parameter::Pointer> ParameterList;

/**
 * \class WrapperParameterList
 */

/*
class ITK_EXPORT ParameterList : public itk::Object
{
public:
  typedef ParameterList                        Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  typedef Parameter                            ParameterType;

  itkNewMacro(Self);

  itkTypeMacro(ParameterList,itk::Object);

  void AddParameter();

  ParameterType * GetParameter(std::string & key);

  std::vector<Parameter::Pointer>& GetList();

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
*/
}
}

#endif // __otbWrapperParameter_h
