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

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbImage.h"
#include "itkExtractImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbTerraSarBrightnessImageFilterTest(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  bool useMetadata = false;
  if(atoi(argv[3]) != 0)
    useMetadata = true;

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

  if( useMetadata )
    {
      // Generate an extract from the large input
      ImageType::RegionType region;
      ImageType::IndexType id;
      id[0] = 50;   id[1] = 100;
      ImageType::SizeType size;
      size[0] = 150;   size[1] = 100;
      region.SetIndex(id);
      region.SetSize(size);
      extractor->SetExtractionRegion(region);
  
      extractor->SetInput(filter->GetOutput());
      writer->SetInput(extractor->GetOutput());
    }
  else
    {
      filter->SetCalFactor( 10 );
      writer->SetInput(filter->GetOutput());
    }

  writer->Update();

  return EXIT_SUCCESS;
}
