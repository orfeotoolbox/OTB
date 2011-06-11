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
#ifndef __otbWrapperSWIGIncludes_h
#define __otbWrapperSWIGIncludes_h

#include "otbWrapperParameter.h"
#include "otbWrapperParameterGroup.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationRegistry.h"


typedef otb::Wrapper::Parameter                        Parameter;
typedef otb::Wrapper::Parameter::Pointer::SmartPointer Parameter_Pointer;

typedef otb::Wrapper::ParameterGroup                        ParameterGroup;
typedef otb::Wrapper::ParameterGroup::Pointer::SmartPointer ParameterGroup_Pointer;

typedef otb::Wrapper::Application                        Application;
typedef otb::Wrapper::Application::Pointer::SmartPointer Application_Pointer;

typedef otb::Wrapper::ApplicationRegistry                        ApplicationRegistry;
typedef otb::Wrapper::ApplicationRegistry::Pointer::SmartPointer ApplicationRegistry_Pointer;

#endif
