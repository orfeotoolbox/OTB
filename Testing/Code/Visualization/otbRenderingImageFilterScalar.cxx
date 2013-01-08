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
#include "otbImageFileWriter.h"
#include "itkRGBPixel.h"

int otbRenderingImageFilterScalar(int argc, char * argv[])
{
  typedef double                                                     PixelType;
  typedef otb::Image<PixelType, 2>                                   ImageType;
  typedef otb::Image<itk::RGBPixel<unsigned char>, 2>                RGBImageType;
  typedef otb::RenderingImageFilter<ImageType, RGBImageType>         RenderingFilterType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef otb::ImageFileWriter<RGBImageType>                WriterType;
  typedef RenderingFilterType::RenderingFunctionType::ParametersType ParametersType;

  // Instantiation
  ReaderType::Pointer          reader    = ReaderType::New();
  RenderingFilterType::Pointer rendering = RenderingFilterType::New();
  WriterType::Pointer          writer    = WriterType::New();

  // min & max
//   PixelType min = atof(argv[3]);
//   PixelType max = atof(argv[4]);
  ParametersType parameters(2);
  parameters[0] = atof(argv[3]); //min
  parameters[1] = atof(argv[4]); //max

  // reading input image
  reader->SetFileName(argv[1]);
  // rendering
  rendering->SetInput(reader->GetOutput());
//   rendering->GetRenderingFunction()->SetMinimum(min);
//   rendering->GetRenderingFunction()->SetMaximum(max);
  rendering->GetRenderingFunction()->SetParameters(parameters);
  // writing
  writer->SetFileName(argv[2]);
  writer->SetInput(rendering->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
