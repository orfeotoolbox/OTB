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
#include "otbWrapperApplication.h"

#include "itkImageFileWriter.h"
#include "otbStandardWriterWatcher.h"

namespace otb
{
namespace Wrapper
{

Application::Application(): m_Name(""), m_Description("")
{
}

Application::~Application()
{
}

void Application::Init()
{
  m_ParameterList = ParameterGroup::New();
  this->DoCreateParameters();
}

void Application::UpdateParameters()
{
  this->DoUpdateParameters();
}

void Application::Execute()
{
  this->DoExecute();

  ParameterGroup* params = GetParameterList();
  for (unsigned int i = 0; i < params->GetNumberOfParameters(); ++i)
    {
    Parameter* p = params->GetParameter( i );
    OutputImageParameter* pAsOutputImage = dynamic_cast<OutputImageParameter*>(p);
    if ( pAsOutputImage != 0 )
      {
      OutputImageParameter::VectorImageType* image = pAsOutputImage->GetValue();
      typedef itk::ImageFileWriter<OutputImageParameter::VectorImageType> WriterType;

      WriterType::Pointer writer = WriterType::New();
      writer->SetInput(image);
      writer->SetFileName(pAsOutputImage->GetFileName());

      std::cout << image << std::endl;
      itk::ProcessObject* source = image->GetSource();
      std::cout << source << std::endl;
      otb::StandardWriterWatcher watcher(writer, source, "test");
      writer->Update();
      }
    }
}

}
}

