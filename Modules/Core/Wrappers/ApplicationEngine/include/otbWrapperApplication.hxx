/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperApplication_hxx
#define otbWrapperApplication_hxx


#include "otbWrapperApplication.h"


namespace otb
{


namespace Wrapper
{


template <class TImageType>
TImageType* Application::GetParameterImage(std::string const& parameter)

{
  typename TImageType::Pointer ret;
  Parameter*                   param     = GetParameterByKey(parameter);
  InputImageParameter*         paramDown = dynamic_cast<InputImageParameter*>(param);
  if (paramDown)
  {
    return paramDown->GetImage<TImageType>();
  }
  else
  {
    itkExceptionMacro(<< parameter << " parameter can't be casted to ImageType");
    return nullptr;
  }
}


template <class TImageType>
void Application::SetParameterOutputImage(std::string const& parameter, TImageType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputImageParameter*>(param))
  {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetValue(value);
  }
}


} // End namespace Wrapper

} // End namespace otb

#endif
