/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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



#include "otbRationalTransform.h"
#include <fstream>


int otbRationalTransform(int argc, char* argv[])
{
  typedef otb::RationalTransform<> RationalTransformType;

  // Instantiation
  RationalTransformType::Pointer rt = RationalTransformType::New();
  rt->SetNumeratorDegree(4);
  rt->SetDenominatorDegree(4);

  RationalTransformType::ParametersType params(rt->GetNumberOfParameters());
  params.Fill(1.);

  // Rational is
  // fx(x, y) = (1+2*x+3*x^2+4*x^3+5*x^4)/(6+7*x+8*x^2+9*x^3+10*x^4)
  // fy(x, y) = (11+12*y+13*y^2+14*y^3+15*y^4)/(16+17*y+18*y^2+19*y^3+20*y^4)
  params[0]=1;
  params[1]=2;
  params[2]=3;
  params[3]=4;
  params[4]=5;
  params[5]=6;
  params[6]=7;
  params[7]=8;
  params[8]=9;
  params[9]=10;
  params[10]=11;
  params[11]=12;
  params[12]=13;
  params[13]=14;
  params[14]=15;
  params[15]=16;
  params[16]=17;
  params[17]=18;
  params[18]=19;
  params[19]=20;

  rt->SetParameters(params);

  RationalTransformType::InputPointType inputPoint;
  RationalTransformType::OutputPointType outputPoint;

  std::ofstream ofs;
  ofs.open(argv[1]);

  // Set floatfield to format writing properly
  ofs.setf(std::ios::fixed, std::ios::floatfield);
  ofs.precision(10);

  unsigned int idx = 2;

  ofs<<"Rational function is: "<<std::endl;
  ofs<<"fx(x, y) = (1+2*x+3*x^2+4*x^3+5*x^4)/(6+7*x+8*x^2+9*x^3+10*x^4)"<<std::endl;
  ofs<<"fy(x, y) = (11+12*y+13*y^2+14*y^3+15*y^4)/(16+17*y+18*y^2+19*y^3+20*y^4)"<<std::endl;
  while(idx+1<(unsigned int)argc)
    {
    inputPoint[0] = atof(argv[idx]);
    inputPoint[1] = atof(argv[idx+1]);
    outputPoint = rt->TransformPoint(inputPoint);
    ofs<<inputPoint<<" -> "<<outputPoint<<std::endl;
    idx+=2;
    }

  ofs.close();

  return EXIT_SUCCESS;
}
