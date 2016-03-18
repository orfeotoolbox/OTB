
#include <fstream>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThresholdImageFilter.h"



int main(int argc, char * argv[] )
{

  if( argc < 5 )
  {
    std::cerr << "BinaryThresholdFilter  inputFile outputFile lowerThreshold upperThreshold" << std::endl;
    return -1;
  }
  
  typedef signed short                          InputPixelType;
  typedef itk::Image< InputPixelType,     3 >   InputImageType;


  typedef unsigned char                          OutputPixelType;
  typedef itk::Image< OutputPixelType,     3 >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType  >   ImageReaderType;
  typedef itk::ImageFileWriter< OutputImageType >   ImageWriterType;

  typedef itk::BinaryThresholdImageFilter< 
                                    InputImageType,
                                    OutputImageType
                                              >  FilterType;

  FilterType::Pointer filter = FilterType::New();

  filter->ReleaseDataFlagOn();

  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName( argv[1] );


  try
    {
    imageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return -1;
    }

  filter->SetInput( imageReader->GetOutput() );


  filter->SetInsideValue( 255 );
  filter->SetOutsideValue( 0  );

  const InputPixelType lowerThreshold = atoi( argv[3] );
  const InputPixelType upperThreshold = atoi( argv[4] );

  filter->SetLowerThreshold( lowerThreshold );
  filter->SetUpperThreshold( upperThreshold );


 
  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return -1;
    }

  ImageWriterType::Pointer imageWriter = ImageWriterType::New();

  imageWriter->SetFileName( argv[2] );

  imageWriter->SetInput( filter->GetOutput() );


  try
    {
    imageWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return -1;
    }

 
  return 0;
}

