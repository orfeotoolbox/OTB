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

#include "otbImageToHessianDeterminantImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageToHessianDeterminantImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * ifname   = argv[1];
  const char * outfname = argv[2];
  const double sigma    = atof(argv[3]);

  const unsigned int Dimension = 2;
  typedef double                                                          PixelType;
  typedef otb::Image<PixelType, Dimension>                                ImageType;
  typedef otb::ImageToHessianDeterminantImageFilter<ImageType, ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                 ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                 WriterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(ifname);

  filter->SetInput(reader->GetOutput());
  filter->SetSigma(sigma);

  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
