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
#ifndef __otbWrapperApplication_h
#define __otbWrapperApplication_h

#include <string>
#include "itkObject.h"
#include "otbWrapperParameterList.h"

namespace otb
{
namespace wrapper
{

/** \class Application
 *  \brief This class represent an application
 *
 *
 */
class ITK_EXPORT Application : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef Application                          Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(Parameter,itk::LightObject);

  std::string GetName()
  {
    return m_Name;
  }

  std::string GetDescription()
  {
    return m_Description;
  }

  ParameterList* GetParameterList();

  virtual int Execute() = 0;

protected:
  Application();
  virtual ~Application();

  void SetName(std::string s)
  {
    m_Name = s;
  }

  void SetDescription(std::string s)
  {
    m_Description = s;
  }

  std::string            m_Name;
  std::string            m_Description;
  ParameterList::Pointer m_ParameterList;
};


}
}

#endif // __otbWrapperApplicationOptions_h_
