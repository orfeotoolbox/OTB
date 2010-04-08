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
#include "itkExceptionObject.h"

#include "otbTerraSarCalibrationImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkExtractImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbTerraSarCalibrationImageFilterTest(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const bool   useFastCalibration = atoi(argv[3]);
  const bool   resultsInDbs = atoi(argv[4]);

  typedef double                                                    PixelType;
  typedef otb::Image<PixelType, 2>                                  ImageType;
  typedef otb::ImageFileReader<ImageType>                           ReaderType;
  typedef otb::ImageFileWriter<ImageType>                           WriterType;
  typedef otb::TerraSarCalibrationImageFilter<ImageType, ImageType> FilterType;
  typedef itk::ExtractImageFilter<ImageType, ImageType>             ExtractorType;

  ReaderType::Pointer    reader = ReaderType::New();
  WriterType::Pointer    writer = WriterType::New();
  FilterType::Pointer    filter = FilterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();

  filter->SetInput(reader->GetOutput());
  filter->SetUseFastCalibration(useFastCalibration);
  filter->SetResultsInDecibels(resultsInDbs);

  if (argc == 9)
    {
    ImageType::RegionType region;
    ImageType::IndexType  id;
    id[0] = atoi(argv[5]);
    id[1] = atoi(argv[6]);
    ImageType::SizeType size;
    size[0] = atoi(argv[7]);
    size[1] = atoi(argv[8]);
    region.SetIndex(id);
    region.SetSize(size);
    extractor->SetExtractionRegion(region);
    extractor->SetInput(filter->GetOutput());
    writer->SetInput(extractor->GetOutput());
    }
  else
    {
    writer->SetInput(filter->GetOutput());
    }

  writer->Update();

  return EXIT_SUCCESS;
}
