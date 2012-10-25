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
#include "otbMorphologicalProfilesSegmentationFilter.h"
#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbMorphologicalProfilesSegmentationFilter(int argc, char * argv[])
{
  if (argc != 6 )
    {
    std::cerr << "Usage: " << argv[0] <<
    " inputFileName outputFileName start step size"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       inputFileName             = argv[1];
  const char *       outputFileName            = argv[2];
  const unsigned int start                     = atoi(argv[3]);
  const unsigned int step                      = atoi(argv[4]);
  const unsigned int size                      = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef float                                            PixelType;
  typedef otb::Image<PixelType, Dimension>                 ImageType;
  typedef otb::Image<unsigned int, Dimension>              LabelImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::ImageFileWriter<LabelImageType>             LabelWriterType;

  typedef otb::MorphologicalProfilesSegmentationFilter<ImageType, LabelImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFileName);

  // Set filter parameters
  filter->SetProfileStart(start);
  filter->SetProfileSize(size);
  filter->SetProfileStep(step);
  filter->SetInput(reader->GetOutput());

  //filter->SetNumberOfThreads(1);
  LabelWriterType::Pointer writerLabel = LabelWriterType::New();

  writerLabel->SetFileName(outputFileName);
  writerLabel->SetInput(filter->GetOutput());

  writerLabel->Update();

  return EXIT_SUCCESS;
}
