#include <iostream>
#include <math.h>
#include <vector>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "otbComputeHistoFilter.h"
#include "otbComputeLutFilter.h"
#include "otbComputeGainFilter.h"

int main ( int itkNotUsed(argc), 
       char const *argv[] )
{
  typedef float PixelType;
  typedef otb::Image < PixelType , 2 > ImageType;
  typedef otb::VectorImage < int , 2 > VectorImageType;
  typedef otb::VectorImage < float , 2 > VectorFloatImageType;
  typedef otb::ImageFileReader < ImageType > ReaderType;
  typedef otb::ImageFileWriter < ImageType > WriterType;
  typedef otb::ComputeHistoFilter < ImageType , VectorImageType > FilterHistoType;
  typedef otb::ComputeLutFilter < VectorImageType , VectorFloatImageType > FilterLutType;
  typedef otb::ComputeGainFilter < ImageType , VectorFloatImageType , ImageType > FilterGainType;


  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  WriterType::Pointer writer( WriterType::New() );
  writer->SetFileName( argv[ 2 ] );
  reader->Update();
  ImageType::Pointer input = reader->GetOutput();
  FilterHistoType::Pointer filterHisto ( FilterHistoType::New() );
  filterHisto->SetInput( reader->GetOutput() );
  ImageType::SizeType thumbSize;
  thumbSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  // thumbSize[0] /= 15;
  // thumbSize[1] /= 15;
  filterHisto->SetThumbSize( thumbSize );
  // filterHisto->SetNoData( 0 );
  filterHisto->SetMin( 0 );
  filterHisto->SetMax( 255 );
  filterHisto->SetThreshold( 3.0 );
  filterHisto->Update();
  FilterLutType::Pointer filterLut ( FilterLutType::New() );
  filterLut->SetMin( 0 );
  filterLut->SetMax( 255 );
  filterLut->SetInput(filterHisto->GetOutput());
  filterLut->Update();
  VectorFloatImageType::Pointer lut = filterLut->GetOutput();
  FilterGainType::Pointer filterGain (FilterGainType::New());
  filterGain->SetInputImage(input);
  filterGain->SetInputLut(lut);
  filterGain->SetMin( 0 );
  filterGain->SetMax( 255 );
  filterGain->Update();
  std::cout<<"yoyo"<<std::endl;
  writer->SetInput( filterGain->GetOutput() );
  writer->Update();
  return 0;

}