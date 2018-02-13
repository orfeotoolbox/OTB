/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbClampImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <limits>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkImageRegionConstIterator.h"

/** Pixel typedefs */
typedef double                                      InputPixelType;
typedef unsigned int                                OutputPixelType;

/** Image typedefs */
const unsigned int                            Dimension = 2;
typedef otb::Image<InputPixelType, Dimension>  InputImageType;
typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

typedef otb::ClampImageFilter<InputImageType, OutputImageType>    FilterType;

int otbClampImageFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  /** instantiating the filter */
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}

int otbClampImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  /** instantiating the filter */
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  filter->SetInput( reader->GetOutput() );
  filter->SetLower( 100 );
  filter->SetUpper( 400 );

  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}

template < class InImageType , class OutImageType >
typename OutImageType::Pointer
Cross ( std::string const & inputFileName )
{
  typedef otb::ImageFileReader< InImageType > ReaderType;
  typedef otb::ClampImageFilter< InImageType , OutImageType > ClampFilter;
  typename ReaderType::Pointer reader ( ReaderType::New() );
  reader->SetFileName( inputFileName );
  typename ClampFilter::Pointer clamp ( ClampFilter::New() );
  clamp->SetInput( reader->GetOutput() );
  clamp->Update();
  return clamp->GetOutput();
}

typedef otb::VectorImage<double> ImageRefType;

template <class ImageType >
bool
CompareImageReal( const ImageRefType::Pointer imRef , 
                  const ImageType * im )
{
  typedef typename ImageType::PixelType RealPixelType;

  RealPixelType min = std::numeric_limits< RealPixelType >::lowest();
  RealPixelType max = std::numeric_limits< RealPixelType >::max();
  auto itRef = itk::ImageRegionConstIterator< ImageRefType >( imRef , 
    imRef->GetLargestPossibleRegion() );
  auto it = itk::ImageRegionConstIterator< ImageType >( im , 
    im->GetLargestPossibleRegion() );
  itRef.GoToBegin();
  it.GoToBegin();
  RealPixelType val;
  double ref;
  while ( !it.IsAtEnd() )
    {
    val = it.Get();
    ref = itRef.Get()[0];
    if ( ref > static_cast<double>( max ) && val != max ) 
      {
      return false;
      }
    else if ( ref < static_cast<double>( min ) && val != min ) 
      {
      return false;
      }
    else if ( static_cast<RealPixelType>( ref ) !=  val )
      {
      return false;
      }
    ++it;
    ++itRef;
    }
    return true;
}

template <class ImageType >
bool
CompareVectorReal( const ImageRefType::Pointer imRef , 
                   const ImageType * im)
{
  typedef typename ImageType::InternalPixelType RealPixelType;
  RealPixelType min = std::numeric_limits< RealPixelType >::lowest();
  RealPixelType max = std::numeric_limits< RealPixelType >::max();
  auto itRef = itk::ImageRegionConstIterator< ImageRefType >( imRef , 
    imRef->GetLargestPossibleRegion() );
  auto it = itk::ImageRegionConstIterator< ImageType >( im , 
    im->GetLargestPossibleRegion() );
  itRef.GoToBegin();
  it.GoToBegin();
  unsigned int nbChanel = im->GetNumberOfComponentsPerPixel ();
  RealPixelType val;
  double ref;
  while ( !it.IsAtEnd() )
    {
    for ( unsigned int i = 0 ; i < nbChanel ; i++ )
      {
      val = it.Get()[i];
      ref = itRef.Get()[i];
      if ( ref > static_cast<double>( max ) && val != max ) 
        {
        return false;
        }
      else if ( ref < static_cast<double>( min ) && val != min ) 
        {
        return false;
        }
      else if ( static_cast<RealPixelType>(ref) !=  val )
        {
        return false;
        }
      }
    std::cout<<itRef.Get()<<std::endl;
    ++it;
    ++itRef;
    }
  return true;
}

