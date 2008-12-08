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
#include "itkBinaryThresholdImageFilter.h"
#include "otbChangeLabelImageFilter.h"

int otbImageViewerWithOverlay( int argc, char * argv[] )
{
  char * filename = argv[1];
  double upperThresh = atof(argv[2]);
  unsigned char opacity = atoi(argv[3]);


  // Parse command line parameters
  typedef double PixelType;
  typedef otb::ImageViewer<PixelType>  ImageViewerType;
  typedef ImageViewerType::SingleImageType ImageType;
  typedef ImageViewerType::ViewModelType ViewModelType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef ImageViewerType::OverlayImageType OverlayImageType;
  typedef otb::Image<unsigned short,2> LabeledImageType;
  typedef itk::BinaryThresholdImageFilter<ImageType,LabeledImageType> ThresholdFilterType;
  typedef otb::ChangeLabelImageFilter<LabeledImageType,OverlayImageType> ChangeLabelFilterType;
  typedef OverlayImageType::PixelType OverlayPixelType;

  // instantiation
  ImageViewerType::Pointer viewer = ImageViewerType::New();
  ThresholdFilterType::Pointer thresh = ThresholdFilterType::New();
  ChangeLabelFilterType::Pointer chLabel = ChangeLabelFilterType::New();

  // check for input images
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->GenerateOutputInformation();

  thresh->SetInput(reader->GetOutput());
  thresh->SetInsideValue(1);
  thresh->SetOutsideValue(0);
  thresh->SetLowerThreshold(0);
  thresh->SetUpperThreshold(upperThresh);

  chLabel->SetInput(thresh->GetOutput());

  OverlayPixelType insideValue,outsideValue;

  insideValue.SetSize(3);
  insideValue.Fill(0);
  insideValue[0]=255;

  outsideValue.SetSize(3);
  outsideValue.Fill(0);
  outsideValue[2]=255;

  chLabel->SetNumberOfComponentsPerPixel(3);
  chLabel->SetChange(0,outsideValue);
  chLabel->SetChange(1,insideValue);
  chLabel->GetOutput()->UpdateOutputInformation();


  viewer->SetImage(reader->GetOutput());
  viewer->SetImageOverlay(chLabel->GetOutput());
  viewer->SetUseImageOverlay(true);

  // build the app
  viewer->Build();
  viewer->SetImageOverlayOpacity(opacity);
  viewer->SetViewModel(static_cast<ViewModelType>(0));
  viewer->Show();

  Fl::check();

  //  Uncomment to hold display
  //  Fl::run();

  return EXIT_SUCCESS;
}


