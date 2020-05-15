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


#include "otbMacro.h"

#include "otbSpectralResponse.h"

int otbSpectralResponse(int argc, char* argv[])
{
  typedef otb::SpectralResponse<double, double> ResponseType;
  typedef ResponseType::Pointer ResponsePointerType;
  typedef otb::VectorImage<double, 2> ImageType;

  if (argc != 3)
  {
    std::cout << argv[0] << std::endl
              << "\t"
              << "<Spectrum_filename>"
              << "\t"
              << "<Lambda>" << std::endl;
    return EXIT_FAILURE;
  }
  // Instantiation
  ResponsePointerType myResponse = ResponseType::New();
  // Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file, 100.0);
  // itk::Indent ind;
  double lambda = atof(argv[2]);

  ImageType::Pointer image = ImageType::New();
  myResponse->GetImage(image);

  otbLogMacro(Debug, << myResponse);
  otbLogMacro(Info, << "SpectResponse(" << lambda << ")= " << (*myResponse)(lambda));
  return EXIT_SUCCESS;
}
