/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include <stdlib.h>
#include <iostream>

#include "otb_ossim.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimFilename.h>
#pragma GCC diagnostic pop
#else
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimFilename.h>
#endif

// This test demonstrate that handler->close() does not release the file descriptor for jpeg files.

int otbOssimJpegFileResourceLeakTest(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage:" << std::endl;
    std::cerr << argv[0] << " infname nbReading" << std::endl;
    return 1;
  }

  const char*        infname   = argv[1];
  const unsigned int nbReading = atoi(argv[2]);

  for (unsigned int i = 0; i < nbReading; ++i)
  {
    // Create the handler
    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(infname));
    if (!handler)
    {
      std::cerr << "Failed to open image " << i << " times." << std::endl;
      return 1;
    }
    handler->close();
  }
  return 0;
}
