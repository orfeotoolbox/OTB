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

  // setup first group of parameters
  GroupPrm::Pointer parameters = GroupPrm::New();

  StringPrm::Pointer strParam = StringPrm::New();
  strParam->SetKey("str");

  IntPrm::Pointer numParam = IntPrm::New();
  numParam->SetKey("num");
  numParam->SetValue(1);

  parameters->AddParameter(strParam.GetPointer());
  parameters->AddParameter(numParam.GetPointer());

  // setup second group of parameters
  GroupPrm::Pointer otherParameters = GroupPrm::New();

  IntPrm::Pointer hiddenParam = IntPrm::New();
  hiddenParam->SetKey("hidden");
  hiddenParam->SetValue(2);

  otherParameters->AddParameter(hiddenParam.GetPointer());

  ProxyPrm::Pointer proxyParam = ProxyPrm::New();
  ProxyPrm::ProxyTargetType target;
  target.first = otherParameters;
  target.second = "hidden";
  proxyParam->SetTarget(target);

  // try to set a proxy to "hidden" in the first group
  std::string proxyKey("num");
  if (! parameters->SetParameter(proxyParam.GetPointer(), proxyKey))
    {
    std::cout << "Failed to replace with proxy parameter" << std::endl;
    return EXIT_FAILURE;
    }

  // check that we get the right value in "num"
  otb::Wrapper::Parameter* resultParam = GroupPrm::ResolveParameter(parameters->GetParameterByKey("num"));
  IntPrm* castInt = dynamic_cast<IntPrm*>(resultParam);
  if (castInt)
    {
    if (castInt->GetValue() != 2)
      {
      std::cout << "Failed to setup proxy on int parameter, got "<< castInt->GetValue()<< ", expected 2."<< std::endl;
      return EXIT_FAILURE;
      }
    }
  else
    {
    std::cout << "Can't cast parameter to Int, probably wrong type."<< std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
