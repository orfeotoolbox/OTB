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
#include "otbKMeansImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbKMeansImageClassificationFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const unsigned int nbClasses = atoi(argv[3]);

  const unsigned int Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, Dimension>                            ImageType;
  typedef otb::Image<LabeledPixelType, Dimension>                           LabeledImageType;
  typedef otb::KMeansImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
  typedef ClassificationFilterType::KMeansParametersType                    KMeansParametersType;
  typedef otb::ImageFileReader<ImageType>                                   ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType>                   WriterType;

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  const unsigned int   sampleSize = ClassificationFilterType::MaxSampleDimension;
  const unsigned int   parameterSize = nbClasses * sampleSize;
  KMeansParametersType parameters;

  parameters.SetSize(parameterSize);
  parameters.Fill(0);

  for (unsigned int i = 0; i < nbClasses; ++i)
    {
    for (unsigned int j = 0; j < reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++j)
      {
      parameters[i * sampleSize + j] = atof(argv[4 + i * reader->GetOutput()->GetNumberOfComponentsPerPixel() + j]);
      }
    }

  std::cout << "Parameters: " << parameters << std::endl;

  filter->SetCentroids(parameters);
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
