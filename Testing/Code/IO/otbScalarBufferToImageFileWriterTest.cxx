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



#include "otbScalarBufferToImageFileWriter.h"


int otbScalarBufferToImageFileWriterTest(int argc, char* argv[])
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
  tab = NULL;

  return EXIT_SUCCESS;
}
