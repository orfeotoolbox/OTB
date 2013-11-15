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
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

#include "otbNCCRegistrationFilter.h"
#include "itkRecursiveGaussianImageFilter.h"

int otbNCCRegistrationFilter(int argc, char* argv[])
{

  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedFileName movingFileName fieldOutName";
    std::cerr << "explorationSize bluringSigma nbIterations ";

    return EXIT_FAILURE;
    }

  const unsigned int ImageDimension = 2;

  typedef double                                      PixelType;
  typedef itk::Vector<double, ImageDimension>         DisplacementPixelType;
  typedef double                                      CoordinateRepresentationType;
  typedef double                                      OutputPixelType;
  typedef otb::Image<OutputPixelType, ImageDimension> OutputImageType;
  typedef otb::Image<PixelType, ImageDimension>       MovingImageType;
  typedef otb::Image<PixelType, ImageDimension>       FixedImageType;
  typedef otb::Image<DisplacementPixelType,
      ImageDimension>       DisplacementFieldType;

  typedef otb::ImageFileReader<FixedImageType> FixedReaderType;
  FixedReaderType::Pointer fReader = FixedReaderType::New();
  fReader->SetFileName(argv[1]);

  typedef otb::ImageFileReader<MovingImageType> MovingReaderType;
  MovingReaderType::Pointer mReader = MovingReaderType::New();
  mReader->SetFileName(argv[2]);

  typedef itk::RecursiveGaussianImageFilter<FixedImageType,
      FixedImageType> FixedBlurType;

  FixedBlurType::Pointer fBlur = FixedBlurType::New();
  fBlur->SetInput(fReader->GetOutput());
  fBlur->SetSigma(atof(argv[5]));

  typedef itk::RecursiveGaussianImageFilter<MovingImageType,
      MovingImageType> MovingBlurType;

  MovingBlurType::Pointer mBlur = MovingBlurType::New();
  mBlur->SetInput(mReader->GetOutput());
  mBlur->SetSigma(atof(argv[5]));

  typedef otb::NCCRegistrationFilter<FixedImageType,
      MovingImageType,
      DisplacementFieldType>
  RegistrationFilterType;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingImage(mBlur->GetOutput());
  registrator->SetFixedImage(fBlur->GetOutput());

  typedef RegistrationFilterType::RadiusType RadiusType;

  RadiusType radius;

  radius[0] = atoi(argv[4]);
  radius[1] = atoi(argv[4]);

  registrator->SetNCCRadius(radius);

  registrator->SetNumberOfIterations(atoi(argv[6]));

  typedef otb::ImageFileWriter<DisplacementFieldType> DFWriterType;
  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput(registrator->GetOutput());
  dfWriter->Update();

  return EXIT_SUCCESS;

}
