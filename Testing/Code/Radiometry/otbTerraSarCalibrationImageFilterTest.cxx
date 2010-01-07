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
  bool useMetadata = true;
  if(atoi(argv[3]) == 0)
    useMetadata = false;

  typedef otb::Image<double, 2>                                     ImageType;
  typedef otb::ImageFileReader<ImageType>                           ReaderType;
  typedef otb::ImageFileWriter<ImageType>                           WriterType;
  typedef otb::TerraSarCalibrationImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::DoubleVectorType                              DoubleVectorType;
  typedef FilterType::DoubleVectorVectorType                        DoubleVectorVectorType;
  typedef itk::ExtractImageFilter<ImageType, ImageType>             ExtractorType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();

  filter->SetInput(reader->GetOutput());
  
  if( !useMetadata )
  { 
    std::cout<<"pas de meeeeeeeeeeeeeeetadata"<<std::endl;
    DoubleVectorType coefs;
    coefs.push_back(1.);
    coefs.push_back(0.5);
    coefs.push_back(1.);
    coefs.push_back(0.1);
    DoubleVectorVectorType coefVect(1, coefs);
  
    coefs.clear();
    coefs.push_back(10.);
    coefs.push_back(5);
    coefs.push_back(10);
    coefs.push_back(1);
    coefVect.push_back( coefs );
    
    coefs.clear();
    coefs.push_back(100);
    coefs.push_back(50);
    coefs.push_back(100);
    coefs.push_back(10);
    coefVect.push_back( coefs );
  
    filter->SetNoisePolynomialCoefficientsList(coefVect);
  
    filter->SetCalFactor( 10 );
    filter->SetNoiseRangeValidityMin( 0 );
    filter->SetNoiseRangeValidityMax( 1 );
    filter->SetNoiseRangeValidityRef( 0.5 );
    filter->SetLocalIncidentAngle( 15 );
  
    std::vector<double> timeUtc;
    timeUtc.push_back(1.);
    timeUtc.push_back(2.);
    timeUtc.push_back(3.);
    filter->SetTimeUTC(timeUtc);
    filter->SetPRF(50);


    writer->SetInput(filter->GetOutput());
    
  }
  else
  {
    std::cout<<"meeeeeeeeeeeeeeeeeeeeeeetadata"<<std::endl;
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
    writer->SetInput(filter/*extractor*/->GetOutput());
  }
  
  filter->SetUseFastCalibrationMethod( false );
  writer->Update();
  

  return EXIT_SUCCESS;
}
