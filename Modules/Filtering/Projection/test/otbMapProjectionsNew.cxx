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



#include <iostream>
#include <fstream>

#include "itkMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"



//Test the specific accessors for some map projection
int otbMapProjectionsTest(int itkNotUsed(argc), char * argv[])
{
  char * filename = argv[1];
  std::ofstream file;
  file.open(filename);

  otb::Eckert4InverseProjection::Pointer lEckert4Projection = otb::Eckert4InverseProjection::New();
  file << lEckert4Projection->GetFalseEasting() << std::endl;
  file << lEckert4Projection->GetFalseNorthing() << std::endl;
  lEckert4Projection->SetFalseEasting(500000);
  lEckert4Projection->SetFalseNorthing(500000);
  file << lEckert4Projection->GetFalseEasting() << std::endl;
  file << lEckert4Projection->GetFalseNorthing() << std::endl;
  lEckert4Projection->SetParameters(200000, 300000);
  file << lEckert4Projection->GetFalseEasting() << std::endl;
  file << lEckert4Projection->GetFalseNorthing() << std::endl;

  otb::Eckert4ForwardProjection::Pointer               lEckert4Projection2 = otb::Eckert4ForwardProjection::New();
  file << lEckert4Projection2->GetFalseEasting() << std::endl;
  file << lEckert4Projection2->GetFalseNorthing() << std::endl;
  lEckert4Projection2->SetFalseEasting(500000);
  lEckert4Projection2->SetFalseNorthing(500000);
  file << lEckert4Projection2->GetFalseEasting() << std::endl;
  file << lEckert4Projection2->GetFalseNorthing() << std::endl;
  lEckert4Projection2->SetParameters(200000, 300000);
  file << lEckert4Projection2->GetFalseEasting() << std::endl;
  file << lEckert4Projection2->GetFalseNorthing() << std::endl;


  otb::MollweidInverseProjection::Pointer          lMollweidProjection = otb::MollweidInverseProjection::New();
  file << lMollweidProjection->GetFalseEasting() << std::endl;
  file << lMollweidProjection->GetFalseNorthing() << std::endl;
  lMollweidProjection->SetFalseEasting(500000);
  lMollweidProjection->SetFalseNorthing(500000);
  file << lMollweidProjection->GetFalseEasting() << std::endl;
  file << lMollweidProjection->GetFalseNorthing() << std::endl;
  lMollweidProjection->SetParameters(200000, 300000);
  file << lMollweidProjection->GetFalseEasting() << std::endl;
  file << lMollweidProjection->GetFalseNorthing() << std::endl;


  otb::MollweidForwardProjection::Pointer          lMollweidProjection2 = otb::MollweidForwardProjection::New();
  file << lMollweidProjection2->GetFalseEasting() << std::endl;
  file << lMollweidProjection2->GetFalseNorthing() << std::endl;
  lMollweidProjection2->SetFalseEasting(500000);
  lMollweidProjection2->SetFalseNorthing(500000);
  file << lMollweidProjection2->GetFalseEasting() << std::endl;
  file << lMollweidProjection2->GetFalseNorthing() << std::endl;
  lMollweidProjection2->SetParameters(200000, 300000);
  file << lMollweidProjection2->GetFalseEasting() << std::endl;
  file << lMollweidProjection2->GetFalseNorthing() << std::endl;

  otb::SinusoidalInverseProjection::Pointer           lSinusoidalProjection = otb::SinusoidalInverseProjection::New();
  file << lSinusoidalProjection->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection->GetFalseNorthing() << std::endl;
  lSinusoidalProjection->SetFalseEasting(500000);
  lSinusoidalProjection->SetFalseNorthing(500000);
  file << lSinusoidalProjection->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection->GetFalseNorthing() << std::endl;
  lSinusoidalProjection->SetParameters(200000, 300000);
  file << lSinusoidalProjection->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection->GetFalseNorthing() << std::endl;

  otb::SinusoidalForwardProjection::Pointer           lSinusoidalProjection2 = otb::SinusoidalForwardProjection::New();
  file << lSinusoidalProjection2->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection2->GetFalseNorthing() << std::endl;
  lSinusoidalProjection2->SetFalseEasting(500000);
  lSinusoidalProjection2->SetFalseNorthing(500000);
  file << lSinusoidalProjection2->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection2->GetFalseNorthing() << std::endl;
  lSinusoidalProjection2->SetParameters(200000, 300000);
  file << lSinusoidalProjection2->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection2->GetFalseNorthing() << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
