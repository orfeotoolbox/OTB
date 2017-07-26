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
#include "itkThresholdImageFilter.h"
#include "otbContrastEnhancementFilter.h"

int const sizeh = 256;
typedef int                  PixelType;
typedef itk::Image< PixelType , 2 >  ImageType;
typedef itk::VectorImage< PixelType , 2 >  VectorImageType;
typedef itk::Image< float , 2 > ImageGainType; 


void
equalized( const std::array< int , sizeh > & inputHisto,
           const std::array< int , sizeh > & targetHisto,
           std::array< int , sizeh > & lut)

{
  int countMapValue = 0;
  int countValue = 0;
  lut[countValue] = 1; // Black stay black
  ++countValue;
  int countInput  = inputHisto[ 0 ] + inputHisto[ countValue ];
  lut[lut.size() - 1 ] = 1 ; // White stay white
  int countTarget = targetHisto[ countMapValue ];
  while ( countMapValue<256 && countValue<255)
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
equalized( std::array< int , sizeh > gridHisto[],
           std::array< int , sizeh > gridLut[],
           const std::array< int , sizeh > & targetHisto,
           int nW,
           int nH)
{
  for (int i = 0 ; i<nW ; i++)
    {
    for ( int j = 0 ; j<nH ; j++ )
      {
      equalized( gridHisto[i + j * nW ] , targetHisto , gridLut[i+ j * nW ]);
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
  // int nbBin = inputHisto[0].size();
  for (int i = 0 ; i < wThumbnail * nW ; i++)
    {
    for (int j = 0 ; j< hThumbnail * nH ; j++)
      {
      index[0] = i;
      index[1] = j;
      ++inputHisto[ (i / wThumbnail)  + ( j / hThumbnail ) * nW][ input->GetPixel( index ) ];
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
              int l)
{
  int nbPixel = h * l;
  int nbBin = targetHisto.size();
  int height = nbPixel/nbBin;
  targetHisto.fill( height );
}

float
interpoleGain( std::array< int , sizeh > lut[],
               int nW,
               int nH,
               int pixelValue,
               ImageType::IndexType index,
               int wThumbnail,
               int hThumbnail)
{
  int lutX = index[0]/wThumbnail;
  int lutY = index[1]/hThumbnail;
  float x = static_cast< float >(index[0]%wThumbnail) / static_cast< float >(wThumbnail);
  float y = static_cast< float >(index[1]%hThumbnail) / static_cast< float >(hThumbnail);
  float w = (1 - std::abs(x - 0.5) )*(1 - std::abs(y - 0.5) );
  float gain = lut[lutX + lutY * nW ][ pixelValue ] * (1 - std::abs(x - 0.5) )*(1 - std::abs(y - 0.5) ) ;
  bool right = x>=0.5 && lutX<(nW - 1) ;
  bool up = y<=0.5 && lutY>0;
  bool left = x<=0.5 && lutX>0;
  bool down = y>=0.5 && lutY<(nH - 1);
  if ( right )
    {
    gain += lut[lutX + 1 + lutY * nW ][ pixelValue ] * (1 - std::abs(y - 0.5) ) * (x - 0.5);
    w += (1 - std::abs(y - 0.5) ) * (x - 0.5);
    }
  if ( left )
    {
    gain += lut[lutX + lutY * nW - 1 ][ pixelValue ] * (1 - std::abs(y - 0.5) ) * std::abs(x - 0.5);
    w += (1 - std::abs(y - 0.5) ) * std::abs(x - 0.5);
    }
  if ( up )
    {
    gain += lut[lutX + (lutY - 1) * nW ][ pixelValue ] * std::abs(y - 0.5) * (1 - std::abs(x - 0.5) );
    w += std::abs(y - 0.5) * (1 - std::abs(x - 0.5) );
    }
  if ( down )
    {
    gain += lut[lutX + (lutY + 1)* nW ][ pixelValue ]  * (y - 0.5) * (1 - std::abs(x - 0.5) );
    w += (y - 0.5) * (1 - std::abs(x - 0.5) );
    }
  if ( up && left )
    {
    gain += lut[( lutX - 1 ) + (lutY - 1 ) * nW ][ pixelValue ] * std::abs(y - 0.5) * std::abs(x - 0.5);
    w += std::abs(y - 0.5) * std::abs(x - 0.5);
    }
  if ( down && left )
    {
    gain += lut[( lutX - 1 ) + (lutY + 1) * nW ][ pixelValue ] * std::abs(y - 0.5) * std::abs(x - 0.5);
    w += std::abs(y - 0.5) * std::abs(x - 0.5);
    }
  if ( up && right )
    {
    gain += lut[( lutX + 1 ) + (lutY - 1) * nW ][ pixelValue ] * std::abs(y - 0.5) * std::abs(x - 0.5);
    w += std::abs(y - 0.5) * std::abs(x - 0.5) ;
    }
  if ( down && right )
    {
    gain += lut[( lutX + 1 ) + (lutY + 1 ) * nW ][ pixelValue ] * std::abs(y - 0.5) * std::abs(x - 0.5);
    w += std::abs(y - 0.5) * std::abs(x - 0.5);
    }

  return gain/(w * pixelValue);
}

void
histoLimiteContrast(std::array< int , sizeh > inputHisto[],
                    int h,
                    int nW,
                    int nH)
{
  int histoLength = inputHisto[0].size();
  int nbHisto = nW * nH;
  int toThresh = 0;
  int npx = 0;
  int rest = 0;
  for ( int i = 0 ; i < nbHisto ; i++)
    {
    toThresh = 0;
    for (int j = 0 ; j < histoLength ; j++)
      {
      if ( inputHisto[i][j] > h )
        {
          toThresh+= inputHisto[i][j] - h;
          inputHisto[i][j] = h ;
        }
      }
    if (toThresh != 0 )
      {
      npx = toThresh / histoLength;
      rest = toThresh%histoLength;
      for (int j = 0 ; j < histoLength ; j++)
       {
        inputHisto[i][j] += npx ;
        if (rest>0)
          {
          ++inputHisto[i][j];
          --rest;
          }
        }
      }
    }
}

ImageGainType::Pointer
gainLimiteContrast(const ImageGainType::Pointer & gainImage,
                   float upThresh,
                   float lowThresh)
{
  typedef itk::ThresholdImageFilter< ImageGainType > ThresholdFilterType;
  ThresholdFilterType::Pointer thresholdFilter ( ThresholdFilterType::New() );
  thresholdFilter->SetInput( gainImage );
  thresholdFilter->SetOutsideValue (upThresh);
  thresholdFilter->ThresholdAbove( upThresh );
  thresholdFilter->Update();
  thresholdFilter->SetInput( thresholdFilter->GetOutput() );
  thresholdFilter->SetOutsideValue (lowThresh);
  thresholdFilter->ThresholdBelow( lowThresh );
  thresholdFilter->Update();
  return thresholdFilter->GetOutput();
}

int
prototype ( int argc, 
      char const *argv[] )
{
  // const char * inputfilename  = argv[ 1 ];
  // const char * outputfilename = argv[ 2 ];
  if (argc<5)
  {
    std::cout<<"error : otbContrastTestDriver -inputFilename -outputFilename -hThumbnail -wThumbnail"<<std::endl;
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
  int hThumbnail = atoi(argv[3]);
  int wThumbnail = atoi(argv[4]);
  ImageType::Pointer inputImage = vectorToImageFilter->GetOutput();
  if ( inputImage->GetLargestPossibleRegion().GetSize()[1]%hThumbnail != 0 )
    {
    std::cout<<"error : hThumbnail = "<<hThumbnail<<" is not a divider of the input's width"<<std::endl;
    std::cout<<"Image Height = "<<input->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
    return 1;
    }
  if ( inputImage->GetLargestPossibleRegion().GetSize()[0]%wThumbnail != 0 )
    {
    std::cout<<"error : wThumbnail = "<<wThumbnail<<"is not a divider of the input's height"<<std::endl;
    std::cout<<"Image Width = "<<input->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
    return 1;
    }

  int nH = inputImage->GetLargestPossibleRegion().GetSize()[0]/wThumbnail;
  std::cout<<"nH ="<<nH<<std::endl;
  int nW = inputImage->GetLargestPossibleRegion().GetSize()[1]/hThumbnail;
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
    std::array< int , sizeh >  histoGrid [(nH)*(nW)] = {};

    // Initialize lutGrid and histoGrid to zero
    for (int i = 0 ; i<(nH)*(nW) ; i++)
    {
      lutGrid[i].fill(0);
      histoGrid[i].fill(0);
    }

    computehisto( inputImage , histoGrid , wThumbnail , hThumbnail , nW , nH);

    // Compute the threshold limit for the clipping described in the original 
    // CLAHE algorithm
    float factor = 3.0;
    int threshH = factor * (wThumbnail * hThumbnail ) / sizeh;
    // histoLimiteContrast( histoGrid , threshH , nW , nH);

    equalized( histoGrid , lutGrid , histoTarget , nW , nH);
    
    float gainValue = 0.0;
    ImageType::IndexType index;
    for (int i = 0 ; i < wThumbnail * nW ; i++)
      {
      for (int j = 0 ; j< hThumbnail * nH ; j++)
        {
        index[0] = i;
        index[1] = j;
        gainValue = interpoleGain(lutGrid , nW , nH , inputImage->GetPixel( index ) , index , wThumbnail , hThumbnail);
        gainImage->SetPixel( index , gainValue );
        }
      }

    float upThresh = 1.5;
    float lowThresh = 0.8;
    // Limit contrast through limited gain
    gainImage = gainLimiteContrast( gainImage , upThresh , lowThresh );
    
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

  return 0;
}

int
main ( int argc, 
       char const *argv[] )
{
  /*typedef otb::ImageFileReader< ImageType > ReaderType;
  typedef otb::ContrastEnhancementFilter< ImageType , ImageType > FilterType;

  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  reader->Update();

  FilterType::Pointer filter( FilterType::New() );
  filter->SetInput(reader->GetOutput());
  filter->setHistoThreshFactor(3);
  filter->setHistoSize(256);
  int sThumbnail = atoi(argv[3]);
  filter->setThumbnailSize( sThumbnail, sThumbnail );
  filter->Update();

  typedef otb::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer( WriterType::New() );
  writer->SetFileName(argv[2]);
  writer->SetInput(filter->GetOutput());
  writer->Update();
  return 0;*/

  typedef otb::ImageFileReader< VectorImageType > ReaderType;
  typedef itk::VectorIndexSelectionCastImageFilter< VectorImageType , ImageType > VectorToImageFilterType;
  typedef itk::ComposeImageFilter< ImageType > ImageToVectorImageFilterType;

  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  reader->Update();
  VectorImageType::Pointer input = reader->GetOutput();

  ImageToVectorImageFilterType::Pointer imageToVectorImageFilterOut( ImageToVectorImageFilterType::New() );
  VectorToImageFilterType::Pointer vectorToImageFilter ( VectorToImageFilterType::New() );
  vectorToImageFilter->SetInput( input );
  vectorToImageFilter->SetIndex( 0 );
  vectorToImageFilter->Update();
  int hThumbnail = atoi(argv[3]);
  int wThumbnail = atoi(argv[4]);
  ImageType::Pointer inputImage = vectorToImageFilter->GetOutput();
  if ( inputImage->GetLargestPossibleRegion().GetSize()[1]%hThumbnail != 0 )
    {
    std::cout<<"error : hThumbnail = "<<hThumbnail<<" is not a divider of the input's height"<<std::endl;
    std::cout<<"Image Height = "<<input->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
    return 1;
    }
  if ( inputImage->GetLargestPossibleRegion().GetSize()[0]%wThumbnail != 0 )
    {
    std::cout<<"error : wThumbnail = "<<wThumbnail<<"is not a divider of the input's width"<<std::endl;
    std::cout<<"Image Width = "<<input->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
    return 1;
    }

  typedef otb::ContrastEnhancementFilter< ImageType , ImageType > FilterType;
  int m = input->GetVectorLength ();

  for (int chanel = 0 ; chanel<m ; chanel++ ) 
    {
    vectorToImageFilter->SetIndex( chanel );
    vectorToImageFilter->Update();
    FilterType::Pointer filter( FilterType::New() );
    filter->SetInput(vectorToImageFilter->GetOutput());
    filter->setHistoThreshFactor(3);
    filter->setHistoSize(256);
    filter->setThumbnailSize( hThumbnail, wThumbnail );
    filter->Update();
    imageToVectorImageFilterOut->SetInput( chanel , filter->GetOutput() );
    }

  // Here we compute a gain image corresponding to the associated gain of the equalization
  imageToVectorImageFilterOut->GetOutput()->SetOrigin(input->GetOrigin());
  typedef otb::ImageFileWriter<VectorImageType>  WriterType;
  WriterType::Pointer writer( WriterType::New());
  writer->SetFileName( argv[2] );
  writer->SetInput( imageToVectorImageFilterOut->GetOutput() );
  writer->Update();
}
