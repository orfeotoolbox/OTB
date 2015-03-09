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

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbImage.h"
#include "itkExtractImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbTerraSarBrightnessImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const bool   useMetadata    = atoi(argv[3]);
  const bool   resultsInDb    = atoi(argv[4]);

  typedef otb::Image<double, 2>                                    ImageType;
  typedef otb::ImageFileReader<ImageType>                          ReaderType;
  typedef otb::ImageFileWriter<ImageType>                          WriterType;
  typedef otb::TerraSarBrightnessImageFilter<ImageType, ImageType> FilterType;
  typedef itk::ExtractImageFilter<ImageType, ImageType>            ExtractorType;

  ReaderType::Pointer    reader    = ReaderType::New();
  WriterType::Pointer    writer    = WriterType::New();
  FilterType::Pointer    filter    = FilterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();
  filter->SetInput(reader->GetOutput());
  filter->SetResultsInDecibels(resultsInDb);

  if (useMetadata)
    {
    // Generate an extract from the large input
    ImageType::RegionType region;
    ImageType::IndexType  id;
    id[0] = atoi(argv[5]);   id[1] = atoi(argv[6]);
    ImageType::SizeType size;
    size[0] = atoi(argv[7]);   size[1] = atoi(argv[8]);
    region.SetIndex(id);
    region.SetSize(size);
    extractor->SetExtractionRegion(region);

    extractor->SetInput(filter->GetOutput());
    writer->SetInput(extractor->GetOutput());
    }
  else
    {
    filter->SetCalibrationFactor(10);
    writer->SetInput(filter->GetOutput());
    }

  writer->Update();

  return EXIT_SUCCESS;
}
