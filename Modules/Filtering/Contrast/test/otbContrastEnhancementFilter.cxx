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
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"

#if 0
int const sizeh = 256;
typedef int                  PixelType;
typedef otb::Image< PixelType , 2 >  ImageType;
typedef otb::VectorImage< PixelType , 2 >  VectorImageType;
typedef otb::Image< float , 2 > ImageGainType; 


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
#endif

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
  // typedef int OPixelType;
  // typedef otb::Image<OPixelType, 2 > OImageType;
  // typedef otb::VectorImage< OPixelType , 2 >  OVectorImageType;
  typedef float                  PixelType;
  typedef otb::Image< PixelType , 2 >  ImageType;
  typedef otb::Image< float , 2 >  ImageGainType;
  typedef otb::VectorImage< PixelType , 2 >  VectorImageType;
  typedef otb::ImageFileReader< VectorImageType > ReaderType;
  typedef otb::ImageFileReader< ImageType > ReaderImageType;
  typedef otb::ImageFileWriter< VectorImageType > WriterType;
  typedef otb :: ImageList< ImageType > ImageListType;
  typedef otb::VectorImageToImageListFilter< VectorImageType, ImageListType > 
          VectorToImageListFilterType;
  typedef otb::ImageListToVectorImageFilter< ImageListType, VectorImageType > 
          ImageListToVectorFilterType;
  typedef otb::ContrastEnhancementFilter< ImageGainType , ImageGainType > FilterType;
  ImageListToVectorFilterType::Pointer imageListToVectorFilterOut( ImageListToVectorFilterType::New() );

  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  reader->Update();
  VectorImageType::Pointer input = reader->GetOutput();
  
  typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;
  ImageMetadataInterfaceType::Pointer metadataInterface = otb::ImageMetadataInterfaceFactory::CreateIMI(input->GetMetaDataDictionary());

  std::vector<double> values;
  std::vector<bool> flags;

  bool ret = metadataInterface->GetNoDataFlags(flags,values);

  if(ret && !values.empty() && !flags.empty() && flags[0])
    {
    std::cout<<values[0]<<std::endl;
    }
  std::vector<uint> rgb = metadataInterface->GetDefaultDisplay() ;


  std::cout<<"red is "<<rgb[ 0 ]<<std::endl;
  std::cout<<"green is "<<rgb[ 1 ]<<std::endl;
  std::cout<<"blue is "<<rgb[ 2 ]<<std::endl;


