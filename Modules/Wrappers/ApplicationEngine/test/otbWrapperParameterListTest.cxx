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

#include "otbWrapperParameterGroup.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperProxyParameter.h"

int otbWrapperParameterListNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  using otb::Wrapper::ParameterGroup;
  ParameterGroup::Pointer parameters = ParameterGroup::New();

  //std::cout << parameter << std::endl;

  return EXIT_SUCCESS;
}

int otbWrapperParameterList(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::ParameterGroup  GroupPrm;
  typedef otb::Wrapper::StringParameter StringPrm;
  typedef otb::Wrapper::IntParameter    IntPrm;
  typedef otb::Wrapper::ProxyParameter  ProxyPrm;
  
  GroupPrm::Pointer parameters = GroupPrm::New();

  StringPrm::Pointer strParam = StringPrm::New();
  strParam->SetKey("str");

  IntPrm::Pointer numParam = IntPrm::New();
  numParam->SetKey("num");

  IntPrm::Pointer hiddenParam = IntPrm::New();
  hiddenParam->SetKey("hidden");

  ProxyPrm::Pointer proxyParam = ProxyPrm::New();
  proxyParam->SetKey("num");
  proxyParam->SetInternalParameter(hiddenParam);

  parameters->AddParameter(strParam.GetPointer());
  parameters->AddParameter(numParam.GetPointer());

  if (parameters->ReplaceParameter(hiddenParam.GetPointer()))
    {
    std::cout << "Parameter with different keys replaced !" << std::endl;
    return EXIT_FAILURE;
    }

  if (! parameters->ReplaceParameter(proxyParam.GetPointer()))
    {
    std::cout << "Failed to replace with proxy parameter" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
