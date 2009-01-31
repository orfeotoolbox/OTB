#include <iostream>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"


int main(int argc, char* argv[] )
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Concatenate n images in a multiband image");
  parser->AddOutputImage();
  parser->AddOption("--InputImagesList","Images list to concatenate","-il",argc-4,true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine(argc,argv,parseResult);
  }
  catch ( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
    {
      return EXIT_SUCCESS;
    }
    if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
    {
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
  }

  const unsigned int NbImages = argc-4;

  std::cout << "Concat of " << NbImages << " images into a multi-band image " <<
            std::endl;

  typedef unsigned short int PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image< PixelType, Dimension > InputImageType;

  typedef otb::ImageFileReader< InputImageType > ImageReaderType;

  typedef otb::ObjectList< ImageReaderType > ReaderListType;

  ReaderListType::Pointer readerList = ReaderListType::New();

  typedef otb::ImageList< InputImageType > ImageListType;

  ImageListType::Pointer imageList = ImageListType::New();

  for (unsigned int i = 0; i<NbImages; i++)
  {

    ImageReaderType::Pointer imageReader = ImageReaderType::New();

    imageReader->SetFileName(parseResult->GetParameterString("--InputImagesList",i).c_str()  );

    std::cout << "Adding image " <<parseResult->GetParameterString("--InputImagesList",i).c_str()  << std::endl;

    imageReader->UpdateOutputInformation();

    imageList->PushBack( imageReader->GetOutput() );

    readerList->PushBack( imageReader );

  }

  typedef otb::VectorImage< PixelType, Dimension > VectorImageType;

  typedef otb::ImageListToVectorImageFilter< ImageListType, VectorImageType >
  ImageListToVectorImageFilterType;

  ImageListToVectorImageFilterType::Pointer iL2VI =
    ImageListToVectorImageFilterType::New();

  iL2VI->SetInput( imageList );

  typedef otb::StreamingImageFileWriter< VectorImageType > ImageWriterType;

  ImageWriterType::Pointer imageWriter = ImageWriterType::New();

  imageWriter->SetFileName(parseResult->GetOutputImage().c_str());


  unsigned long size = (10000 * 10000 * sizeof(PixelType)) / NbImages;

  std::cout<<"Streaming size: "<<size<<std::endl;

  imageWriter->SetBufferMemorySize(size);

  imageWriter->SetInput( iL2VI->GetOutput() );

  imageWriter->Update();



  return 0;
}
