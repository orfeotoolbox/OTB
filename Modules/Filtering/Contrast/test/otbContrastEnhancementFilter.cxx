#include <iostream>
#include <math.h>
#include <vector>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkComposeImageFilter.h"

int const sizeh = 256;
typedef int                   PixelType;
typedef itk::Image< PixelType , 2 >  ImageType;
typedef itk::VectorImage< PixelType , 2 >  VectorImageType;

void
equalized( const std::array< int , sizeh > & inputHisto,
           const std::array< int , sizeh > & targetHisto,
           std::array< int , sizeh > & lut)

{
  int countMapValue = 0 ;
  int countValue = 0;
  int countInput  = inputHisto[ countValue ];
  int countTarget = targetHisto[ countMapValue ];
  while ( countMapValue<256 )
    {
    if (countInput > countTarget)
      {
      ++countMapValue;
      countTarget += targetHisto[countMapValue];
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
equalized(std::array< int , sizeh > gridHisto[],
          std::array< int , sizeh > gridLut[],
          const std::array< int , sizeh > & targetHisto,
          int nW,
          int nH)
{
  for (int i = 0 ; i<nH ; i++)
    {
    for ( int j = 0 ; j<nW ; j++ )
      {
      equalized( gridHisto[i * nW + j] , targetHisto , gridLut[i * nW + j]);
      }
    }
}

void
computehisto( ImageType::Pointer const input,
              std::array< int , sizeh > inputHisto[],
              int wThumbnail,
              int hThumbnail,
              int nW,
              int nH)
{
  ImageType::IndexType index;
  for (int i = 0 ; i < wThumbnail * nW ; i++)
    {
    for (int j = 0 ; j< hThumbnail * nH ; j++)
      {
      index[0] = i;
      index[1] = j;
      ++inputHisto[ (i / wThumbnail) * nW + j / hThumbnail][ input->GetPixel( index ) ];
      }
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

void
createTarget( std::array< int , sizeh > & targetHisto,
              int h,
              int l )
{
  int nbPixel = h * l;
  int height = nbPixel/256 ;
  targetHisto.fill( height );
}

int
main( int argc, 
      char const *argv[] )
{
  // const char * inputfilename  = argv[ 1 ];
  // const char * outputfilename = argv[ 2 ];
  if (argc<6)
  {
    std::cout<<"error : otbContrastTestDriver -inputFilename -outputFilename -gainImageFilename -h -l"<<std::endl;
    if (argc>1)
      {
        typedef otb::ImageFileReader< VectorImageType > ReaderType;
        ReaderType::Pointer reader( ReaderType::New() );
        reader->SetFileName( argv[ 1 ] );
        reader->Update();
        VectorImageType::Pointer input = reader->GetOutput();
        std::cout<<"Image Width = "<<input->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
        std::cout<<"Image Height = "<<input->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
      }
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
  vectorToImageFilter->Update();
  int hThumbnail = atoi(argv[4]);
  int wThumbnail = atoi(argv[5]);
  ImageType::Pointer inputImage = vectorToImageFilter->GetOutput();
  if ( inputImage->GetLargestPossibleRegion().GetSize()[1]%wThumbnail != 0 )
    {
    std::cout<<"error : l is not a divider of the input's width"<<std::endl;
    std::cout<<"Image Height = "<<input->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
    return 1;
    }
  if ( inputImage->GetLargestPossibleRegion().GetSize()[0]%hThumbnail != 0 )
    {
    std::cout<<"error : h is not a divider of the input's height"<<std::endl;
    std::cout<<"Image Width = "<<input->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
    return 1;
    }

  int nH = inputImage->GetLargestPossibleRegion().GetSize()[0]/hThumbnail;
  std::cout<<"nH ="<<inputImage->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
  int nW = inputImage->GetLargestPossibleRegion().GetSize()[1]/wThumbnail;
  std::cout<<"nW ="<<nW<<std::endl;
  std::array< int , sizeh > histoTarget;
  // createTarget( histoTarget , vectorToImageFilter->GetOutput() );
  createTarget( histoTarget , hThumbnail , wThumbnail );
  int m = input->GetVectorLength ();

  for (int chanel = 0 ; chanel<m ; chanel++ ) 
    {
    MultiplyImageFilterType::Pointer gainMultiplyer ( MultiplyImageFilterType::New() );
    vectorToImageFilter->SetIndex( chanel );
    vectorToImageFilter->Update();
    ImageGainType::Pointer gainImage ( ImageGainType::New() );
    gainImage->SetRegions( inputImage->GetRequestedRegion() );
    gainImage->Allocate();
    gainImage->SetOrigin( inputImage->GetOrigin() );

    std::array< int , sizeh > lutGrid[(nH)*(nW)] = {};
    

    // lutGrid[0][0].fill(0);
    std::array< int , sizeh >  histoGrid [(nH)*(nW)] = {};
    std::cout<<nH*nW<<std::endl;
    for (int i = 0 ; i<(nH)*(nW) ; i++)
    {
      lutGrid[i].fill(0);
      histoGrid[i].fill(0);
      // for (int j = 0 ; j<50 ; j++)
      // {
      // std::cout<<i<<"   "<<j<<"   "<<histoGrid[i][j]<<std::endl;
      // }
    }
    // for (int i = 0 ; i<50 ; i++)
    // {
      // std::cout<<i<<"   "<<histoGrid[0][i]<<std::endl;
    // }

    computehisto( inputImage , histoGrid , wThumbnail , hThumbnail , nW , nH);

    // for (int i = 0 ; i<50 ; i++)
    // {
    //   std::cout<<i<<"   "<<histoGrid[0][i]<<std::endl;
    // }

    equalized( histoGrid , lutGrid , histoTarget , nW , nH);
    

    /*std::array< int , sizeh > hInput;
    hInput.fill(0);
    computehisto( inputImage , hInput );
    std::array< int , sizeh > lut;
    lut.fill(0);
    equalized( hInput , histoTarget , lut );*/
    
    /*itk::ImageRegionIterator< ImageType > itin( inputImage , inputImage->GetRequestedRegion() );
    itk::ImageRegionIterator< ImageGainType > itgain( gainImage , gainImage->GetRequestedRegion() );
    itin.GoToBegin();
    itgain.GoToBegin();
    while( !itin.IsAtEnd() )
      {
      itgain.Set( static_cast<float>(lut[itin.Get()]) / static_cast<float>( itin.Get() ) ) ;
      ++itin;
      ++itgain;
      }*/
    float pixelValue = 0.0;
    ImageType::IndexType index;
    for (int i = 0 ; i < wThumbnail * nW ; i++)
      {
      for (int j = 0 ; j< hThumbnail * nH ; j++)
        {
        index[0] = i;
        index[1] = j;
        pixelValue = static_cast<float>( lutGrid[ (i / wThumbnail) * nW + j / hThumbnail ] [ inputImage->GetPixel( index ) ] )\
                                      / static_cast<float>( inputImage->GetPixel( index ) ) ;
        gainImage->SetPixel( index , pixelValue );
        }
      }


    gainMultiplyer->SetInput1( inputImage );
    gainMultiplyer->SetInput2( gainImage );
    gainMultiplyer->Update();
    imageGainToVectorImageGainFilterOut->SetInput( chanel , gainImage );
    imageToVectorImageFilterOut->SetInput( chanel , gainMultiplyer->GetOutput() );
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
