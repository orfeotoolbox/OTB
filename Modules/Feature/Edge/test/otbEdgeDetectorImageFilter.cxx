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
#include "otbImage.h"
#include "otbEdgeDetectorImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkSobelEdgeDetectionImageFilter.h"

int otbEdgeDetectorImageFilter(int itkNotUsed(argc), char* argv[])
{
  typedef double                          PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>               SobelDetectionType;
  typedef otb::EdgeDetectorImageFilter<ImageType, ImageType, SobelDetectionType> DetectorType;

  /**Instantiation of an object*/
  ReaderType::Pointer         reader      = ReaderType::New();
  WriterType::Pointer         writer      = WriterType::New();
  DetectorType::Pointer       detector    = DetectorType::New();
  SobelDetectionType::Pointer sobelFilter = SobelDetectionType::New();

  reader->SetFileName(argv[1]);
  detector->SetInput(reader->GetOutput());

  detector->SetLowerThreshold(static_cast<PixelType>(atof(argv[3])));
  detector->SetUpperThreshold(static_cast<PixelType>(atof(argv[4])));

  writer->SetFileName(argv[2]);
  writer->SetInput(detector->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
