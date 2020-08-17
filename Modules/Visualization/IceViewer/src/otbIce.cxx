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

#include "otbIceViewer.h"
#include "otbDEMHandler.h"


void error_callback(int, const char* description)
{
  std::cerr << description << std::endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " img1 ... imgN" << std::endl << std::endl;

    return EXIT_FAILURE;
  }

  char* demdir    = getenv("OTB_DEM_DIR");
  char* geoidfile = getenv("OTB_GEOID_FILE");

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();

  if (demdir != nullptr)
  {
    std::cout << "Configuring DEM directory: " << demdir << std::endl;
    demHandler->OpenDEMDirectory(demdir);
  }

  if (geoidfile != nullptr)
  {
    std::cout << "Configuring geoid file: " << geoidfile << std::endl;
    demHandler->OpenGeoidFile(geoidfile);
  }

  otb::IceViewer::Pointer viewer = otb::IceViewer::New();

  // Initialize viewer
  try
  {
    viewer->Initialize(800, 600);
  }
  catch (itk::ExceptionObject& err)
  {
    std::cerr << "Failed to initialized viewer: " << err << std::endl;
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; ++i)
  {
    try
    {
      viewer->AddImage(argv[i], argv[i]);
    }
    catch (itk::ExceptionObject& err)
    {
      std::cerr << "Failed to open object as image: " << err << std::endl;
      try
      {
        viewer->AddVector(argv[i], argv[i]);
      }
      catch (itk::ExceptionObject& err2)
      {
        std::cerr << "Failed to open object as vector: " << err2 << std::endl;
        std::cerr << "Could not open file " << argv[i] << " as an image or a vector, skipping." << std::endl;
      }
    }
    catch (std::runtime_error& err)
    {
      std::cerr << "Runtime error: " << err.what() << std::endl;
      return EXIT_FAILURE;
    }
  }

  std::cout << "Press F1 for help" << std::endl;

  viewer->Start();


  return EXIT_SUCCESS;
}
