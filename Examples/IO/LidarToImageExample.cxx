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

#include "itkPointSet.h"
#include "otbImage.h"
#include "otbPointSetFileReader.h"
#include "itkBSplineScatteredDataPointSetToImageFilter.h"
#include "otbImageFileWriter.h"
    
// 1   5 12 
    
int main( int argc, char* argv[] )
{


  if(argc!=6)
  {
    std::cout << argv[0] <<" <input_lidar_filename> <output_image_filename>"
	      << " <output_resolution> <spline_order>"
              << " <number_of_level>" 
	      << std::endl;
    return EXIT_FAILURE;
  }
  
  typedef double RealType;
  typedef itk::Vector<RealType, 1> VectorType;
  typedef itk::PointSet <VectorType, 2> PointSetType;
  typedef otb::PointSetFileReader<PointSetType> PointSetFileReaderType;
  PointSetFileReaderType::Pointer reader = PointSetFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->Update();
  
  
  typedef double PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
    
  
  typedef itk::Image<VectorType, 2> VectorImageType;
  
  double resolution = atof(argv[3]);
  int splineOrder = atoi(argv[4]);
  int level = atoi(argv[5]);
  
  ImageType::IndexType start;
  start[0] =  0;
  start[1] =  0; 
  
//   std::cout << std::setprecision(15);
//   std::cout << reader->GetMinX() << std::endl;
//   std::cout << reader->GetMaxX() << std::endl;
//   std::cout << reader->GetMinY() << std::endl;
//   std::cout << reader->GetMaxY() << std::endl;
  
  ImageType::SizeType  size;
  size[0]  = static_cast<long int >(
         ceil((reader->GetMaxX()-reader->GetMinX()+1)/resolution))+1; 
  size[1]  = static_cast<long int >(
         ceil((reader->GetMaxY()-reader->GetMinY()+1)/resolution))+1; 

  ImageType::PointType origin;
  origin[0] = reader->GetMinX();//minX;
  origin[1] = reader->GetMaxY();//maxY;
  
  ImageType::SpacingType spacing;
  spacing[0] = resolution;
  spacing[1] = -resolution;

  
//   std::cout << "Size: " << size << std::endl;
//   std::cout << "Origin: " << origin << std::endl;
//   std::cout << "Spacing: " << spacing << std::endl;
  
   typedef itk::BSplineScatteredDataPointSetToImageFilter
      <PointSetType, VectorImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  
  filter->SetSplineOrder( splineOrder );  
  FilterType::ArrayType ncps;  
  ncps.Fill( 6 );  
  filter->SetNumberOfControlPoints( ncps );
  filter->SetNumberOfLevels( level );

  // Define the parametric domain.
  filter->SetOrigin( origin );
  filter->SetSpacing( spacing );
  filter->SetSize( size );

  filter->SetInput( reader->GetOutput() );

  filter->Update();

  
    // Write the output to an image.
  typedef otb::Image<RealType, 2> RealImageType;
  RealImageType::Pointer image = RealImageType::New();
  ImageType::RegionType region;
  region.SetSize( size );
  region.SetIndex( start );
  image->SetRegions( region );
  image->Allocate();
  itk::ImageRegionIteratorWithIndex<RealImageType> 
      Itt( image, image->GetLargestPossibleRegion() );
  
  for ( Itt.GoToBegin(); !Itt.IsAtEnd(); ++Itt )
  {
    Itt.Set( filter->GetOutput()->GetPixel( Itt.GetIndex() )[0] );
  }

  
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( image );
  writer->SetFileName( argv[2] );
  writer->Update();


  return EXIT_SUCCESS;

}

