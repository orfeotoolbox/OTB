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



#include "itkMacro.h"

#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbLineSpatialObjectList.h"

#include "otbLocalHoughFilter.h"

#include <iostream>
#include <fstream>

int otbLocalHough(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outfname  = argv[2];
  unsigned int RadiusX((unsigned int) ::atoi(argv[3]));
  unsigned int RadiusY((unsigned int) ::atoi(argv[4]));
  unsigned int NumberOfLines((unsigned int) ::atoi(argv[5]));

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef otb::LocalHoughFilter<InputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  typedef otb::LineSpatialObjectList LinesListType;
  LinesListType::Pointer list = LinesListType::New();

  filter->SetRadius(Radius);
  filter->SetNumberOfLines(NumberOfLines);

  filter->SetInput(reader->GetOutput());
  filter->Update();

  list = filter->GetOutput();

  LinesListType::const_iterator itList;

  std::ofstream outfile(outfname);
  outfile << "size of the Line list " << list->size() << std::endl;

  for (itList = list->begin(); itList != list->end(); itList++)
    outfile << (*itList)->GetPoints()[0].GetPosition()  << " \t" << (*itList)->GetPoints()[1].GetPosition()   <<
    std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
