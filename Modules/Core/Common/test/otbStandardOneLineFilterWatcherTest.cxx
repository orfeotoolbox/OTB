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

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbStandardOneLineFilterWatcher.h"
#include "itkGradientMagnitudeImageFilter.h"

int otbStandardOneLineFilterWatcherTest(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> FilterType;
  FilterType::Pointer gradient = FilterType::New();

  typedef otb::StandardOneLineFilterWatcher WatcherType;
  WatcherType watcher1(gradient, "Gradient");

  gradient->SetInput(reader->GetOutput());
  gradient->Update();

  // Test copy constructor.
  WatcherType watcher2( watcher1 );
  if ( watcher1.GetNameOfClass() != watcher2.GetNameOfClass()
       || watcher1.GetProcess() != watcher2.GetProcess()
       || watcher1.GetComment() != watcher2.GetComment() )
    {
    std::cout << "Copy constructor failed." << std::endl;
    return EXIT_FAILURE;
    }

  // Test default constructor.
  WatcherType watcher3;

  // Test assignment operator.
  watcher3 = watcher2;
  if ( watcher3.GetNameOfClass() != watcher2.GetNameOfClass()
       || watcher3.GetProcess() != watcher2.GetProcess()
       || watcher3.GetComment() != watcher2.GetComment() )
    {
    std::cout << "Operator= failed." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
