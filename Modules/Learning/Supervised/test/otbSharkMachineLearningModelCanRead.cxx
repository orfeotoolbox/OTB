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

#include <iostream>

#include <otbMachineLearningModel.h>

#ifdef OTB_USE_SHARK
#include "otbSharkRandomForestsMachineLearningModel.h"
#endif

typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;


int otbSharkRFMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
#ifdef OTB_USE_SHARK  
  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType, TargetValueType> RFType;
  RFType::Pointer classifier = RFType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Error otb::SharkRandomForestsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
#endif
  return EXIT_SUCCESS;
}
