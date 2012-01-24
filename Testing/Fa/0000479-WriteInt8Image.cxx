/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"


int main(int argc, char* argv[])
{
  typedef char PixelType;
  
  const char * out = argv[1];
  const PixelType value = static_cast<PixelType>(atof(argv[2] ));

  typedef otb::Image<PixelType, 2>              ImageType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef itk::ImageRegionIterator<ImageType>   IterType;

  ImageType::IndexType id;
  ImageType::SizeType size;
  ImageType::RegionType region;

  id.Fill(0);
  size.Fill(10);

  region.SetSize( size );
  region.SetIndex( id );

  ImageType::Pointer image = ImageType::New();
  ImageType::PixelType pix;
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(value);
 
  IterType it1( image, region );
  it1.GoToBegin();
 
  it1.GoToBegin();
  while( !it1.IsAtEnd() )
    {
    if( value != it1.Get() )
      {
      std::cout<<"Allocated image pixel value "<<it1.Get()<<" differs from the asked one "<<(double)value<<"."<<std::endl;
      return EXIT_FAILURE;
      }
    ++it1;
    }
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image);
  writer->SetFileName(out);
  writer->Update();


  typedef otb::Image<double, 2>                     ReaderImageType;
  typedef otb::ImageFileReader<ReaderImageType>     ReaderType;
  typedef itk::ImageRegionIterator<ReaderImageType> ReaderIterType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(out);
  reader->Update();

  ReaderIterType it2( reader->GetOutput(), region );
  it2.GoToBegin();
  while( !it2.IsAtEnd() )
    {
    if( value != it2.Get() )
      {
      std::cout<<"Read image pixel value "<<it2.Get()<<" differs from the asked one "<<(double)value<<"."<<std::endl;
      return EXIT_FAILURE;
      }
    ++it2;
    }


  return EXIT_SUCCESS;
}
