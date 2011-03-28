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
#include "otbMacro.h"
#include "itkObject.h"
#include "otbWrapperParameterList.h"
#include "itkObjectFactory.h"

namespace otb
{
namespace Wrapper
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
itkTypeMacro(Application,itk::Object);

/** Set the parameter name */
itkSetStringMacro(Name);

/** Get the parameter name */
itkGetStringMacro(Name);

/** Set the parameter description */
itkSetStringMacro(Description);

/** Get the parameter description */
itkGetStringMacro(Description);

ParameterList* GetParameterList()
          {
  return m_ParameterList;
          }

virtual int Execute() {return 0;}

protected:
/** Constructor */
Application();
virtual ~Application();

private:
Application(const Application &); //purposely not implemented
void operator =(const Application&); //purposely not implemented

std::string            m_Name;
std::string            m_Description;
ParameterList::Pointer m_ParameterList;

}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperApplication_h_
