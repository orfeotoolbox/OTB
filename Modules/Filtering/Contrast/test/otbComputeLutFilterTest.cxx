#include <iostream>
#include <math.h>
#include <vector>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "otbComputeHistoFilter.h"

int main ( int argc, 
       char const *argv[] )
{
  typedef int                  PixelType;
  typedef otb::Image< PixelType , 2 >  ImageType;
  typedef otb::VectorImage< PixelType , 2 >  VectorImageType;
  typedef otb::ImageFileReader< ImageType > ReaderType;
  typedef otb::ImageFileWriter< VectorImageType > WriterType;
  typedef otb::ComputeHistoFilter< ImageType , VectorImageType > FilterType;


  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( argv[ 1 ] );
  reader->Update();
  FilterType::Pointer filter ( FilterType::New() );
  filter->SetInput( reader->GetOutput() );
  ImageType::SizeType thumbSize;
  thumbSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  filter->SetThumbSize(thumbSize);
  filter->SetMin(0);
  filter->SetMax(0);
  filter->Update();
  WriterType::Pointer writer( WriterType::New() );
  writer->SetFileName( argv[ 2 ] );
  writer->SetInput(filter->GetOutput());
  writer->Update();
  return 0;

}