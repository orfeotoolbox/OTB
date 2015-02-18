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
#include "otbImageList.h"
#include "otbPolygon.h"
#include "otbRCC8VertexBase.h"
#include "otbImageMultiSegmentationToRCC8GraphFilter.h"
#include "otbImageFileReader.h"
#include "otbRCC8GraphFileWriter.h"

int otbImageMultiSegmentationToRCC8GraphFilter(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;
  char *             outputFilename = argv[1];
  int                useOptimisation = atoi(argv[2]);
  unsigned int       nbImages = atoi(argv[3]);

  // typedefs
  typedef unsigned short                        LabelPixelType;
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef otb::Polygon<>                        PolygonType;
  typedef otb::RCC8VertexBase<PolygonType>      VertexType;
  typedef otb::RCC8Graph<VertexType>            RCC8GraphType;
  typedef otb::ImageMultiSegmentationToRCC8GraphFilter<LabelImageType, RCC8GraphType>
  RCC8GraphFilterType;
  typedef otb::ImageList<LabelImageType>          ImageListType;
  typedef otb::ImageFileReader<LabelImageType>    ReaderType;
  typedef otb::RCC8GraphFileWriter<RCC8GraphType> GraphWriterType;

  ImageListType::Pointer inputList = ImageListType::New();

  // Reading input images
  for (unsigned int i = 0; i < nbImages; ++i)
    {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[4 + i]);
    reader->Update();
    inputList->PushBack(reader->GetOutput());
    }
  std::cout << "Input image loaded into images list." << std::endl;
  // Instantiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();
  filter->SetInput(inputList);
  filter->SetOptimisation(useOptimisation > 0);

  // Writing output graph
  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
