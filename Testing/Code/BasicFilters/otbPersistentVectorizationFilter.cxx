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

#include "otbImage.h"
#include "otbPolygon.h"
#include "otbPersistentVectorizationImageFilter.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbPersistentVectorizationFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname  = argv[1];
  const char * outfname = argv[2];

  const unsigned int ImageDimension = 2;
  typedef unsigned short LabelType;

  typedef otb::Image<LabelType, ImageDimension>                           ImageType;
  typedef otb::Polygon <LabelType>                                        PolygonType;
  typedef otb::PersistentVectorizationImageFilter<ImageType, PolygonType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                 ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->Update();

  typedef FilterType::PathListType PathListType;

  std::ofstream f;
  f.open(outfname);

  for (PathListType::Iterator it = filter->GetPathList()->Begin();
       it != filter->GetPathList()->End();
       ++it)
    {
    for (unsigned int i = 0; i < it.Get()->GetVertexList()->Size(); ++i)
      {
      f << "[ " << it.Get()->GetVertexList()->GetElement(i)[0]
        << ", " << it.Get()->GetVertexList()->GetElement(i)[1]
        << "] ";
      }
    f << std::endl;
    }

  f.close();

  return EXIT_SUCCESS;

}
