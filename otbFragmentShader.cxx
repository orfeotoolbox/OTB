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
#include "otbFragmentShader.h"
#include "otbFragmentShaderRegistry.h"

namespace otb
{
FragmentShader::FragmentShader()
{}

FragmentShader::~FragmentShader()
{}

void FragmentShader::BuildShader()
{
  std::string source = this->GetSource();
  std::string name = this->GetName();

  std::cout<<"Name: "<<name<<", source: "<<source<<std::endl;

  try
    {
    otb::FragmentShaderRegistry::Instance()->RegisterShader(name,source);
    }
  catch(itk::ExceptionObject& err)
    {
    std::cerr<<err<<std::endl;
    // Shader already registered, do nothing
    }
}

void FragmentShader::LoadShader()
{
  otb::FragmentShaderRegistry::Instance()->LoadShader(GetName());
}

void FragmentShader::UnloadShader()
{
  otb::FragmentShaderRegistry::Instance()->UnloadShader();
}

void FragmentShader::SetupShader()
{}

}
