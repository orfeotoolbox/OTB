
#include <fstream>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"
#include "itkVectorConfidenceConnectedImageFilter.h"



int main(int argc, char * argv[] )
{

  if( argc < 6 )
  {
    std::cerr << "VWColorSegmentation  inputFile outputFile seedsFile multiplier numberOfIterations" << std::endl;
    return -1;
  }
  
  typedef unsigned char                         PixelComponentType;
  typedef itk::RGBPixel<PixelComponentType>     ImagePixelType;
  typedef itk::Image< ImagePixelType,     3 >   ImageType;


  typedef unsigned char                          OutputPixelType;
  typedef itk::Image< OutputPixelType,     3 >   OutputImageType;

  typedef itk::ImageFileReader< ImageType  >        ImageReaderType;
  typedef itk::ImageFileWriter< OutputImageType >   ImageWriterType;

  typedef itk::VectorConfidenceConnectedImageFilter< 
                                              ImageType,
                                              OutputImageType
                                              >  ConfidenceConnectedFilterType;

  ConfidenceConnectedFilterType::Pointer 
                          confidenceFilter = ConfidenceConnectedFilterType::New();

  confidenceFilter->ReleaseDataFlagOn();

  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName( argv[1] );

  const unsigned int VectorDimension = 3;

  try
    {
    imageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return -1;
    }

  confidenceFilter->SetInput( imageReader->GetOutput() );


  confidenceFilter->SetReplaceValue( 255 );
  confidenceFilter->SetNumberOfIterations( atoi( argv[5] ) );
  confidenceFilter->SetMultiplier( atof( argv[4] ) );

  std::ifstream seedsFile;
  seedsFile.open( argv[3] );

  if( seedsFile.fail() )
    {
    std::cerr << "Problem opening seeds file " << std::endl;  
    }
      
  ImageType::IndexType index;

  float x;
  float y;
  float z;

  seedsFile >>  x >> y >> z;

  index[0] = static_cast<signed long>( x );
  index[1] = static_cast<signed long>( y );
  index[2] = static_cast<signed long>( z );

  while( !seedsFile.eof() )
    {
    confidenceFilter->AddSeed( index );
    seedsFile >>  x >> y >> z;
    index[0] = static_cast<signed long>( x );
    index[1] = static_cast<signed long>( y );
    index[2] = static_cast<signed long>( z );
    }
  
  seedsFile.close();
  
  try
    {
    confidenceFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return -1;
    }

  ImageWriterType::Pointer imageWriter = ImageWriterType::New();

  imageWriter->SetFileName( argv[2] );

  imageWriter->SetInput( confidenceFilter->GetOutput() );


  try
    {
    imageWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return -1;
    }


  
  #ifdef DEBUGSEGFAULTING
  const ConfidenceConnectedFilterType::CovarianceMatrixType & covariance = 
                                           confidenceFilter->GetCovariance();

  const ConfidenceConnectedFilterType::MeanVectorType  & mean = 
                                                 confidenceFilter->GetMean();


  // Write the mean and covariance to a file
  std::ofstream ofs;
  ofs.open("ConfidenceConnectedColorSegmentation.dat");
  
  for(unsigned int ii=0; ii<VectorDimension; ii++)
    {
    ofs << mean[ii] << "  ";
    std::cout << mean[ii] << "  ";
    }
  ofs << std::endl;
  std::cout << std::endl;

  for(unsigned int ki=0; ki<VectorDimension; ki++)
    {
    for(unsigned int kj=0; kj<VectorDimension; kj++)
      {
      ofs << covariance[ki][kj] << "  ";
      std::cout << covariance[ki][kj] << "  ";
      }
    ofs << std::endl;
    std::cout << std::endl;
    }

  ofs.close();
  #endif
 
  return 0;
}

