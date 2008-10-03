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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {fixed.tif}, {moving.tif}
//    OUTPUTS: {deformationFieldOutput2.png}
//    5 1.0 2
//  Software Guide : EndCommandLineArgs



#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbNCCRegistrationFilter.h"
#include "otbCommandLineArgumentParser.h"
#include "itkRecursiveGaussianImageFilter.h"

#include <iostream>


int main(int argc, char** argv )
{
  
  const unsigned int ImageDimension = 2;

  typedef double                                PixelType;
  typedef itk::Vector<double,ImageDimension>    DeformationPixelType;

  typedef double                   CoordinateRepresentationType;

  //Allocate Images
  typedef otb::Image<PixelType,ImageDimension>         MovingImageType;
  typedef otb::Image<PixelType,ImageDimension>         FixedImageType;
  typedef otb::Image<DeformationPixelType,
                               ImageDimension>         DeformationFieldType;

  typedef otb::ImageFileReader< FixedImageType > FixedReaderType;
  FixedReaderType::Pointer fReader = FixedReaderType::New();
  fReader->SetFileName(argv[1]);  

  typedef otb::ImageFileReader< MovingImageType > MovingReaderType;
  MovingReaderType::Pointer mReader = MovingReaderType::New();
  mReader->SetFileName(argv[2]);  


  //Blur input images

  typedef itk::RecursiveGaussianImageFilter< FixedImageType,
    FixedImageType > FixedBlurType;

  FixedBlurType::Pointer fBlur = FixedBlurType::New();
  fBlur->SetInput( fReader->GetOutput() );
  fBlur->SetSigma( atof(argv[5]) );


  typedef itk::RecursiveGaussianImageFilter< MovingImageType,
    MovingImageType > MovingBlurType;

  MovingBlurType::Pointer mBlur = MovingBlurType::New();
  mBlur->SetInput( mReader->GetOutput() );
  mBlur->SetSigma( 1.0 );


  //Create the filter
  typedef otb::NCCRegistrationFilter< FixedImageType, 
                                       MovingImageType,
                                       DeformationFieldType >
                                           RegistrationFilterType;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingImage( mBlur->GetOutput() );
  registrator->SetFixedImage( fBlur->GetOutput() );


  typedef RegistrationFilterType::RadiusType RadiusType;

  RadiusType radius;

  radius[0] = atoi(argv[4]);
  radius[1] = atoi(argv[4]);

  registrator->SetNCCRadius( radius );

  std::cout << "NCC radius " << registrator->GetNCCRadius() << std::endl;
  
  
  registrator->SetNumberOfIterations( atoi(argv[6]) );

  registrator->GetDeformationField();

  registrator->Update();



  typedef otb::ImageFileWriter< DeformationFieldType > DFWriterType;

  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput( registrator->GetOutput() );
  dfWriter->Update();

  
  return EXIT_SUCCESS;

}

