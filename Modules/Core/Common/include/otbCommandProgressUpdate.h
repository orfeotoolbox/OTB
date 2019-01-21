/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbCommandProgressUpdate_h
#define otbCommandProgressUpdate_h

#include "itkCommand.h"
#include <sstream>

namespace otb
{

/**
* \class CommandProgressUpdate
* \brief TODO
*
 *
 * \ingroup OTBCommon
*/
template <class TFilter>
class CommandProgressUpdate : public itk::Command
{
public:
  typedef  CommandProgressUpdate   Self;
  typedef  itk::Command            Superclass;
  typedef  itk::SmartPointer<Self> Pointer;
  itkNewMacro(Self);
protected:
  CommandProgressUpdate() {}
public:
  typedef   const TFilter * FilterPointer;

  void Execute(itk::Object *caller, const itk::EventObject& event) override;

  void Execute(const itk::Object * object, const itk::EventObject& event) override;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCommandProgressUpdate.hxx"
#endif

#endif
