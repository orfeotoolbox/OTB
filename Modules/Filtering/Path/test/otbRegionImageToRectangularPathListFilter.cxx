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
#include "otbRegionImageToRectangularPathListFilter.h"
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"

#include <stdio.h>
#include <iostream>

int otbRegionImageToRectangularPathListFilter(int itkNotUsed(argc), char * argv[])
{

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned short InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  reader->Update();

  typedef otb::PolyLineParametricPathWithValue<double, Dimension> PathType;
  typedef otb::RegionImageToRectangularPathListFilter<InputImageType, PathType>
  RectangleListFilterType;

  RectangleListFilterType::Pointer rectangleFilter =
    RectangleListFilterType::New();

  rectangleFilter->SetInput(reader->GetOutput());

  rectangleFilter->SetMinimumFit(::atof(argv[3]));
  rectangleFilter->SetMinimumSize(::atoi(argv[4]));

  rectangleFilter->Update();

  typedef RectangleListFilterType::OutputPathListType ListType;

  ListType* pathList = rectangleFilter->GetOutput();

  ListType::Iterator listIt = pathList->Begin();

  FILE *file = fopen(outputFilename, "w");
  if (file == NULL)
    {
    fprintf(stderr, "Error, can't open file");
    exit(-1);
    }

  while (listIt != pathList->End())
    {

    for (PathType::VertexListType::ConstIterator vit  = listIt.Get()->GetVertexList()->Begin();
         vit != listIt.Get()->GetVertexList()->End(); ++vit)
      {
      double x = vit.Value()[0];
      double y = vit.Value()[1];
      fprintf(file, "%8.3f %8.3f\n", x, y);

      }

    double score = listIt.Get()->GetValue();
    fprintf(file, "%8.3f\n", score);

    ++listIt;
    }

  fclose(file);

  return EXIT_SUCCESS;
}
