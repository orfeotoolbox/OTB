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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbKullbackLeiblerProfileImageFilter.h"

int otbKullbackLeiblerProfileImageFilter(int argc, char * argv[])
{
  if (argc != 6)
    {
    std::cerr <<
    "Detection de changements par mesure de Kullback-Leibler, optimisee par un development de Edgeworth\n";
    std::cerr << argv[0] << " imgAv imgAp imgResu winSizeMin winSizeMax\n";
    return 1;
    }

  char * fileName1 = argv[1];
  char * fileName2 = argv[2];
  char * fileNameOut = argv[3];
  int    winSizeMin = atoi(argv[4]);
  int    winSizeMax = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::Image<PixelType, Dimension>                                              ImageType;
  typedef otb::VectorImage<PixelType, Dimension>                                        VectorImageType;
  typedef otb::KullbackLeiblerProfileImageFilter<ImageType, ImageType, VectorImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                               ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                                         WriterType;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(fileName1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(fileName2);

  FilterType::Pointer filter = FilterType::New();
  filter->SetRadius((winSizeMin - 1) / 2, (winSizeMax - 1) / 2);
  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(fileNameOut);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
