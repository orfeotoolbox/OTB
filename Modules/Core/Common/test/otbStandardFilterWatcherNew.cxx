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

#include "itkMacro.h"

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbStandardFilterWatcher.h"
#include "itkGradientMagnitudeImageFilter.h"


  // Test default constructor.
  WatcherType watcher3;

  // Test assignment operator.
  watcher3 = watcher2;
  if ( watcher3.GetNameOfClass() != watcher2.GetNameOfClass()
       || watcher3.GetProcess() != watcher2.GetProcess()
       || watcher3.GetComment() != watcher2.GetComment() )
    {
    std::cout << "Operator= failed." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
