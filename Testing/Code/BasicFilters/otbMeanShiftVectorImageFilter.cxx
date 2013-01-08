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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftVectorImageFilter.h"

int otbMeanShiftVectorImageFilter(int argc, char * argv[])
{
  if (argc != 10)
    {
            std::cerr << "Usage: " << argv[0]
            << " infname filteredfname clusteredfname labeledclusteredfname clusterboundariesfname spatialRadius rangeRadius minregionsize scale "
        << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname = argv[1];
  const char * filteredfname = argv[2];
  const char * clusteredfname = argv[3];
  const char * labeledclusteredfname = argv[4];
  const char * clusterboundariesfname = argv[5];
  const unsigned int spatialRadius = atoi(argv[6]);
  const double rangeRadius = atof(argv[7]);
  const unsigned int minRegionSize = atoi(argv[8]);
  const double scale = atof(argv[9]);

  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::LabeledOutputType LabeledImageType;
  typedef otb::ImageFileWriter<LabeledImageType> LabeledWriterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(infname);
  reader->Update();
  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  filter->SetScale(scale);

  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
  LabeledWriterType::Pointer writer3 = LabeledWriterType::New();
  LabeledWriterType::Pointer writer4 = LabeledWriterType::New();
  writer1->SetFileName(filteredfname);
  writer2->SetFileName(clusteredfname);
  writer3->SetFileName(labeledclusteredfname);
  writer4->SetFileName(clusterboundariesfname);
  writer1->SetInput(filter->GetOutput());
  writer2->SetInput(filter->GetClusteredOutput());
  writer3->SetInput(filter->GetLabeledClusteredOutput());
  writer4->SetInput(filter->GetClusterBoundariesOutput());
  writer1->Update();
  writer2->Update();
  writer3->Update();
  writer4->Update();

  return EXIT_SUCCESS;
}
