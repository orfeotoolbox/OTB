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

#include "otbScalarImageToPanTexTextureFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbScalarImageToPanTexTextureFilter(int argc, char * argv[])
{
  if(argc != 5)
    {
    std::cerr<<"Usage: "<<argv[0]<<" infname outprefix nbBins radius"<<std::endl;
    return EXIT_FAILURE;
    }
  const char * infname      = argv[1];
  const char * outprefix    = argv[2];
  const unsigned int nbBins = atoi(argv[3]);
  const unsigned int radius = atoi(argv[4]);

  const unsigned int Dimension = 2;
  typedef float                                  PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::ScalarImageToPanTexTextureFilter
    <ImageType,ImageType>                        PanTexTextureFilterType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;
  typedef itk::MinimumMaximumImageCalculator
    <ImageType>                                  MinMaxCalculatorType;

  ReaderType::Pointer         reader = ReaderType::New();
  PanTexTextureFilterType::Pointer filter = PanTexTextureFilterType::New();
  WriterType::Pointer         writer = WriterType::New();

  // Read image
  reader->SetFileName(infname);
  reader->Update();

  // Build radius
  PanTexTextureFilterType::SizeType sradius;
  sradius.Fill(radius);

  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfBinsPerAxis(nbBins);
  filter->SetRadius(sradius);

  otb::StandardFilterWatcher watcher(filter,"Textures filter");

  // Compute min/max
  MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
  minMax->SetImage(reader->GetOutput());
  minMax->Compute();

  filter->SetInputImageMinimum(minMax->GetMinimum());
  filter->SetInputImageMaximum(minMax->GetMaximum());
  filter->Update();

  // Write outputs
  itk::OStringStream oss;

  oss.str("");
  oss<<outprefix<<"PanTex.tif";
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  return EXIT_SUCCESS;
}
