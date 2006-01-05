/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: VectorGradientAnisotropicDiffusionFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2003/07/25 14:12:35 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkRGBPixel.h"


int main( int argc, char * argv[] )
{

  if( argc < 5 ) 
    { 
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputGradientImageFile ";
    std::cerr << "numberOfIterations  timeStep  " << std::endl;
    return 1;
    }

  typedef float PixelComponentType;
  const unsigned long Dimension = 3;

  typedef itk::RGBPixel< PixelComponentType >    PixelType;
  typedef itk::Image< PixelType, Dimension  >    ImageType;

  typedef itk::RGBPixel< unsigned char >         OutputPixelType;
  typedef itk::Image< OutputPixelType, 
                                 Dimension  >    OutputImageType;


  typedef itk::ImageFileReader< ImageType >  ReaderType;

  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  typedef itk::VectorCastImageFilter< 
                               ImageType, 
                               OutputImageType > CasterType;

  typedef itk::VectorGradientAnisotropicDiffusionImageFilter<
                       ImageType, ImageType >  FilterType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );

  const unsigned int numberOfIterations = atoi( argv[3] );
  const double       timeStep           = atof( argv[4] );

  filter->SetNumberOfIterations( numberOfIterations );
  filter->SetTimeStep( timeStep );
  filter->SetConductanceParameter( 3.0 );
  
  filter->Update();

  CasterType::Pointer caster = CasterType::New();

  WriterType::Pointer writer = WriterType::New();

  caster->SetInput( filter->GetOutput() );
  writer->SetInput( caster->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;

}

