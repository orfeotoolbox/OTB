#include <iostream>
#include <math.h>
#include <vector>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageToHistogramFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkMultiplyImageFilter.h"


typedef int                   PixelType;
typedef itk::Image< PixelType , 2 >  ImageType;
typedef itk::VectorImage< PixelType , 2 >  VectorImageType;

void
computeLuminance( ImageType::Pointer luminance ,
                  VectorImageType::Pointer input )
{
  itk::ImageRegionIterator< VectorImageType >  itinp( input , input->GetRequestedRegion() );
  itk::ImageRegionIterator< ImageType >  itlum( luminance , luminance->GetRequestedRegion() );
  itinp.GoToBegin();
  itlum.GoToBegin();
  while ( !itinp.IsAtEnd() )
  {
    // 0.3R 0.6G 0.1B
    itlum.Set( round(0.3 * itinp.Get()[0] + 0.6 * itinp.Get()[1] + 0.1  * itinp.Get()[2]) );
    ++itinp;
    ++itlum;
  }
}

int
main( int argc, 
      char const *argv[] )
{
  // const char * inputfilename  = argv[ 1 ];
  // const char * outputfilename = argv[ 2 ];


  typedef otb::ImageFileReader< VectorImageType > ReaderType;
  typedef itk::Statistics::ImageToHistogramFilter< ImageType > HistogramFilterType;
  typedef HistogramFilterType::HistogramType HistogramType;
  typedef itk::VectorIndexSelectionCastImageFilter< VectorImageType , ImageType > VectorToImageFilterType;


  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  reader->Update();
  VectorImageType::Pointer input = reader->GetOutput();
  ImageType::Pointer lum ( ImageType::New() ); 
  lum->SetRegions( input->GetRequestedRegion() );
  lum->Allocate();
  lum->SetOrigin( input->GetOrigin() );
  VectorToImageFilterType::Pointer vectorToImageFilter ( VectorToImageFilterType::New() );
  if ( input->GetVectorLength() == 3 )
  {
    computeLuminance(lum, input);
  }
  else
  {
    // default is a black and white image, if length>3 and is not equal to one
    // other case :  how to apply the equalization :
    // - channel per channel?
    // - with a general luminance?
    vectorToImageFilter->SetInput( input );
    vectorToImageFilter->SetIndex( 0 );
    vectorToImageFilter->Update();
    lum = vectorToImageFilter->GetOutput();
  }


  HistogramFilterType::Pointer histofilter( HistogramFilterType::New() );
  histofilter->SetInput( lum );
  histofilter->Update();
  // Compute the histogram of the bw image
  HistogramType * histo = histofilter->GetOutput();

  // Define propertise of the target histogram : nomber of bin, height per bin
  ImageType::SizeType lumSize = lum->GetLargestPossibleRegion().GetSize(); 
  int npx = lumSize[0] * lumSize[1];
  int nbin = histo->GetSize()[0];
  float height = npx / nbin;

  // The transfer function will be a look up table
  std::vector< int > lut;
  float nbOri, nbNew ; 
  int countnew, countold;
  countnew = 0;
  countold = 0;
  nbNew = height;
  HistogramType::Iterator it = histo->Begin();
  nbOri = it.GetFrequency();
  while ( it != histo->End() )
  {
    if (nbOri> nbNew)
    {
      nbNew += height;
      // index[0] += 1; 
      ++countnew;
    }
    else
    {
      ++it;
      nbOri += it.GetFrequency();
      ++countold;
      lut.push_back(countnew);
    }
  }

  // Here we compute a gain image corresponding to the associated gain of the equalization
  typedef itk::Image< float , 2 > ImageGainType;
  ImageGainType::Pointer gainImage ( ImageGainType::New() );
  gainImage->SetRegions( lum->GetRequestedRegion() );
  gainImage->Allocate();
  gainImage->SetOrigin( lum->GetOrigin() );
  itk::ImageRegionIterator< ImageType > itlum( lum , lum->GetRequestedRegion() );
  itk::ImageRegionIterator< ImageGainType > itgain( gainImage , gainImage->GetRequestedRegion() );
  itlum.GoToBegin();
  itgain.GoToBegin();
  while( !itlum.IsAtEnd() )
  {
    itgain.Set( static_cast<float>(lut[itlum.Get()]) / static_cast<float>( itlum.Get() ) ) ;
    ++itlum;
    ++itgain;
  }

  typedef  itk::MultiplyImageFilter< VectorImageType, ImageGainType, VectorImageType > MultiplyImageFilterType;
  MultiplyImageFilterType::Pointer gainMultiplyer ( MultiplyImageFilterType::New() );
  gainMultiplyer->SetInput1( input );
  gainMultiplyer->SetInput2( gainImage );
  gainMultiplyer->Update();
  typedef otb::ImageFileWriter<VectorImageType>  writertype;
  writertype::Pointer writer( writertype::New());
  writer->SetFileName( argv[2] );
  writer->SetInput( gainMultiplyer->GetOutput() );
  writer->Update();
  return 0;
}
