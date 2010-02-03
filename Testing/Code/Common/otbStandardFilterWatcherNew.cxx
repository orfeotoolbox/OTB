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

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbStandardFilterWatcher.h"
#include "itkGradientMagnitudeImageFilter.h"

int otbStandardFilterWatcherNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;
  typedef otb::Image<PixelType,Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> FilterType;
  FilterType::Pointer gradient = FilterType::New();

  otb::StandardFilterWatcher watcher(gradient, "Gradient");

  gradient->SetInput(reader->GetOutput());
  gradient->Update();


  return EXIT_SUCCESS;
}
