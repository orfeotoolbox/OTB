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

#include "otbPolyLineImageIterator.h"
#include "itkPolyLineParametricPath.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"

int otbPolyLineImageIterator(int argc, char * argv[])
{
  if (argc != 5)
    {
    std::cout << "Usgage: " << argv[0] << " sizex sizey nbpoints outfname" << std::endl;
    return EXIT_SUCCESS;
    }
  const unsigned int sizex = atoi(argv[1]);
  const unsigned int sizey = atoi(argv[2]);
  const unsigned int nbpoints  = atoi(argv[3]);
  const char *       outfname = argv[4];

  const unsigned int Dimension = 2;
  typedef unsigned char                                   PixelType;
  typedef otb::Image<PixelType, Dimension>                ImageType;
  typedef itk::PolyLineParametricPath<Dimension>          PathType;
  typedef PathType::VertexType                            VertexType;
  typedef otb::PolyLineImageIterator<ImageType, PathType> IteratorType;
  typedef otb::ImageFileWriter<ImageType>                 WriterType;

  ImageType::SizeType size;
  size[0] = sizex;
  size[1] = sizey;
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);
  ImageType::Pointer image = ImageType::New();
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  PathType::Pointer path = PathType::New();

  for (unsigned int i = 1; i < nbpoints; ++i)
    {
    VertexType vertex1, vertex2;
    vertex1[0] = 0;
    vertex1[1] = i * sizey / nbpoints;
    vertex2[0] = i * sizex / nbpoints;
    vertex2[1] = 0;
    path->AddVertex(vertex1);
    path->AddVertex(vertex2);
    }
  for (unsigned int i = 1; i < nbpoints; ++i)
    {
    VertexType vertex1, vertex2;
    vertex1[0] = i * sizex / nbpoints;
    vertex1[1] = sizey - 1;
    vertex2[0] = sizex - 1;
    vertex2[1] = i * sizey / nbpoints;
    path->AddVertex(vertex1);
    path->AddVertex(vertex2);
    }

  IteratorType it(image, path);

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    it.Set(255);
    // std::cout<<it.GetIndex()<<std::endl;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image);
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
