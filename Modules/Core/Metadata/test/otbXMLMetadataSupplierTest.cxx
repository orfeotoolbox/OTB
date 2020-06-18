/*
 * Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbXMLMetadataSupplier.h"

int otbXMLMetadataSupplierTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* fileName = argv[1];
  const char* outputFilename = argv[2];
  otb::XMLMetadataSupplier mds(fileName);

  std::ofstream file;
  file.open(outputFilename);
  file << mds.GetMetadataValue("calibration.adsHeader.swath") << "\n";
  file << mds.GetMetadataValue("calibration.calibrationVectorList.calibrationVector_2.line") << "\n";
  file.close();

  return EXIT_SUCCESS;
}
