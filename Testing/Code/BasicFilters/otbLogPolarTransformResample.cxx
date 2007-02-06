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

    const unsigned int Dimension = 2;
    typedef double PrecisionType;
    typedef double PixelType;
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
    interpolator->SetInputImage(reader->GetOutput());

    std::cout<<interpolator<<std::endl;
    LogPolarTransformType::Pointer transform = LogPolarTransformType::New();
    
    LogPolarTransformType::ParametersType params(4);
    // Center the transform
    params[0]=0.5*static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]);
    params[1]=0.5*static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]);
    params[2]=360./1024;
    params[3]=vcl_log(vcl_sqrt(vcl_pow(static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]),2)
+vcl_pow(static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]),2))/2)/512;
    transform->SetParameters(params);

    ImageType::SizeType size;
    size[0]=1024;
    size[1]=512;

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
