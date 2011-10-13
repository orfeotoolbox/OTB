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

#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperInputImageParameter.h"
//#include "otbWrapperParameterGroup.h"


namespace otb
{
namespace Wrapper
{

template <class TImageType>
void Application::SetParameterOutputImage(std::string parameter, TImageType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetValue(value);
    }
}

template <class TImageType>
TImageType* Application::GetParameterImage(std::string parameter)
{
  typename TImageType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    ret = paramDown->GetImage<TImageType>();
    }

  //TODO: exception if not found ?
  return ret;
}

}
}
