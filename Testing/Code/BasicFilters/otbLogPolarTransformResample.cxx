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
#include "otbLogPolarTransform.h"
#include "otbImage.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkPoint.h"
#include "otbMacro.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbLogPolarTransformResample(int argc, char* argv[])
{
try
  {
    char * inputFileName = argv[1];
    char * outputFileName = argv[2];
    double radialStep = atof(argv[3]);
    double angularStep = atof(argv[4]);

    const unsigned int Dimension = 2;
    typedef double PrecisionType;
    typedef unsigned char PixelType;
    typedef otb::Image<PixelType,Dimension> ImageType;
    typedef otb::LogPolarTransform<PrecisionType> LogPolarTransformType;
    typedef itk::LinearInterpolateImageFunction<ImageType,PrecisionType> InterpolatorType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    typedef otb::ImageFileWriter<ImageType> WriterType;
    typedef itk::ResampleImageFilter<ImageType,ImageType,PrecisionType> ResampleFilterType;


    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFileName);
    reader->Update();

    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    LogPolarTransformType::Pointer transform = LogPolarTransformType::New();
    
    LogPolarTransformType::ParametersType params(4);
    // Center the transform
    params[0]=0.5*static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]);
    params[1]=0.5*static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]);
    params[2]=radialStep;
    params[3]=angularStep;
    transform->SetParameters(params);

    ImageType::SizeType size;
    size[0]=512;
    size[1]=1024;

    ImageType::SpacingType spacing;
    spacing.Fill(1.0);

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetInput(reader->GetOutput());
    resampler->SetTransform(transform);
    resampler->SetInterpolator(interpolator);
    resampler->SetDefaultPixelValue(0);
    resampler->SetSize(size);
   
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFileName);
    writer->SetInput(resampler->GetOutput());
    writer->Update();

  }
catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
catch( ... ) 
  { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
  } 
 return EXIT_SUCCESS;













}
