#include <iostream>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"

template<typename PixelType>
int generic_main_concatenate(otb::CommandLineArgumentParseResult* parseResult)
{
  const unsigned int NbImages = parseResult->GetNumberOfParameters("--InputImagesList");

  std::ofstream textFile;

  if (parseResult->IsOptionPresent("--OutputNameList"))
    {
    std::string textName = parseResult->GetParameterString("--OutputNameList");
    textFile.open(textName.c_str());
    }

  std::cout << "Concat of " << NbImages << " images into a multi-band image " << std::endl;

  const unsigned int Dimension = 2;
  typedef otb::Image<PixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;
  typedef otb::ObjectList<ImageReaderType> ReaderListType;

  typename ReaderListType::Pointer readerList = ReaderListType::New();

  typedef otb::ImageList<InputImageType> ImageListType;
  typename ImageListType::Pointer imageList = ImageListType::New();

  for (unsigned int i = 0; i < NbImages; i++)
    {
    typename ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetFileName(parseResult->GetParameterString("--InputImagesList", i).c_str());

    std::cout << "Adding image " << parseResult->GetParameterString("--InputImagesList", i).c_str() << std::endl;
    textFile << parseResult->GetParameterString("--InputImagesList", i) << "\n";

    imageReader->UpdateOutputInformation();

    imageList->PushBack(imageReader->GetOutput());
    readerList->PushBack(imageReader);
    }

  textFile.close();

  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> ImageListToVectorImageFilterType;
  typename ImageListToVectorImageFilterType::Pointer iL2VI = ImageListToVectorImageFilterType::New();

  iL2VI->SetInput(imageList);

  typedef otb::StreamingImageFileWriter<VectorImageType> ImageWriterType;
  typename ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  imageWriter->SetFileName(parseResult->GetOutputImage().c_str());

  unsigned long size = (10000 * 10000 * sizeof(PixelType)) / NbImages;

  std::cout << "Streaming size: " << size << std::endl;

  imageWriter->SetBufferMemorySize(size);
  imageWriter->SetInput(iL2VI->GetOutput());
  imageWriter->Update();

  return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Concatenate n images in a multiband image");
  parser->AddOutputImage();
  parser->AddOptionNParams("--InputImagesList", "Images list to concatenate", "-il", true);
  parser->AddOption("--OutputPixelType",
                    "OutputPixelType: unsigned char (1), short int (2), int (3), float (4), double (5), unsigned short int (12), unsigned int (13); default 1",
                    "-t", 1, false);
  parser->AddOption("--OutputNameList",
                    "Text file containing the name of the images used to generate the output in the same order",
                    "-ot", 1, false);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer parseResult = ParserResultType::New();

  try
    {
    parser->ParseCommandLine(argc, argv, parseResult);
    }
  catch (itk::ExceptionObject & err)
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

  unsigned int type = 2; //default to short int as the original program
  if (parseResult->IsOptionPresent("--OutputPixelType"))
    {
    type = parseResult->GetParameterUInt("--OutputPixelType");
    }

  switch (type)
    {
    case 1:
      generic_main_concatenate<unsigned char> (parseResult);
      break;
    case 2:
      generic_main_concatenate<short int> (parseResult);
      break;
    case 3:
      generic_main_concatenate<int> (parseResult);
      break;
    case 4:
      generic_main_concatenate<float> (parseResult);
      break;
    case 5:
      generic_main_concatenate<double> (parseResult);
      break;
    case 12:
      generic_main_concatenate<unsigned short int> (parseResult);
      break;
    case 13:
      generic_main_concatenate<unsigned int> (parseResult);
      break;
    default:
      generic_main_concatenate<unsigned char> (parseResult);
      break;
    }

  return EXIT_SUCCESS;
}

