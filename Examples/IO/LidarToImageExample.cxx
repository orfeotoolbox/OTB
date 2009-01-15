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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {TO_core_last_zoom.las}
//    OUTPUTS: {lidar-image-4.hdr}, {lidar-image-4-pretty.png}
//    1.0 5 4
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {TO_core_last_zoom.las}
//    OUTPUTS: {lidar-image-6.hdr}, {lidar-image-6-pretty.png}
//    1.0 5 6
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// This example describes how to convert a point set obtained from lidar data
// to an image file. A lidar produces a point set which is irregular in terms of spatial
// sampling. To be able to generate an image, an interpolation is required.
//
// The interpolation is done using the
// \doxygen{itk}{BSplineScatteredDataPointSetToImageFilter} which uses BSplines. The
// method is fully described in \cite{Tustison2005} and \cite{Lee1997}.
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet

#include "itkPointSet.h"
#include "otbImage.h"
#include "otbPointSetFileReader.h"
#include "itkBSplineScatteredDataPointSetToImageFilter.h"
#include "otbImageFileWriter.h"

// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Here a note is important. The \doxygen{itk}{BSplineScatteredDataPointSetToImageFilter}
// is on the review directory of ITK, which means that it won't be compiled by default.
// If you want to use it, you have to set:
// ITK\_USE\_REVIEW to ON
// in the ccmake of OTB.
//
// Software Guide : EndLatex

#include "itkRescaleIntensityImageFilter.h"

int main( int argc, char* argv[] )
{


  if(argc!=7)
  {
    std::cout << argv[0] <<" <input_lidar_filename> <output_image_filename(double)>"
              << " <output_image_filename(unsigned char)>"
	      << " <output_resolution> <spline_order>"
              << " <number_of_levels>"
	      << std::endl;
    return EXIT_FAILURE;
  }

  // Software Guide : BeginLatex
  //
  // Then, we declare the type of the data that we are going to use. As
  // lidar decribes the altitude of the point (often to centimeter accuracy),
  // it is required to use a real type to represent it.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double RealType;
  typedef itk::Vector<RealType, 1> VectorType;
  typedef itk::PointSet <VectorType, 2> PointSetType;
  typedef otb::Image<RealType, 2> ImageType;
  typedef itk::Image<VectorType, 2> VectorImageType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Lidar data are read into a point set using the
  // \doxygen{otb}{PointSetFileReader}. Its usage is very similar to
  // the \doxygen{otb}{ImageFileReader}:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::PointSetFileReader<PointSetType> PointSetFileReaderType;
  PointSetFileReaderType::Pointer reader = PointSetFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now prepare the parameters to pass to the interpolation filter:
  // you have to be aware that the origin of the image is on the upper left
  // corner and thus corresponds to the minimun easting but the maximum northing.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double resolution = atof(argv[4]);
  int splineOrder = atoi(argv[5]);
  int level = atoi(argv[6]);
  // Software Guide : EndCodeSnippet

  std::cout << "*** Data area *** " << std::endl;
  std::cout << std::setprecision(15);
  std::cout << " - Easting min:  " << reader->GetMinX() << std::endl;
  std::cout << " - Easting max:  " << reader->GetMaxX() << std::endl;
  std::cout << " - Northing min: " << reader->GetMinY() << std::endl;
  std::cout << " - Northing max: " << reader->GetMaxY() << std::endl;
  std::cout << "Data points: " << reader->GetNumberOfPoints() << std::endl;

  // Software Guide : BeginCodeSnippet
  ImageType::IndexType start;
  start[0] =  0;
  start[1] =  0;

  ImageType::SizeType  size;
  size[0]  = static_cast<long int >(
         ceil((reader->GetMaxX()-reader->GetMinX()+1)/resolution))+2;
  size[1]  = static_cast<long int >(
         ceil((reader->GetMaxY()-reader->GetMinY()+1)/resolution))+2;

  ImageType::PointType origin;
  origin[0] = reader->GetMinX();
  origin[1] = reader->GetMaxY();

  ImageType::SpacingType spacing;
  spacing[0] = resolution;
  spacing[1] = -resolution;
  // Software Guide : EndCodeSnippet

//   std::cout << "Size: " << size << std::endl;
//   std::cout << "Origin: " << origin << std::endl;
//   std::cout << "Spacing: " << spacing << std::endl;

  // Software Guide : BeginLatex
  //
  // All these parameters are passed to the interpolation filter:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
   typedef itk::BSplineScatteredDataPointSetToImageFilter
      <PointSetType, VectorImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  filter->SetSplineOrder( splineOrder );
  FilterType::ArrayType ncps;
  ncps.Fill( 6 );
  filter->SetNumberOfControlPoints( ncps );
  filter->SetNumberOfLevels( level );

  filter->SetOrigin( origin );
  filter->SetSpacing( spacing );
  filter->SetSize( size );

  filter->SetInput( reader->GetOutput() );

  filter->Update();
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // The result of this filter is an image in which every pixel
  // is a vector (with only one element). For now, the otb writer
  // does not know how to process that (hopefully soon!). So we
  // have to manually copy the element in a standard image:
  //
  // Software Guide : EndLatex

  //TODO ImageOfVectorsToVectorImage needed !
    // Write the output to an image.

   // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Everything is ready so we can just write the image:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( image );
  writer->SetFileName( argv[2] );
  writer->Update();
  // Software Guide : EndCodeSnippet

  typedef otb::Image<unsigned char, 2> UCharImageType;
  typedef itk::RescaleIntensityImageFilter<ImageType,
               UCharImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput(image);
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);

  typedef otb::ImageFileWriter<UCharImageType> WriterUCharType;
  WriterUCharType::Pointer writerUChar = WriterUCharType::New();
  writerUChar->SetInput(rescaler->GetOutput());
  writerUChar->SetFileName(argv[3]);
  writerUChar->Update();





    // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:LIDARTOIMAGEEXAMPLE} shows the output images with two sets of parameters
  //
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{lidar-image-4-pretty.eps}
  // \includegraphics[width=0.40\textwidth]{lidar-image-6-pretty.eps}
  // \itkcaption[Image from lidar data]{Image obtained with 4 level spline interpolation (left) and 6 levels (right)}
  // \label{fig:LIDARTOIMAGEEXAMPLE}
  // \end{figure}
  //
  // Software Guide : EndLatex







  return EXIT_SUCCESS;

}

