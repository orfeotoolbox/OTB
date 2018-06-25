/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbCommandProgressUpdate_hxx
#define otbCommandProgressUpdate_hxx

#include "otbCommandProgressUpdate.h"

namespace otb
{

template <class TFilter>
void CommandProgressUpdate<TFilter>::Execute(itk::Object *caller, const itk::EventObject& event)
{
  Execute((const itk::Object *) caller, event);
}

template <class TFilter>
void CommandProgressUpdate<TFilter>::Execute(const itk::Object * object, const itk::EventObject& event)
{
  FilterPointer filter =
    dynamic_cast<FilterPointer>(object);
  if (typeid(event) != typeid(itk::ProgressEvent))
    {
    return;
    }

  int factor = 160;

  int val = int(filter->GetProgress() * factor);

  if ((val % 2) == 0)
    {
    std::cout << "|";
    std::cout.flush();
    }

  if (val == factor)
    {
    std::cout << ">";
    std::cout.flush();
    }
}

} // end namespace otb

#endif
