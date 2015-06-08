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

#include "otbNoDataHelper.h"
#include "otbMacro.h"

int otbNoDataHelperTest(int itkNotUsed(argc),char * itkNotUsed(argv) [])
{
  std::vector<bool> b1(1,true);
  std::vector<double> v1(1,0);

  otbControlConditionTestMacro(otb::IsNoData(10,b1,v1)," wrong output of IsNoData function");
  otbControlConditionTestMacro(!otb::IsNoData(0,b1,v1)," wrong output of IsNoData function");
  b1[0]=false;
  otbControlConditionTestMacro(otb::IsNoData(10,b1,v1)," wrong output of IsNoData function");
  otbControlConditionTestMacro(otb::IsNoData(0,b1,v1)," wrong output of IsNoData function");


  std::vector<bool> b2(4,true);
  std::vector<double> v2(4,1);

  itk::VariableLengthVector<unsigned short int> v(4);
  v.Fill(10);

  otbControlConditionTestMacro(otb::IsNoData(v,b2,v2)," wrong output of IsNoData function");
  v[1]=1;
  otbControlConditionTestMacro(!otb::IsNoData(v,b2,v2)," wrong output of IsNoData function");
  b2[1]=false;
  otbControlConditionTestMacro(otb::IsNoData(v,b2,v2)," wrong output of IsNoData function");

  return EXIT_SUCCESS;
}
