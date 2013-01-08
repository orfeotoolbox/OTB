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

#include "otbDEMCaracteristicsExtractor.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

int otbDEMCaracteristicsExtractor(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputSlotFileName = argv[6];
  const char * outputAspectFileName = argv[7];
  const char * outputIncidenceFileName = argv[8];
  const char * outputExitanceFileName = argv[9];

  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputImageType;

  typedef otb::DEMCaracteristicsExtractor<InputImageType, InputImageType> DEMCaracteristicsExtractorType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::ImageFileWriter<InputImageType>                   WriterType;

  ReaderType::Pointer                     input = ReaderType::New();
  WriterType::Pointer                     slotWriter = WriterType::New();
  WriterType::Pointer                     aspectWriter = WriterType::New();
  WriterType::Pointer                     incidenceWriter = WriterType::New();
  WriterType::Pointer                     exitanceWriter = WriterType::New();
  DEMCaracteristicsExtractorType::Pointer filter = DEMCaracteristicsExtractorType::New();

  slotWriter->SetNumberOfDivisionsStrippedStreaming(1);
  aspectWriter->SetNumberOfDivisionsStrippedStreaming(1);
  incidenceWriter->SetNumberOfDivisionsStrippedStreaming(1);
  exitanceWriter->SetNumberOfDivisionsStrippedStreaming(1);

  input->SetFileName(inputFileName);
  slotWriter->SetFileName(outputSlotFileName);
  aspectWriter->SetFileName(outputAspectFileName);
  incidenceWriter->SetFileName(outputIncidenceFileName);
  exitanceWriter->SetFileName(outputExitanceFileName);
  filter->SetInput(input->GetOutput());
  filter->SetViewAzimut(static_cast<double>(atof(argv[2])));
  filter->SetViewAngle(static_cast<double>(atof(argv[3])));
  filter->SetSolarAzimut(static_cast<double>(atof(argv[4])));
  filter->SetSolarAngle(static_cast<double>(atof(argv[5])));

  slotWriter->SetInput(filter->GetSlopOutput());
  aspectWriter->SetInput(filter->GetAspectOutput());
  incidenceWriter->SetInput(filter->GetIncidenceOutput());
  exitanceWriter->SetInput(filter->GetExitanceOutput());

  slotWriter->Update();
  aspectWriter->Update();
  incidenceWriter->Update();
  exitanceWriter->Update();

  return EXIT_SUCCESS;
}
