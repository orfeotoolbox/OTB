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

#include "otbLabelToBoundaryImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbLabelToBoundaryImageFilter(int argc, char * argv [])
{
  typedef unsigned int InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::LabelToBoundaryImageFilter
  <InputImageType, OutputImageType> BoundaryFilterType;
  typedef BoundaryFilterType::Pointer BoundaryFilterPointerType;

  if (argc != 3)
  {
    std::cerr << "Usage : "<<argv[0]<<"  input_label_image  output_boundary"<< std::endl;
    return EXIT_FAILURE;
  }

  std::string inputPath(argv[1]);
  std::string outputPath(argv[2]);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputPath);

  BoundaryFilterPointerType filter = BoundaryFilterType::New();
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outputPath);
  writer->Update();

  return EXIT_SUCCESS;
}