template <class ImageType >
bool
CompareImageComplex( const ImageRefType::Pointer imageRef , 
                     const ImageType * im )
{
  typedef typename ImageType::PixelType ComplexType;
  typedef typename ComplexType::value_type RealType;

  RealType min = std::numeric_limits< RealType >::lowest();
  RealType max = std::numeric_limits< RealType >::max();
  auto itRef = itk::ImageRegionConstIterator< ImageRefType >( imageRef , 
    imageRef->GetLargestPossibleRegion() );
  auto it = itk::ImageRegionConstIterator< ImageType >( im , 
    im->GetLargestPossibleRegion() );
  itRef.GoToBegin();
  it.GoToBegin();
  ComplexType val;
  double reRef , imRef;
  while ( !it.IsAtEnd() )
    {
    val = it.Get();
    reRef = itRef.Get()[0];
    imRef = itRef.Get()[1];
    if ( ( reRef > static_cast<double>( max ) && val.real() != max )
      || ( imRef > static_cast<double>( max ) && val.imag() != max ) ) 
      {
      return false;
      }
    else if ( ( reRef < static_cast<double>( min ) && val.real() != min )
           || ( imRef < static_cast<double>( min ) && val.imag() != min ) ) 
      {
      return false;
      }
    else if ( static_cast<RealType>( reRef ) !=  val.real() 
           || static_cast<RealType>( imRef ) !=  val.imag() )
      {
      return false;
      }
    ++it;
    ++itRef;
    }
    return true;
}

template <class ImageType >
bool
CompareVectorComplex( const ImageRefType::Pointer imageRef , 
                     const ImageType * im )
{
  typedef typename ImageType::InternalPixelType ComplexType;
  typedef typename ComplexType::value_type RealType;

  RealType min = std::numeric_limits< RealType >::lowest();
  RealType max = std::numeric_limits< RealType >::max();
  auto itRef = itk::ImageRegionConstIterator< ImageRefType >( imageRef , 
    imageRef->GetLargestPossibleRegion() );
  auto it = itk::ImageRegionConstIterator< ImageType >( im , 
    im->GetLargestPossibleRegion() );
  itRef.GoToBegin();
  it.GoToBegin();
  unsigned int nbChanel = im->GetNumberOfComponentsPerPixel ();
  ComplexType val;
  float reRef , imRef;
  while ( !it.IsAtEnd() )
    {
    for (unsigned int i = 0 ; i < nbChanel ; i++ )
      {
      val = it.Get()[i];
      reRef = itRef.Get()[ 2 * i ];
      imRef = itRef.Get()[ 2 * i + 1 ];
      std::cout<<it.Get()[0].real()<<" , "<<it.Get()[0].imag()<<" ; "<<
      it.Get()[1].real()<<" , "<<it.Get()[1].imag()<<std::endl;
      std::cout<<itRef.Get()<<std::endl;
      if ( ( reRef > static_cast<double>( max ) && val.real() != max )
        || ( imRef > static_cast<double>( max ) && val.imag() != max ) ) 
        {
        return false;
        }
      else if ( ( reRef < static_cast<double>( min ) && val.real() != min )
             || ( imRef < static_cast<double>( min ) && val.imag() != min ) ) 
        {
        return false;
        }
      else if ( static_cast<RealType>( reRef ) != val.real()
             || static_cast<RealType>( imRef ) != val.imag() )
        {
        return false;
        }
      }
    ++it;
    ++itRef;
    }
    return true;
}

int otbClampImageFilterConversionFromRealTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ImageFileReader< ImageRefType > ReaderType;
  ReaderType::Pointer reader ( ReaderType::New() );
  reader->SetFileName( argv[1] );
  reader->Update();
  ImageRefType::Pointer imageRef = reader->GetOutput();
  otb::VectorImage< std::complex<float>>::Pointer image1 = 
    Cross < otb::VectorImage< float >  , otb::VectorImage<std::complex<float>> > ( argv[1] );
  bool test1 = CompareVectorComplex < otb::VectorImage<std::complex<float>> >( imageRef , image1 );


  otb::Image<double>::Pointer image2 = 
    Cross < otb::VectorImage< float >  , otb::Image<double> > ( argv[1] );
  bool test2 = CompareImageReal < otb::Image<double> >( imageRef , image2 );

  otb::Image< std::complex<float>>::Pointer image3 = 
    Cross < otb::VectorImage< float >  , otb::Image<std::complex<float>> > ( argv[1] );
  bool test3 = CompareImageComplex < otb::Image<std::complex<float>> >( imageRef , image3 );

  if (test1 && test2 && test3)
    return EXIT_SUCCESS;
  return 42;
}
