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
#include "otbWrapperApplicationFactoryBase.h"

namespace otb
{
namespace Wrapper
{

Application::Pointer
ApplicationFactoryBase::CreateApplication(const char* name)
{
  Application::Pointer appli;
  
  LightObject::Pointer obj = this->CreateObject(name);
  if (obj.IsNotNull())
    {
    Application* app = dynamic_cast<Application*> (obj.GetPointer());
    if (app)
      {
      appli = app;
      }
    }
  return appli;
}

}
}
