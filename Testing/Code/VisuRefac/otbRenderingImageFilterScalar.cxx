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
#include "otbRenderingImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkRGBPixel.h"

int otbRenderingImageFilterScalar( int argc, char * argv[] )
{
  typedef double                                            PixelType;
  typedef otb::Image<PixelType,2>                           ImageType;
  typedef otb::Image<itk::RGBPixel<unsigned char>, 2>       RGBImageType;
  typedef otb::RenderingImageFilter<ImageType,RGBImageType> RenderingFilterType;
  typedef otb::ImageFileReader<ImageType>                   ReaderType;
  typedef otb::StreamingImageFileWriter<RGBImageType>       WriterType;

  // Instanatiation
  ReaderType::Pointer          reader    = ReaderType::New();
  RenderingFilterType::Pointer rendering = RenderingFilterType::New();
  WriterType::Pointer          writer    = WriterType::New();

  // min & max
  PixelType min = atof(argv[3]);
  PixelType max = atof(argv[4]);

  // reading input image
  reader->SetFileName(argv[1]);
  // rendering
  rendering->SetInput(reader->GetOutput());
  rendering->GetRenderingFunction()->SetMinimum(min);
  rendering->GetRenderingFunction()->SetMaximum(max);
  // writing
  writer->SetFileName(argv[2]);
  writer->SetInput(rendering->GetOutput());
  writer->Update();


  return EXIT_SUCCESS;
}
