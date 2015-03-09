/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

         This software is distributed WITHOUT ANY WARRANTY; without even
         the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
         PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageSeriesFileReader.h"
#include "otbImageFileWriter.h"
#include "itksys/SystemTools.hxx"

int otbImageSeriesFileReader(int itkNotUsed(argc), char* argv[])
{
  const char * enviMetaFile = argv[1];
  const char * outputFile   = argv[2];

  const unsigned int Dimension = 2;
  typedef unsigned char                          PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageList<ImageType>              ImageListType;

  typedef otb::ImageSeriesFileReader<ImageType> ImageReader;
  ImageReader::Pointer reader = ImageReader::New();
  reader->SetFileName(enviMetaFile);
  reader->Update();

  std::cout << reader << std::endl;

  ImageListType * imageList = reader->GetOutput();

  typedef otb::ImageFileWriter<ImageType> WriterType;

  for (unsigned int i = 0; i < imageList->Size(); ++i)
    {
    std::cout << "Image no " << i << "\n";

    std::string path = itksys::SystemTools::GetFilenamePath(outputFile);
    std::string name = itksys::SystemTools::GetFilenameWithoutExtension(outputFile);
    std::string ext = itksys::SystemTools::GetFilenameExtension(outputFile);
    std::stringstream outputFilenameStream;
    outputFilenameStream << path << "/" << name << "-" << i << ext;

    std::string outputFilename = itksys::SystemTools::ConvertToOutputPath(
      outputFilenameStream.str().c_str());
    std::cout << "saved in " << outputFilename << "\n";
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(reader->GetOutput(i));
    writer->Update();

    }

  return EXIT_SUCCESS;
}
