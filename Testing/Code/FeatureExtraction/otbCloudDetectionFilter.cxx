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

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbCloudDetectionFunctor.h"
#include "otbCloudDetectionFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbCloudDetectionFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension                                                   = 2;
  typedef double                                                             PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                             VectorImageType;
  typedef otb::Image<PixelType, Dimension>                                   ImageType;
  typedef VectorImageType::PixelType                                         VectorPixelType;
  typedef otb::Functor::CloudDetectionFunctor<VectorPixelType, PixelType>    FunctorType;
  typedef otb::CloudDetectionFilter<VectorImageType, ImageType, FunctorType> CloudDetectionFilterType;
  typedef otb::ImageFileReader<VectorImageType>                              ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                    WriterType;

  //Parameters
  const char *    inputFileName(argv[1]);
  const char *    outputFileName(argv[2]);
  VectorPixelType referencePixel;
  referencePixel.SetSize(4);
  referencePixel.Fill(0.);

  referencePixel[0] = (atof(argv[3]));
  referencePixel[1] = (atof(argv[4]));
  referencePixel[2] = (atof(argv[5]));
  referencePixel[3] = (atof(argv[6]));

  const double variance = (atof(argv[7]));
  const double minThreshold = (atof(argv[8]));
  const double maxThreshold = (atof(argv[9]));

// Instantiating object
  ReaderType::Pointer               reader = ReaderType::New();
  CloudDetectionFilterType::Pointer cloudDetection = CloudDetectionFilterType::New();
  WriterType::Pointer               writer = WriterType::New();

  //Initialization parameters
  reader->SetFileName(inputFileName);

  cloudDetection->SetInput(reader->GetOutput());
  cloudDetection->SetReferencePixel(referencePixel);
  cloudDetection->SetMinThreshold(minThreshold);
  cloudDetection->SetMaxThreshold(maxThreshold);
  cloudDetection->SetVariance(variance);

  writer->SetFileName(outputFileName);
  writer->SetInput(cloudDetection->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
