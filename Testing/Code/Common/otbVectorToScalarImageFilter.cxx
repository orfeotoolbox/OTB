#include <iostream>
#include "itkVector.h"
#include "itkVectorImage.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkVariableLengthVector.h"
#include "itkImageAdaptor.h"

#include "otbVectorToScalarImageFilter.h"


int otbVectorToScalarImageFilter(int argc, char * argv [])
{
  try
  {
  unsigned int  channel((unsigned int)::atoi(argv[3]));

  const unsigned int Dimension = 2;
  
  typedef unsigned char          PixelType;

  typedef itk::VectorImage < PixelType, Dimension>        InImageType;
  typedef itk::Image<PixelType, Dimension> OutImageType;

  typedef itk::ImageFileReader< InImageType, itk::DefaultConvertPixelTraits< PixelType > > ReaderType;
  typedef itk::ImageFileWriter< OutImageType > WriterType;

  typedef otb::VectorToScalarImageFilter< PixelType , PixelType > VectorToScalarFilterType;
  
  ReaderType::Pointer  reader      = ReaderType::New();
  WriterType::Pointer  writer      = WriterType::New();
  VectorToScalarFilterType::Pointer v2scalar = VectorToScalarFilterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->Update();
  v2scalar->SetInput( reader->GetOutput() );
  
  v2scalar->SetChannel(channel);
  
  writer->SetInput( v2scalar->GetOutput() );
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

