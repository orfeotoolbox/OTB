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
#include "otbVectorImage.h"
#include "otbImageList.h"
#include "otbImageSeriesFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "base/ossimFilename.h"

#include "otbCommandProgressUpdate.h"
#include "otbCommandLineArgumentParser.h"

int otbImageSeriesFileReader(int argc, char* argv[])
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

  ossimFilename outputFilenameBase(outputFile);

  ImageListType * imageList = reader->GetOutput();

  typedef otb::StreamingImageFileWriter<ImageType> WriterType;

  for (unsigned int i = 0; i < imageList->Size(); i++)
    {
    itk::OStringStream title;
    title << "Image no " << i;
    std::cerr << title.str().c_str() << "\n";

    std::stringstream number;
    number << "-" << i;
    ossimString   fileNum(number.str());
    ossimFilename outputFilename;
    outputFilename.merge("", outputFilenameBase.path(),
                         outputFilenameBase.fileNoExtension() + fileNum,
                         outputFilenameBase.ext()
                         );

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(reader->GetOutput(i));
    writer->Update();

    }

  std::cerr << "Normal Exit\n";

  return EXIT_SUCCESS;
}
