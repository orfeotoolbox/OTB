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

#include "otbWrapperRAMParameter.h"

int otbWrapperRAMParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::RAMParameter RAMParameterType;
  RAMParameterType::Pointer parameter = RAMParameterType::New();

  // Test Set/Get Value
  parameter->SetValue(256);
  parameter->SetValue(2560);
  parameter->SetValue(128);
  std::cout <<"Last RAMParameter Value set : " << parameter->GetValue() << std::endl;

  // Test Set/Get Default, extremum value
  RAMParameterType::ScalarType  min = 0;
  RAMParameterType::ScalarType  max = 1024;
  RAMParameterType::ScalarType  def = 256;

  parameter->SetDefaultValue(def);
  parameter->SetMinimumValue(min);
  parameter->SetMaximumValue(max);

  if (parameter->GetMinimumValue() != min)
    {
    std::cout <<"Minimum Value : expexted "<< min << " --> got "<< parameter->GetMinimumValue()  << std::endl;
    return EXIT_FAILURE;
    }

  if (parameter->GetMaximumValue() != max)
    {
    std::cout <<"Maximum Value : expexted "<< max << " --> got "<< parameter->GetMaximumValue()  << std::endl;
    return EXIT_FAILURE;
    }

  if (parameter->GetDefaultValue() != def)
    {
    std::cout <<"Default Value : expexted "<< def<< " --> got "<< parameter->GetDefaultValue()  << std::endl;
    return EXIT_FAILURE;
    }

  // Reset Value
  parameter->Reset();
  std::cout <<"Last RAMParameter Value set : " << parameter->GetValue() << std::endl;


  return EXIT_SUCCESS;
}
