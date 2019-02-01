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




#include "otbScalarBufferToImageFileWriter.h"


int otbScalarBufferToImageFileWriterTest(int itkNotUsed(argc), char* argv[])
{
       unsigned int nbCh = atoi(argv[1]);
       unsigned int sizeX = atoi(argv[2]);
       unsigned int sizeY = atoi(argv[3]);

  typedef otb::ScalarBufferToImageFileWriter<float, unsigned int> FilterType;
  FilterType::Pointer filter = FilterType::New();

  float * tab = new float[nbCh*sizeX*sizeY];

  int cpt=0;
  double val = 0.;
  for(unsigned int sx=0; sx<sizeX; sx++)
       {
              for(unsigned int sy=0; sy<sizeY; sy++)
              {
                     for(unsigned int i=0; i<nbCh; ++i)
                     {
                            tab[cpt++] = val;
                            val++;
                     }
              }
       }

  FilterType::SizeType size;
  size[0] = sizeX;
  size[1] = sizeY;
  filter->SetImageSize(size);
  filter->SetNumberOfChannels(nbCh);
  filter->SetBuffer(tab);
  filter->SetFileName(argv[4]);
  filter->SetInverseXSpacing(true);
  filter->Update();

  delete[] tab;
  tab = nullptr;

  return EXIT_SUCCESS;
}
