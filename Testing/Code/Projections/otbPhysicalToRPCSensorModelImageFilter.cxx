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

#include "otbPhysicalToRPCSensorModelImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbPhysicalToRPCSensorModelImageFilter(int argc, char* argv[])
{
  const char * infname  = argv[1];
  unsigned int gridSpacing = atoi(argv[2]);
  const char * outfname  = argv[3];
  
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  
  typedef otb::PhysicalToRPCSensorModelImageFilter<ImageType> PhysicalToSensorModelType;

  // Object instanciation
  PhysicalToSensorModelType::Pointer estimator = PhysicalToSensorModelType::New();
  ReaderType::Pointer                reader    = ReaderType::New();
  
  // Set the fileName
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  
  estimator->SetInput(reader->GetOutput());
  estimator->SetGridSpacing(gridSpacing);

  // Write the resampled image
  typedef otb::StreamingImageFileWriter<ImageType>    WriterType;
  WriterType::Pointer writer= WriterType::New();
  writer->SetTilingStreamDivisions();
  writer->WriteGeomFileOn();
  writer->SetFileName(outfname);
  writer->SetInput(estimator->GetOutput());
  writer->Update();
  
  return 0;
}
