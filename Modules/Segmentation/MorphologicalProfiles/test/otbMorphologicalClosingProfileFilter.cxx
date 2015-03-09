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
#include "otbMorphologicalClosingProfileFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMorphologicalClosingProfileFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];
  const unsigned int profileSize = atoi(argv[4]);
  const unsigned int initialValue = atoi(argv[5]);
  const unsigned int step = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef double OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>           ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalClosingProfileFilter<InputImageType, InputImageType, StructuringElementType>
  ClosingProfileFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  ClosingProfileFilterType::Pointer profileFilter = ClosingProfileFilterType::New();
  profileFilter->SetInput(reader->GetOutput());
  profileFilter->SetProfileSize(profileSize);
  profileFilter->SetInitialValue(initialValue);
  profileFilter->SetStep(step);
  profileFilter->Update();

  WriterType::Pointer writer;

  // std::stringstream oss;
  std::ostringstream oss;
  // Writing the results images
  for (unsigned int i = 1; i <= profileSize; ++i)
    {
    writer =  WriterType::New();
    oss << outputFilenamePrefix << i << "." << outputFilenameSuffix;
    writer->SetInput(profileFilter->GetOutput()->GetNthElement(i - 1));
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    }

  return EXIT_SUCCESS;
}
