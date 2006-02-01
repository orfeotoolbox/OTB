#include <iostream>
#include "itkVectorImage.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"

#include "otbVectorToRGBImageFilter.h"


int otbVectorToRGBImageFilter(int argc, char * argv [])
{
  try
  {
  unsigned int  channelRed((unsigned int)::atoi(argv[3]));
  unsigned int  channelGreen((unsigned int)::atoi(argv[4]));
  unsigned int  channelBlue((unsigned int)::atoi(argv[5]));

  const unsigned int Dimension = 2;
  
  typedef unsigned char          PixelType;

  typedef itk::RGBPixel< PixelType > RGBPixelType;
  typedef itk::VectorImage < PixelType, Dimension>        ImageType;
  
  typedef itk::Image<RGBPixelType, Dimension> RGBImageType;

  typedef itk::ImageFileReader< ImageType, itk::DefaultConvertPixelTraits< PixelType > > ReaderType;
  typedef itk::ImageFileWriter< RGBImageType > WriterType;


  typedef otb::VectorToRGBImageFilter< PixelType , PixelType > VectorToRGBFilterType;
  
  ReaderType::Pointer  reader      = ReaderType::New();
  WriterType::Pointer  writer      = WriterType::New();
  VectorToRGBFilterType::Pointer v2rgb = VectorToRGBFilterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->Update();
  v2rgb->SetInput( reader->GetOutput() );
  
  v2rgb->SetRedChannel(channelRed);
  v2rgb->SetGreenChannel(channelGreen);
  v2rgb->SetBlueChannel(channelBlue);
  
  writer->SetInput( v2rgb->GetOutput() );
  writer->Update();

    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;

}

