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
#include "itkMacro.h"
#include <iostream>
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

int otbImageFileWriterWithExtendedOptionBox(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const std::string inputFilename = argv[1];
  const std::string outputFilename = argv[2];

  const unsigned int startx = atoi(argv[3]);
  const unsigned int starty = atoi(argv[4]);
  const unsigned int sizex = atoi(argv[5]);
  const unsigned int sizey = atoi(argv[6]);
  const unsigned int ram = atoi(argv[7]);
  const std::string separator = ":";

  typedef float InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> InputImageType;
  typedef otb::MultiChannelExtractROI<InputImageType::InternalPixelType,
                                      InputImageType::InternalPixelType> ExtractROIFilterType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  typedef itk::ImageRegionConstIterator< InputImageType >  ConstIteratorType;

  ReaderType::Pointer reader = ReaderType::New();
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);

  //Build output filename with extended box
  std::ostringstream outputFilenameExtended;
  outputFilenameExtended
                         << outputFilename
                         << "?&box=" << startx << separator
                         << starty << separator
                         << sizex << separator
                         << sizey
;

  std::cout << "Output image with user defined path " << outputFilenameExtended.str() << std::endl;

  writer->SetFileName(outputFilenameExtended.str());
  writer->SetInput(reader->GetOutput());
  writer->SetAutomaticAdaptativeStreaming(ram);

  extractROIFilter->SetStartX(startx);
  extractROIFilter->SetStartY(starty);
  extractROIFilter->SetSizeX(sizex);
  extractROIFilter->SetSizeY(sizey);
  extractROIFilter->SetInput(reader->GetOutput());

  writer->Update();
  extractROIFilter->Update();

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(outputFilename);

  reader2->Update();

  InputImageType::ConstPointer readImage = reader2->GetOutput();
  InputImageType::ConstPointer extractImage = extractROIFilter->GetOutput();

  ConstIteratorType ritr( readImage, readImage->GetLargestPossibleRegion() );
  ConstIteratorType extractitr( extractImage, extractImage->GetLargestPossibleRegion() );

  ritr.GoToBegin();
  extractitr.GoToBegin();

  std::cout << "Comparing the pixel values.. :" << std::endl;

  while( !ritr.IsAtEnd() &&  !extractitr.IsAtEnd() )
    {
    if( ritr.Get() != extractitr.Get() )
      {
      std::cerr << "Pixel comparison failed at index = " << ritr.GetIndex() << std::endl;
      std::cerr << "Expected pixel value " << extractitr.Get() << std::endl;
      std::cerr << "Read Image pixel value " << ritr.Get() << std::endl;
      return EXIT_FAILURE;
      }
    ++extractitr;
    ++ritr;
    }

  std::cout << std::endl;
  std::cout << "Test PASSED !" << std::endl;

  return EXIT_SUCCESS;
}
