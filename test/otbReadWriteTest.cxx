/*=========================================================================

   Program:   ORFEO Toolbox
   Language:  C++
   Date:      $Date$
   Version:   $Revision$


   Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
   See OTBCopyright.txt for details.

   Some parts of this code are derived from ITK. See ITKCopyright.txt
   for details.


      This software is distributed WITHOUT ANY WARRANTY; without even
      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
      PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Includes
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <cstdlib>
#include <iostream>
#include <boost/chrono/thread_clock.hpp>

// Test
int otbReadWriteTest(int argc, char * argv[]) 
{
  // Start chrono
  boost::chrono::thread_clock::time_point startTimer = boost::chrono::thread_clock::now();
  
  // Verify the number of parameters in the command line
  if (argc != 3)
  {
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " inputImageFile outputImageFile " << std::endl;
    return EXIT_FAILURE;
  } 

  // Image types
  typedef float PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType,  Dimension> ImageType;

  // Reader
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  // Reader configuration
  std::string inputFilename = std::string(argv[1]);
  reader->SetFileName(inputFilename);

  // Writer
  typedef otb::ImageFileWriter<ImageType>  WriterType;
  WriterType::Pointer writer = WriterType::New();

  // Writer configuration
  std::string outputFilename = std::string(argv[2]);
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
  
  // Execute the pipeline
  try{
    writer->Update();
  }
  catch (std::exception & err) {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  
  // End chrono
  boost::chrono::thread_clock::time_point stopTimer = boost::chrono::thread_clock::now();
  std::cout << "Duration = " << boost::chrono::duration_cast<boost::chrono::milliseconds>(stopTimer-startTimer).count() <<" ms\n";
  
  return EXIT_SUCCESS;
}

