/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:	  $Date$
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

int otbImageSeriesFileReader(int argc, char* argv[] )
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();
  parser->AddInputImage();
  parser->AddOption("--OutputImageBaseName","Output Image Base Name","-out", 1, true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << "This program tests the reader of lists through ImageSeriesReader\n";
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser")
         != std::string::npos )
      return EXIT_SUCCESS;
    if(descriptionException.find("ParseCommandLine(): Version Parser")
       != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  const char * inputListName = parseResult->GetInputImage().c_str();

  const  unsigned int Dimension = 2;
  typedef unsigned char PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;
  typedef otb::ImageList< ImageType > ImageListType;

  typedef otb::ImageSeriesFileReader< ImageType > ImageReader;
  ImageReader::Pointer reader = ImageReader::New();
  reader->SetFileName( inputListName );
  reader->Update();


  ossimFilename outputFilenameBase(
         parseResult->GetParameterString("--OutputImageBaseName").c_str());

  ImageListType * imageList = reader->GetOutput();

  typedef otb::StreamingImageFileWriter<ImageType> WriterType;

  for ( unsigned int i = 0; i < imageList->Size(); i++ )
  {
    itk::OStringStream title;
    title << "Image no " << i;
    std::cerr << title.str().c_str() << "\n";

    std::stringstream number;
    number << "-" << i;
    ossimString fileNum(number.str());
    ossimFilename outputFilename;
    outputFilename.merge(outputFilenameBase.drive(),
                          outputFilenameBase.path(),
                          outputFilenameBase.fileNoExtension()+fileNum,
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


