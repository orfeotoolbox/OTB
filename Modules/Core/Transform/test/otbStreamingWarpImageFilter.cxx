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
#include "otbVectorImage.h"
#include "itkVector.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingWarpImageFilter.h"

int otbStreamingWarpImageFilter(int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cout << "usage: " << argv[0] << "infname deffname outfname radius" << std::endl;
    return EXIT_SUCCESS;
    }

  // Input parameters
  const char * infname = argv[1];
  const char * deffname = argv[2];
  const char * outfname = argv[3];
  const double maxdef = atoi(argv[4]);

  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::Image<PixelType, Dimension>            ImageType;
  typedef itk::Vector<PixelType, 2>                   DisplacementValueType;
  typedef otb::Image<DisplacementValueType, Dimension> DisplacementFieldType;

  // Warper
  typedef otb::StreamingWarpImageFilter<ImageType, ImageType, DisplacementFieldType> ImageWarperType;

  // Reader/Writer
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageFileReader<DisplacementFieldType> DisplacementReaderType;
  typedef otb::ImageFileWriter<ImageType>   WriterType;

  // Objects creation
  DisplacementReaderType::Pointer displacementReader = DisplacementReaderType::New();
  ReaderType::Pointer            reader = ReaderType::New();
  WriterType::Pointer            writer = WriterType::New();
  ImageWarperType::Pointer       warper = ImageWarperType::New();

  // Reading
  reader->SetFileName(infname);
  displacementReader->SetFileName(deffname);

  // Warping
  DisplacementValueType maxDisplacement;
  maxDisplacement.Fill(maxdef);
  warper->SetMaximumDisplacement(maxDisplacement);
  warper->SetInput(reader->GetOutput());
  warper->SetDisplacementField(displacementReader->GetOutput());

  // Writing
  writer->SetInput(warper->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
