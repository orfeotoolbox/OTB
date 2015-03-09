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

#include <math.h>
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbConvolutionImageFilter.h"
#include "otbOverlapSaveConvolutionImageFilter.h"
#include "otbGaborFilterGenerator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkTimeProbe.h"

int otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter(int argc, char *argv[])
{
  if (argc != 12)
    {
    std::cerr << "Usage: " << argv[0] << " infname outfname1 outfname2 xradius yradius a b theta u0 v0 phi" <<
    std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname =  argv[1];
  const char *       outfname1 = argv[2];
  const char *       outfname2 = argv[3];
  const unsigned int xradius = atoi(argv[4]);
  const unsigned int yradius = atoi(argv[5]);
  const double       a = atof(argv[6]);
  const double       b = atof(argv[7]);
  const double       theta = atof(argv[8]);
  const double       u0 = atof(argv[9]);
  const double       v0 = atof(argv[10]);
  const double       phi = atof(argv[11]);

  typedef double                                   PrecisionType;
  typedef otb::GaborFilterGenerator<PrecisionType> GaborGeneratorType;
  typedef GaborGeneratorType::RadiusType           RadiusType;
  typedef GaborGeneratorType::ArrayType            ArrayType;

  typedef otb::Image<PrecisionType, 2>                                 ImageType;
  typedef otb::ImageFileReader<ImageType>                              ReaderType;
  typedef otb::ImageFileWriter<ImageType>                     WriterType;
  typedef otb::OverlapSaveConvolutionImageFilter<ImageType, ImageType> OSConvolutionFilterType;
  // Setting the same boundary conditions than the one used in the overlap save convolution filter
  typedef itk::ConstantBoundaryCondition<ImageType>                                BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<ImageType, ImageType, BoundaryConditionType> ConvolutionFilterType;

  // Gabor filter generation
  GaborGeneratorType::Pointer gabor = GaborGeneratorType::New();
  RadiusType                  radius;
  radius[0] = xradius;
  radius[1] = yradius;
  gabor->SetRadius(radius);
  gabor->SetA(a);
  gabor->SetB(b);
  gabor->SetTheta(theta);
  gabor->SetU0(u0);
  gabor->SetV0(v0);
  gabor->SetPhi(phi);
  ArrayType filter = gabor->GetFilter();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  itk::TimeProbe probe1, probe2;

  ConvolutionFilterType::Pointer convolution = ConvolutionFilterType::New();
  convolution->SetRadius(radius);
  convolution->SetFilter(filter);
  convolution->SetInput(reader->GetOutput());

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(convolution->GetOutput());
  writer1->SetFileName(outfname1);

  probe1.Start();
  writer1->Update();
  probe1.Stop();

  std::cout << "Classical convolution algorithm took " << probe1.GetMean() << " seconds." << std::endl;

  OSConvolutionFilterType::Pointer osconvolution = OSConvolutionFilterType::New();
  osconvolution->SetRadius(radius);
  osconvolution->SetFilter(filter);
  osconvolution->SetInput(reader->GetOutput());

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(osconvolution->GetOutput());
  writer2->SetFileName(outfname2);

  probe2.Start();
  writer2->Update();
  probe2.Stop();

  std::cout << "Overlap-save convolution algorithm took " << probe2.GetMean() << " seconds." << std::endl;

  return EXIT_SUCCESS;
}
