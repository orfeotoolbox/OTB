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

#include "otbNeighborhoodScalarProductFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientRecursiveGaussianImageFilter.h"

int otbNeighborhoodScalarProductFilter(int itkNotUsed(argc), char * argv[])
{

  const char * infname = argv[1];
  const char * outfname = argv[2];
  const char * diroutfname = argv[3];
  const double sigma = atof(argv[4]);

  const unsigned int Dimension = 2;
  typedef double                                                                      PixelType;
  typedef itk::CovariantVector<PixelType, Dimension>                                  VectorPixelType;
  typedef otb::Image<VectorPixelType, Dimension>                                      VectorImageType;
  typedef otb::Image<PixelType, Dimension>                                            ImageType;
  typedef otb::NeighborhoodScalarProductFilter<VectorImageType, ImageType, ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                             ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                             WriterType;
  typedef otb::ImageFileWriter<VectorImageType>                                       TempWriter;
  typedef itk::GradientRecursiveGaussianImageFilter<ImageType, VectorImageType>       GradientFilterType;

  // Instantiating object
  FilterType::Pointer         filter = FilterType::New();
  ReaderType::Pointer         reader = ReaderType::New();
  WriterType::Pointer         writer = WriterType::New();
  GradientFilterType::Pointer gradient = GradientFilterType::New();

  reader->SetFileName(infname);
  reader->Update();

  gradient->SetInput(reader->GetOutput());
  gradient->SetSigma(sigma);
  filter->SetInput(gradient->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
  writer = WriterType::New();
  writer->SetFileName(diroutfname);
  writer->SetInput(filter->GetOutputDirection());
  writer->Update();
  return EXIT_SUCCESS;
}
