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

#include "otbFltkFilterWatcher.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include "itkGradientMagnitudeImageFilter.h"


int otbFltkFilterWatcher(int argc, char * argv[])
{
        const char * infname = argv[1];
        typedef otb::Image<char,2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  FilterType::Pointer gradient = FilterType::New();
  gradient->SetInput(reader->GetOutput());
        otb::FltkFilterWatcher watcher(gradient,0,0,200,20,"Gradient");
        gradient->Update();
        return EXIT_SUCCESS;
}
