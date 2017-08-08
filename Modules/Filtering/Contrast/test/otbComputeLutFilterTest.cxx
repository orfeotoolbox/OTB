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

int main ( int argc, 
       char const *argv[] )
{
  typedef unsigned char PixelType;
  typedef otb::Image < PixelType , 2 > ImageType;
  typedef otb::VectorImage < PixelType , 2 > VectorImageType;
  typedef otb::ImageFileReader < ImageType > ReaderType;
  typedef otb::ImageFileWriter < ImageType > WriterType;
  typedef otb::ComputeHistoFilter < ImageType , VectorImageType > FilterHistoType;
  typedef otb::ComputeLutFilter < VectorImageType , VectorImageType > FilterLutType;


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
  filterHisto->SetThumbSize( thumbSize );
  filterHisto->SetMin( 0 );
  filterHisto->SetMax( 255 );
  filterHisto->Update();
  FilterLutType::Pointer filterLut ( FilterLutType::New() );
  filterLut->SetInput(filterHisto->GetOutput());
  filterLut->Update();
  VectorImageType::Pointer lut = filterLut->GetOutput();
  itk::ImageRegionIterator < ImageType > it (input ,
            input->GetLargestPossibleRegion() );
  it.GoToBegin();
  VectorImageType::IndexType index;
  index.Fill(0);
  while ( !it.IsAtEnd() )
  {
    it.Set(lut->GetPixel(index)[it.Get()]);
    ++it;
  }
  writer->SetInput( input );
  writer->Update();
  return 0;

}