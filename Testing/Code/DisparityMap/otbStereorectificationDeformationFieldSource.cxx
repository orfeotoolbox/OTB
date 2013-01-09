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
#include "itkVector.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStereorectificationDeformationFieldSource.h"

typedef itk::Vector<double,2>                               DeformationType;
typedef otb::Image<DeformationType>                         DeformationFieldType;
typedef otb::Image<unsigned short>                          ImageType;
typedef otb::StereorectificationDeformationFieldSource
   <ImageType,DeformationFieldType>                         DeformationFieldSourceType;
typedef otb::ImageFileReader<ImageType>                     ReaderType;
typedef otb::StreamingImageFileWriter<DeformationFieldType> WriterType;

int otbStereorectificationDeformationFieldSourceNew(int argc, char * argv[])
{
  // Instantiation
  DeformationFieldSourceType::Pointer dfSource = DeformationFieldSourceType::New();

  return EXIT_SUCCESS;
}

int otbStereorectificationDeformationFieldSource(int argc, char * argv[])
{
  char * infname1  = argv[1];
  char * infname2  = argv[2];
  char * outfname1 = argv[3];
  char * outfname2 = argv[4];
  double avgElev   = atof(argv[5]);
  double scale     = atof(argv[6]);
  double gridStep  = atoi(argv[7]);
  
  // Instantiation
  DeformationFieldSourceType::Pointer dfSource = DeformationFieldSourceType::New();

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(infname1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(infname2);

  dfSource->SetLeftImage(reader1->GetOutput());
  dfSource->SetRightImage(reader2->GetOutput());
  dfSource->SetGridStep(gridStep);
  dfSource->SetScale(scale);

  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(avgElev);

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(dfSource->GetLeftDeformationFieldOutput());
  writer1->SetFileName(outfname1);
  writer1->Update();

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(dfSource->GetRightDeformationFieldOutput());
  writer2->SetFileName(outfname2);
  writer2->Update();

  return EXIT_SUCCESS;
}
