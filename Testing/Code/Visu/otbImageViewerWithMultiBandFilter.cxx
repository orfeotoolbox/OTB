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
#include "otbImageViewer.h"
#include "otbMacro.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbImage.h"


int otbImageViewerWithMultiBandFilter( int argc, char * argv[] )
{
  char * filename = argv[1];

  // Parse command line parameters
  typedef double PixelType;
  typedef otb::ImageViewer<PixelType>  ImageViewerType;
  typedef ImageViewerType::ImageType VectorImageType;
  typedef ImageViewerType::SingleImageType ImageType;
  typedef itk::SobelEdgeDetectionImageFilter<ImageType,ImageType> FilterType;
  typedef otb::PerBandVectorImageFilter<VectorImageType,VectorImageType,FilterType>
  PerBandFilterType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;

  // instantiation
  ImageViewerType::Pointer viewer = ImageViewerType::New();
  PerBandFilterType::Pointer filter = PerBandFilterType::New();

  // check for input images
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);

  filter->SetInput(reader->GetOutput());
  filter->UpdateOutputInformation();

  std::cout<<"Main - number of bands: "<<filter->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;
  std::cout<<"Main - largest region: "<<filter->GetOutput()->GetLargestPossibleRegion()<<std::endl;

  viewer->SetImage(filter->GetOutput());

  // build the app
  viewer->Show();
  Fl::check();

  return EXIT_SUCCESS;
}


