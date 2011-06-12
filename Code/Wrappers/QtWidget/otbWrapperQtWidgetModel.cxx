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
#include "otbWrapperQtWidgetModel.h"
#include "otbWrapperOutputImageParameter.h"
#include "itkImageFileWriter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetModel::QtWidgetModel(Application* app)
 : m_Application(app)
{

  m_Application->Init();

}

QtWidgetModel::~QtWidgetModel()
{
}

void QtWidgetModel::NotifyUpdate()
{
  m_Application->UpdateParameters();
}

void QtWidgetModel::Execute()
{
  m_Application->Execute();

  ParameterGroup* params = m_Application->GetParameterList();
  for (unsigned int i = 0; i < params->GetNumberOfParameters(); ++i)
    {
    Parameter* p = params->GetParameterByIndex( i );

    OutputImageParameter* pAsOutputImage = dynamic_cast<OutputImageParameter*>(p);
    if ( pAsOutputImage != 0 )
      {
      VectorImageType* image = pAsOutputImage->GetValue();
      typedef itk::ImageFileWriter<VectorImageType> WriterType;

      WriterType::Pointer writer = WriterType::New();
      writer->SetInput(image);
      writer->SetFileName(pAsOutputImage->GetFileName());
      writer->Update();
      }
    }
}

}
}
