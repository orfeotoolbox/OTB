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
#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbConvexOrConcaveClassificationFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * levelingfname = argv[2];
  const char * outfname = argv[3];
  const double sigma = atof(argv[4]);

  typedef otb::Image<double, 2>        InputImageType;
  typedef otb::Image<unsigned char, 2> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>                                      ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                            WriterType;
  typedef otb::ConvexOrConcaveClassificationFilter<InputImageType, OutputImageType> ClassificationFilterType;

  // Instantiating objects
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();
  ReaderType::Pointer               reader = ReaderType::New();
  ReaderType::Pointer               levelingReader = ReaderType::New();
  WriterType::Pointer               writer = WriterType::New();

  reader->SetFileName(infname);
  levelingReader->SetFileName(levelingfname);

  filter->SetInput(reader->GetOutput());
  filter->SetInputLeveling(levelingReader->GetOutput());
  filter->SetConvexLabel(255);
  filter->SetConcaveLabel(128);
  filter->SetFlatLabel(0);
  filter->SetSigma(sigma);

  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
