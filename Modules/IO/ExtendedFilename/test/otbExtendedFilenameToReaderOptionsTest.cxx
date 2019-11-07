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

#include "otbExtendedFilenameToReaderOptions.h"
#include <iostream>
#include <fstream>

typedef otb::ExtendedFilenameToReaderOptions FilenameHelperType;

int otbExtendedFilenameToReaderOptions(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputExtendedFilename = argv[1];
  const char* outputFilename        = argv[2];

  FilenameHelperType::Pointer helper = FilenameHelperType::New();

  helper->SetExtendedFileName(inputExtendedFilename);

  std::ofstream file;
  file.open(outputFilename);

  file << helper->SimpleFileNameIsSet() << std::endl;
  file << helper->GetSimpleFileName() << std::endl;

  file << helper->ExtGEOMFileNameIsSet() << std::endl;
  file << helper->GetExtGEOMFileName() << std::endl;

  file << helper->SubDatasetIndexIsSet() << std::endl;
  file << helper->GetSubDatasetIndex() << std::endl;

  file << helper->ResolutionFactorIsSet() << std::endl;
  file << helper->GetResolutionFactor() << std::endl;

  file << helper->SkipCartoIsSet() << std::endl;
  file << helper->GetSkipCarto() << std::endl;

  file << helper->BandRangeIsSet() << std::endl;
  file << "[";

  std::vector<otb::ExtendedFilenameHelper::GenericBandRange> rangeList = helper->GetGenericBandRange(helper->GetBandRange());
  for (unsigned int i = 0; i < rangeList.size(); i++)
  {
    if (i)
      file << ",";
    rangeList[i].Print(file);
  }
  file << "]" << std::endl;

  if (argc >= 4)
  {
    unsigned int              nbBands = atoi(argv[3]);
    std::vector<unsigned int> bandList;
    bool                      ret = helper->ResolveBandRange(helper->GetBandRange(), nbBands, bandList);
    if (ret)
    {
      file << "BandList = [";
      for (unsigned int k = 0; k < bandList.size(); k++)
      {
        if (k)
          file << ",";
        file << bandList[k];
      }
      file << "]" << std::endl;
    }
    else
    {
      std::cout << "Invalid band range for a " << nbBands << " bands image" << std::endl;
    }
  }

  file.close();
  return EXIT_SUCCESS;
}