#if 0
  VectorToImageListFilterType::Pointer vectorToImageListFilter ( VectorToImageListFilterType::New() );
  vectorToImageListFilter->SetInput( input );
  vectorToImageListFilter->Update();
  if (argc<5)
  {
    std::cout<<"error : otbContrastTestDriver -inputFilename -outputFilename -hThumbnail -wThumbnail"<<std::endl;
    if (argc>1)
      {
        std::cout<<"Image Width = "<<input->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
        std::cout<<"Image Height = "<<input->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
      }
    return 1;
  }
  int hThumbnail = atoi(argv[3]);
  int wThumbnail = atoi(argv[4]);
  
  ImageListType::Pointer inputImageList = vectorToImageListFilter->GetOutput();

  //////////For several input but one image out/////
  // ImageListType::Pointer inputImageList (ImageListType::New());
  // for (int i = 1 ; i<4 ; i++)
  // {
  // ReaderImageType::Pointer readerImage (ReaderImageType::New());
  // readerImage->SetFileName(argv[i]);
  // readerImage->Update();
  // inputImageList->PushBack( readerImage->GetOutput() );
  // }
  // int hThumbnail = atoi(argv[5]);
  // int wThumbnail = atoi(argv[6]);
  ImageType::Pointer inputImage = inputImageList->Front();

  if ( inputImage->GetLargestPossibleRegion().GetSize()[1]%hThumbnail != 0 )
    {
    std::cout<<"error : hThumbnail = "<<hThumbnail<<" is not a divider of the input's height"<<std::endl;
    std::cout<<"Image Height = "<<inputImage->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
    return 1;
    }
  if ( inputImage->GetLargestPossibleRegion().GetSize()[0]%wThumbnail != 0 )
    {
    std::cout<<"error : wThumbnail = "<<wThumbnail<<"is not a divider of the input's width"<<std::endl;
    std::cout<<"Image Width = "<<inputImage->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
    return 1;
    }


  ImageListType::Pointer outputImageList ( ImageListType::New() );

  itk::ImageRegionIterator < ImageType > 
      rit ( inputImageList->GetNthElement(0) ,
           inputImageList->GetNthElement(0)->GetRequestedRegion() );
  itk::ImageRegionIterator < ImageType > 
      git ( inputImageList->GetNthElement(1) ,
           inputImageList->GetNthElement(1)->GetRequestedRegion() );
  itk::ImageRegionIterator < ImageType > 
      bit ( inputImageList->GetNthElement(2) ,
           inputImageList->GetNthElement(2)->GetRequestedRegion() );

  ImageGainType::Pointer luminance(ImageGainType::New());
  luminance->SetRegions( inputImage->GetRequestedRegion() );
  luminance->Allocate();
  luminance->SetOrigin( inputImage->GetOrigin() );
  luminance->SetSpacing( inputImage->GetSpacing() );

  itk::ImageRegionIterator < ImageGainType > 
      lit ( luminance ,
           luminance->GetRequestedRegion() );
  lit.GoToBegin();
  bit.GoToBegin();
  rit.GoToBegin();
  git.GoToBegin();
  while ( !lit.IsAtEnd())
    {
    lit.Set( 0.21 * rit.Get() + 0.71 * git.Get() + 0.08 * bit.Get() );
    ++lit;
    ++bit;
    ++rit;
    ++git;
    }
  FilterType::Pointer filter( FilterType::New() );
  filter->SetInput( luminance ) ;
  filter->setHistoThreshFactor(4);
  filter->setHistoSize(512);
  filter->setNoData(0);
  // filter->setGainThresh(0.0 , 10.0);
  filter->setThumbnailSize( wThumbnail, hThumbnail );
  filter->Update();
  itk::ImageRegionIterator < ImageGainType > 
      nlit ( filter->GetOutput() , 
            filter->GetOutput()->GetRequestedRegion() );
  lit.GoToBegin();
  nlit.GoToBegin();
  bit.GoToBegin();
  rit.GoToBegin();
  git.GoToBegin();
  float gain = 0.0;
  float denum = 1;
  while ( !lit.IsAtEnd())
    {
    if ( lit.Get() != 0 )
      denum = lit.Get();
    else
      {
      denum = 1;
      }
    gain = nlit.Get()/denum;
    bit.Set(gain * bit.Get());
    rit.Set(gain * rit.Get());
    git.Set(gain * git.Get());
    ++nlit;
    ++lit;
    ++bit;
    ++rit;
    ++git;
    }
/*
  // Working for equalization channel by channel
  int m = input->GetVectorLength ();
  for (int chanel = 0 ; chanel<m ; chanel++ ) 
    {
    FilterType::Pointer filter( FilterType::New() );
    filter->SetInput( inputImageList->GetNthElement(chanel) ) ;
    filter->setHistoThreshFactor(6);
    filter->setHistoSize(256);
    filter->setNoData(0);
    // filter->setGainThresh(0.0 , 10.0);
    filter->setThumbnailSize( wThumbnail, hThumbnail );
    filter->Update();
    outputImageList->PushBack( filter->GetOutput() );
    }
*/ 
  imageListToVectorFilterOut->SetInput(inputImageList);
  imageListToVectorFilterOut->Update();
  VectorImageType::Pointer output = imageListToVectorFilterOut->GetOutput();
  output->SetOrigin(inputImage->GetOrigin());
  // output->CopyInformation(input);
  WriterType::Pointer writer( WriterType::New());
  writer->SetFileName( argv[2] );
  writer->SetInput( output );
  writer->Update();


#if 0
  ImageType::Pointer img ( ImageType::New() );
  ImageType::SizeType size;
  size[1] = 1080;
  size[0] = (int) (size[1]*(16.0/9.0));
  ImageType::RegionType region;
  region.SetSize(size);
  img->SetRegions(region);
  img->Allocate();
  itk::ImageRegionIterator<ImageType> pat(img , img->GetLargestPossibleRegion());
  pat.GoToBegin();
  int val = 0;
  while(!pat.IsAtEnd())
  { 
    val = pat.GetIndex()[0]%2;
    pat.Set(val*255);
    ++pat;
  }


   otb::ImageFileWriter< ImageType >::Pointer writ(otb::ImageFileWriter< ImageType >::New()) ;
   const char * name = "../../my_data/imgline1080.jpeg";
   writ->SetFileName(name);
   writ->SetInput(img);
   writ->Update();

#endif

#endif
return 0;

}
