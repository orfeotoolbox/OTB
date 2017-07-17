#include <iostream>
#include <math.h>
#include <vector>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageToHistogramFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkComposeImageFilter.h"

int const sizeh = 256;
typedef int                   PixelType;
typedef itk::Image< PixelType , 2 >  ImageType;
typedef itk::VectorImage< PixelType , 2 >  VectorImageType;
typedef itk::Statistics::ImageToHistogramFilter< ImageType > HistogramFilterType;
typedef HistogramFilterType::HistogramType HistogramType;

void
equalized( std::array< int , sizeh > const inputHisto,
           std::array< int , sizeh > const targethisto,
           std::array< int , sizeh > & lut)

{
  int countMapValue = 0 ;
  int countValue = 0;
  int countInput  = inputHisto[ countValue ];
  int countTarget = targethisto[ countMapValue ];
  while ( countMapValue<256 )
    {
    if (countInput > countTarget)
      {
      ++countMapValue;
      countTarget += targethisto[countMapValue];
      }
    else
      {
      lut[countValue] =  countMapValue ;
      ++countValue;
      countInput  += inputHisto[ countValue ];
      }
    }
}

void
computehisto( ImageType::Pointer const input,
              std::array<int , sizeh> & inputHisto)
{
  itk::ImageRegionConstIterator< ImageType > it ( input , input->GetRequestedRegion() );
  it.GoToBegin();
  while ( !it.IsAtEnd() )
    {
    ++inputHisto[ it.Get() ];
    ++it;
    }
}

void
createTarget( std::array< int , sizeh > & targetHisto,
              ImageType::Pointer const input )
{
  ImageType::SizeType size = input->GetLargestPossibleRegion().GetSize();
  int nbPixel = size[0] * size[1];
  int height = nbPixel/256 ;
  targetHisto.fill( height );
}

int
main( int argc, 
      char const *argv[] )
{
  // const char * inputfilename  = argv[ 1 ];
  // const char * outputfilename = argv[ 2 ];
  if (argc<3)
  {
    std::cout<<"error"<<std::endl;
    return 1;
  }

  typedef otb::ImageFileReader< VectorImageType > ReaderType;
  typedef itk::VectorIndexSelectionCastImageFilter< VectorImageType , ImageType > VectorToImageFilterType;
  typedef itk::Image< float , 2 > ImageGainType;
  typedef itk::VectorImage< float , 2 >  VectorGainType;
  typedef itk::ComposeImageFilter< ImageGainType > ImageGainToVectorImageGainFilterType;
  typedef itk::ComposeImageFilter< ImageType > ImageToVectorImageFilterType;
  typedef itk::MultiplyImageFilter< ImageType, ImageGainType, ImageType > MultiplyImageFilterType;

  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  reader->Update();
  VectorImageType::Pointer input = reader->GetOutput();

  ImageGainToVectorImageGainFilterType::Pointer imageGainToVectorImageGainFilterOut( ImageGainToVectorImageGainFilterType::New() );
  ImageToVectorImageFilterType::Pointer imageToVectorImageFilterOut( ImageToVectorImageFilterType::New() );
  VectorToImageFilterType::Pointer vectorToImageFilter ( VectorToImageFilterType::New() );
  vectorToImageFilter->SetInput( input );
  vectorToImageFilter->SetIndex( 0 );
  // Hack to get quick result
  HistogramFilterType::Pointer histofilter( HistogramFilterType::New() );
  histofilter->SetInput( vectorToImageFilter->GetOutput() );
  histofilter->Update();
  
  std::array< int , sizeh > hTarget;
  createTarget( hTarget , vectorToImageFilter->GetOutput() );
  int m = input->GetVectorLength ();
  for (int chanel = 0 ; chanel<m ; chanel++ ) 
    {
    MultiplyImageFilterType::Pointer gainMultiplyer ( MultiplyImageFilterType::New() );
    vectorToImageFilter->SetIndex( chanel );
    vectorToImageFilter->Update();
    ImageGainType::Pointer gainImage ( ImageGainType::New() );
    gainImage->SetRegions( vectorToImageFilter->GetOutput()->GetRequestedRegion() );
    gainImage->Allocate();
    gainImage->SetOrigin( vectorToImageFilter->GetOutput()->GetOrigin() );
    itk::ImageRegionIterator< ImageGainType > itgain( gainImage , gainImage->GetRequestedRegion() );
    std::array< int , sizeh > hInput;
    hInput.fill(0);
    computehisto( vectorToImageFilter->GetOutput() , hInput );

    // histofilter->SetInput( vectorToImageFilter->GetOutput() );
    // histofilter->Update();
    // HistogramType::Pointer hInput = histofilter->GetOutput();
    
    std::array< int , sizeh > lut;
    equalized( hInput , hTarget , lut );
    itk::ImageRegionIterator< ImageType > itin( vectorToImageFilter->GetOutput() , vectorToImageFilter->GetOutput()->GetRequestedRegion() );
    itin.GoToBegin();
    itgain.GoToBegin();
    while( !itin.IsAtEnd() )
      {
      itgain.Set( static_cast<float>(lut[itin.Get()]) / static_cast<float>( itin.Get() ) ) ;
      // itin.Set(lut[itin.Get()]);
      ++itin;
      ++itgain;
      }
    // gainImage->SetOrigin( vectorToImageFilter->GetOutput() );
    // gainImage->SetSpacing( vectorToImageFilter->GetOutput() );

    gainMultiplyer->SetInput1( vectorToImageFilter->GetOutput() );
    gainMultiplyer->SetInput2( gainImage );
    gainMultiplyer->Update();
    imageGainToVectorImageGainFilterOut->SetInput( chanel , gainImage );
    imageToVectorImageFilterOut->SetInput( chanel , gainMultiplyer->GetOutput() );
    // imageToVectorImageFilterOut->SetInput( chanel , vectorToImageFilter->GetOutput() ); 
    }

  // Here we compute a gain image corresponding to the associated gain of the equalization
  VectorGainType::Pointer gainVector = imageGainToVectorImageGainFilterOut->GetOutput();
  typedef otb::ImageFileWriter<VectorImageType>  WriterType;
  WriterType::Pointer writer( WriterType::New());
  writer->SetFileName( argv[2] );
  writer->SetInput( imageToVectorImageFilterOut->GetOutput() );
  writer->Update();

  typedef otb::ImageFileWriter<VectorGainType>  WriterGainType;
  WriterGainType::Pointer writergain( WriterGainType::New() );
  writergain->SetFileName( argv[3] );
  writergain->SetInput( gainVector );
  writergain->Update();
  return 0;
}
