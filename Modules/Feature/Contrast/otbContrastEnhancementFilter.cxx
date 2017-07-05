#include <iostream>
#include <math.h>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkAdditiveGaussianNoiseImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkMultiplyImageFilter.h"
#include "itkDivideImageFilter.h"
#include "itkAbsImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkSquareImageFilter.h"
#include "itkSqrtImageFilter.h"
#include "itkMaximumImageFilter.h"

typedef float                     PixelType;
typedef itk::Image< PixelType , 2>  ImageType;
typedef itk::AddImageFilter<ImageType, ImageType, ImageType>  AddFilterType;
typedef  otb::StreamingStatisticsImageFilter<ImageType>      StatFilter;

ImageType::Pointer 
createImagefrom( ImageType::Pointer input )
{
  ImageType::Pointer cloneImage( ImageType::New() );
  cloneImage->SetRegions( input->GetRequestedRegion() ); 
  cloneImage->Allocate();
  cloneImage->SetOrigin( input->GetOrigin() );
  return cloneImage ;

}
void
copyAtoB( const ImageType::Pointer input ,
          ImageType::Pointer output )
{
  itk::ImageRegionConstIterator< ImageType > itinp( input , input->GetRequestedRegion() );
  itk::ImageRegionIterator< ImageType >      itout( output , output->GetRequestedRegion() );
  itinp.GoToBegin();
  itout.GoToBegin();
  while (!itinp.IsAtEnd())
  {
    itout.Set( itinp.Get() );
    ++itout;
    ++itinp;
  }
}

void
normL( ImageType::Pointer inputX ,
       ImageType::Pointer inputY ,
       ImageType::Pointer output )
{
  typedef itk::SquareImageFilter< ImageType , ImageType > SquareFilterType;
  typedef itk::SqrtImageFilter< ImageType , ImageType > SqrtFitlerType;
  SquareFilterType::Pointer square( SquareFilterType::New() );
  SqrtFitlerType::Pointer sqrt( SqrtFitlerType::New() );
  AddFilterType::Pointer add( AddFilterType::New() );
  square->SetInput( inputX );
  square->Update();
  copyAtoB( square->GetOutput() , output );
  add->SetInput1( output );
  square->SetInput( inputY );
  square->Update();
  add->SetInput2( square->GetOutput() );
  sqrt->SetInput( add->GetOutput() );
  sqrt->Update();
  copyAtoB( sqrt->GetOutput() , output );
}

int
main( int argc, 
      char const *argv[] )
{
	const char * inputFilename  = argv[ 1 ];
  const char * outputFilename = argv[ 2 ];


  typedef otb::ImageFileReader< ImageType > ReaderType;

 	ReaderType::Pointer reader( ReaderType::New() );
 	reader->SetFileName( argv[ 1 ] );

  ImageType::Pointer noisyImage( ImageType::New() );

  typedef otb::ImageFileWriter<ImageType>  WriterType;
 	WriterType::Pointer writer( WriterType::New());
  return 0;
}