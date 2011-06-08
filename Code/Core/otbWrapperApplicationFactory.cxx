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
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

ApplicationFactory::ApplicationFactory()
{
}

ApplicationFactory::~ApplicationFactory()
{
}

ApplicationFactory::ApplicationPointer
ApplicationFactory::CreateApplication(const std::string& name)
{
  ApplicationPointer appli;

  std::list<ApplicationPointer> possibleApp;
  std::list<LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("otbWrapperApplication");

  // Downcast and Sanity check
  for (std::list<LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
    {
    Application* io = dynamic_cast<Application*> (i->GetPointer());
    if (io)
      {
      possibleApp.push_back(io);
      }
    else
      {
      otbMsgDevMacro( << "Error ApplicationFactory factory did not return an Application: " << (*i)->GetNameOfClass() << std::endl );
      }
    }

  // Return the app with the desired name
  for(std::list<ApplicationPointer>::iterator k = possibleApp.begin();
      k != possibleApp.end(); ++k)
    {
    if ( (*k)->GetName() == name )
      {
      return *k;
      }
    }


  return appli;
}

std::list<std::string>
ApplicationFactory::GetAvailableApplications()
{
  ApplicationPointer appli;

  std::list<ApplicationPointer> possibleApp;
  std::list<LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("otbWrapperApplication");

  // Downcast and Sanity check
  for (std::list<LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
    {
    Application* io = dynamic_cast<Application*> (i->GetPointer());
    if (io)
      {
      possibleApp.push_back(io);
      }
    else
      {
      otbMsgDevMacro( "Error ApplicationFactory factory did not return an Application: " << (*i)->GetNameOfClass() << std::endl );
      }
    }

  // Return the app list
  std::list<std::string> availableApp;
  for(std::list<ApplicationPointer>::iterator k = possibleApp.begin();
      k != possibleApp.end(); ++k)
    {
    availableApp.push_back((*k)->GetName());
    }
  return availableApp;
}


} // end namespace Wrapper
} //end namespace otb
