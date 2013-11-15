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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperStringListParameter.h"

int otbWrapperStringListParameterNew(int argc, char* argv[])
{
  typedef otb::Wrapper::StringListParameter StringListParameterType;
  StringListParameterType::Pointer parameter = StringListParameterType::New();

  return EXIT_SUCCESS;
}


int otbWrapperStringListParameterTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::StringListParameter StringListParameterType;
  StringListParameterType::Pointer numParam = StringListParameterType::New();

  const std::string value1 = argv[1];
  const std::string value2 = argv[2];
  const std::string value3 = argv[3];
  const std::string key    = argv[4];
  const std::string desc   = argv[5];

  numParam->AddString(value1);
  StringListParameterType::StringListType sList;
  sList.push_back(value2);
  sList.push_back(value3);
  numParam->SetValue( sList );
  numParam->SetKey(key);
  numParam->SetDescription(desc);

  if( numParam->GetValue()[0] != value2 )
    {
      return EXIT_FAILURE;
    }

  if( numParam->GetNthElement(1) != value3 )
    {
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
