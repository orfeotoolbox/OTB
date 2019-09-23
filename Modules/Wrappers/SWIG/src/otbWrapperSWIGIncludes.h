/*
 * Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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

#ifndef otbWrapperSWIGIncludes_h
#define otbWrapperSWIGIncludes_h

#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperAddProcessToWatchEvent.h"
#include "otbWrapperDocExampleStructure.h"
#include "otbWrapperMetaDataHelper.h"

typedef otb::Wrapper::Application            Application;
typedef otb::Wrapper::Application::Pointer   Application_Pointer;
typedef otb::Wrapper::ApplicationRegistry    Registry;
typedef otb::Wrapper::AddProcessToWatchEvent AddProcessToWatchEvent;
typedef otb::Wrapper::DocExampleStructure    DocExampleStructure;
typedef otb::Wrapper::Parameter              Parameter;
typedef otb::Wrapper::OutputImageParameter   OutputImageParameter;
typedef otb::Wrapper::InputImageParameter    InputImageParameter;

typedef otb::Wrapper::ImageBaseType ImageBaseType;

#if defined(SWIGPYTHON)
#include "otbPythonLogOutput.h"
#include "otbLogger.h"
#include "otbProgressReporterManager.h"

typedef otb::Logger                           Logger;
typedef otb::Logger::Pointer                  Logger_Pointer;
typedef otb::SwigPrintCallback                SwigPrintCallback;
typedef otb::PythonLogOutput                  PythonLogOutput;
typedef otb::PythonLogOutput::Pointer         PythonLogOutput_Pointer;
typedef otb::ProgressReporterManager          ProgressReporterManager;
typedef otb::ProgressReporterManager::Pointer ProgressReporterManager_Pointer;
#endif

#endif
