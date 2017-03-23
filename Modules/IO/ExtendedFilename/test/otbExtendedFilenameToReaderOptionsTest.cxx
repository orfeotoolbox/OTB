/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbExtendedFilenameToReaderOptions.h"
#include <iostream>
#include <fstream>

typedef otb::ExtendedFilenameToReaderOptions FilenameHelperType;

int otbExtendedFilenameToReaderOptions(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputExtendedFilename  = argv[1];
  const char * outputFilename = argv[2];

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
  for (unsigned int i=0 ; i<rangeList.size(); i++)
    {
    if (i) file << ",";
    rangeList[i].Print(file);
    }
  file << "]" << std::endl;

  if (argc >= 4)
    {
    unsigned int nbBands = atoi(argv[3]);
    std::vector<unsigned int> bandList;
    bool ret = helper->ResolveBandRange(helper->GetBandRange(), nbBands,bandList);
    if (ret)
      {
      file << "BandList = [";
      for (unsigned int k=0 ; k<bandList.size() ; k++)
        {
        if (k) file << ",";
        file << bandList[k];
        }
      file << "]" << std::endl;
      }
    else
      {
      std::cout << "Invalid band range for a "<<nbBands<<" bands image"<< std::endl;
      }
    }

  file.close();
  return EXIT_SUCCESS;
}
