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

#include "otbTerraSarRadarBrightnessImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToAmplitudeImageFilter.h"

int otbTerraSarRadarBrightnessImageFilterTest(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef otb::Image<double, 2>                                              ImageType;
  typedef otb::VectorImage<double, 2>                                        VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>                              ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                    WriterType;
  typedef otb::VectorImageToAmplitudeImageFilter<VectorImageType, ImageType> AmplitudeFilterType;
  typedef otb::TerraSarRadarBrightnessImageFilter<ImageType, ImageType>     FilterType;
  typedef FilterType::DoubleVectorType                                       DoubleVectorType;
  typedef FilterType::DoubleVectorVectorType                                 DoubleVectorVectorType;

  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();
  AmplitudeFilterType::Pointer ampli = AmplitudeFilterType::New();
  FilterType::Pointer filter         = FilterType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();
  //unsigned int nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  std::cout<<reader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;


  filter->SetCalFactor( 10 );


  
  ampli->SetInput(reader->GetOutput());
  filter->SetInput(ampli->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->SetNumberOfStreamDivisions(1);
  writer->Update(); 

  return EXIT_SUCCESS;
}
