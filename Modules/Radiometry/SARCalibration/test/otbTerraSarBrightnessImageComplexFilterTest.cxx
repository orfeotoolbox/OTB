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
#include "otbVectorImage.h"
#include "itkExtractImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbComplexToVectorImageCastFilter.h"

int otbTerraSarBrightnessImageComplexFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const bool   useMetadata    = atoi(argv[3]);
  const bool   resultsInDb    = atoi(argv[4]);

  typedef std::complex<double>                                            ComplexType;
  typedef otb::Image<ComplexType, 2>                                      ImageType;
  typedef otb::VectorImage<double, 2>                                     VectorImageType;
  typedef otb::ImageFileReader<ImageType>                                 ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                           WriterType;

  typedef otb::TerraSarBrightnessImageFilter<ImageType, ImageType>        FilterType;
  typedef itk::ExtractImageFilter<ImageType, ImageType>                   ExtractorType;
  typedef otb::ComplexToVectorImageCastFilter<ImageType, VectorImageType> CastType;

  ReaderType::Pointer    reader    = ReaderType::New();
  WriterType::Pointer    writer    = WriterType::New();
  FilterType::Pointer    filter    = FilterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();
  CastType::Pointer      caster    = CastType::New();

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
    caster->SetInput(extractor->GetOutput());
    }
  else
    {
    filter->SetCalibrationFactor(10);
    caster->SetInput(filter->GetOutput());
    }


  writer->SetInput(caster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
