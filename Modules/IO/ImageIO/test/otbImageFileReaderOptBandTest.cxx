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
#include "otbExtern.h"
#include "otbImage.h"
#include "itkMacro.h"
#include <iostream>
#include <fstream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtendedFilenameToReaderOptions.h"

int otbImageFileReaderOptBandTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ExtendedFilenameToReaderOptions FilenameHelperType;
  FilenameHelperType::Pointer helper = FilenameHelperType::New();

  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];



  helper->SetExtendedFileName(inputFilename);
  unsigned int nbBands = atoi(argv[3]);
  std::vector<unsigned int> bandList;
  bool ret = helper->ResolveBandRange(helper->GetBandRange(),nbBands,bandList);
  if (ret)
    {
    std::cout << "BandList = [";
    for (unsigned int k=0 ; k<bandList.size() ; k++)
      {
      if (k) std::cout << ",";
      std::cout << bandList[k];
      }
    std::cout << "]" << std::endl;
    }
  else
    {
    std::cout << "Invalid band range for a "<<nbBands<<" bands image"<< std::endl;
    }
  
  
  typedef unsigned int PixelType;
  const unsigned int Dimension = 2;
  
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  
  typedef otb::ImageFileReader<ImageType> ReaderType; 
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
 
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
