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
class ITK_EXPORT ParameterList : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef ParameterList                        Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  void AddParameter();

  Parameter* GetParameter(Key& key);

protected:

  std::vector<Parameter::Pointer> m_ParameterList;

};

}
}

#endif // __otbWrapperParameter_h
