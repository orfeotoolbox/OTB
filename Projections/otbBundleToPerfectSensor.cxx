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
#include "otbBundleToPerfectSensor.h"

#include <iostream>

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"

#include "itkExceptionObject.h"

#include "otbStandardWriterWatcher.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "itkPixelBuilder.h"

#include "init/ossimInit.h"
#include "itkFixedArray.h"

#include "otbPipelineMemoryPrintCalculator.h"
#include "itkExtractImageFilter.h"

namespace otb
{
int BundleToPerfectSensor::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("BundleToSensorModel");
  descriptor->SetDescription("Using available image metadata to determine the sensor model, computes a cartographic projection of the image");

  descriptor->AddOption("InputPanchro","The input panchromatic image","inP", 1,true,otb::ApplicationDescriptor::InputImage);
  descriptor->AddOption("InputXS","The input multi-spectral image","inXS", 1,true,otb::ApplicationDescriptor::InputImage);
  descriptor->AddOption("DEMDirectory","Directory were to find the DEM tiles","dem",1,false,otb::ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("LocMapSpacing","Generate a coarser deformation field with the given spacing.","lmSpacing",1,false,otb::ApplicationDescriptor::Real);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram",1,false, otb::ApplicationDescriptor::Integer);

  descriptor->AddOutputImage();

  return EXIT_SUCCESS;
}

int BundleToPerfectSensor::Execute(otb::ApplicationOptionsResult* parseResult)
{
  try
    {

    typedef unsigned short int PixelType;

    typedef otb::VectorImage<PixelType, 2>                XsImageType;
    typedef otb::Image<PixelType,2>                       PanImageType;
    typedef otb::ImageFileReader<XsImageType>             XsReaderType;
    typedef otb::ImageFileReader<PanImageType>            PanReaderType;
    typedef otb::StreamingImageFileWriter<XsImageType>    WriterType;
    typedef otb::BCOInterpolateImageFunction<XsImageType> InterpolatorType;

    typedef otb::GenericRSResampleImageFilter<XsImageType,XsImageType>  ResamplerType;

    typedef otb::SimpleRcsPanSharpeningFusionImageFilter<PanImageType,XsImageType,XsImageType> FusionFilterType;

    typedef itk::ExtractImageFilter<XsImageType,XsImageType> ExtractFilterType;

    // Read input images information
    PanReaderType::Pointer preader= PanReaderType::New();
    preader->SetFileName(parseResult->GetParameterString("InputPanchro"));
    preader->GenerateOutputInformation();

    XsReaderType::Pointer xsreader= XsReaderType::New();
    xsreader->SetFileName(parseResult->GetParameterString("InputXS"));
    xsreader->GenerateOutputInformation();
    
    // Resample filter 
    ResamplerType::Pointer    resampler = ResamplerType::New();
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    resampler->SetInterpolator(interpolator);
    
    // Add DEM if any
    if(parseResult->IsOptionPresent("DEMDirectory"))
      {
      resampler->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory",0));
      }
    
    // Set up output image informations
    XsImageType::SpacingType spacing = preader->GetOutput()->GetSpacing();
    XsImageType::IndexType start = preader->GetOutput()->GetLargestPossibleRegion().GetIndex();
    XsImageType::SizeType size = preader->GetOutput()->GetLargestPossibleRegion().GetSize();
    XsImageType::PointType origin = preader->GetOutput()->GetOrigin();

    if(parseResult->IsOptionPresent("LocMapSpacing"))
      {
      double defScalarSpacing = parseResult->GetParameterFloat("LocMapSpacing");
      std::cout<<"Generating coarse deformation field (spacing="<<defScalarSpacing<<")"<<std::endl;
      XsImageType::SpacingType defSpacing;

      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = defScalarSpacing;
      
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    else
      {
      XsImageType::SpacingType defSpacing;
      defSpacing[0]=10*spacing[0];
      defSpacing[1]=10*spacing[1];
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    
    XsImageType::PixelType defaultValue;
    itk::PixelBuilder<XsImageType::PixelType>::Zero(defaultValue,
                                                    xsreader->GetOutput()->GetNumberOfComponentsPerPixel());

    resampler->SetInput(xsreader->GetOutput());
    resampler->SetOutputOrigin(origin);
    resampler->SetOutputSpacing(spacing);
    resampler->SetOutputSize(size);
    resampler->SetOutputStartIndex(start);
    resampler->SetOutputKeywordList(preader->GetOutput()->GetImageKeywordlist());
    resampler->SetOutputProjectionRef(preader->GetOutput()->GetProjectionRef());
    resampler->SetEdgePaddingValue(defaultValue);

    FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
    fusionFilter->SetPanInput(preader->GetOutput());
    fusionFilter->SetXsInput(resampler->GetOutput());
    fusionFilter->GetOutput()->UpdateOutputInformation();
    
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage());
    writer->SetInput(fusionFilter->GetOutput());
    writer->SetWriteGeomFile(true);

    otb::StandardWriterWatcher w4(writer,resampler,"Perfect sensor fusion");

    // Estimate memory print
    otb::PipelineMemoryPrintCalculator::Pointer memoryPrintCalculator = otb::PipelineMemoryPrintCalculator::New();
    const double byteToMegabyte = 1./vcl_pow(2.0, 20);
    
    // Trick to avoid having the resampler compute the whole
    // deformation field
    ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
    extractFilter->SetInput(fusionFilter->GetOutput());
    XsImageType::RegionType smallRegion;
    XsImageType::SizeType smallSize;
    smallSize.Fill(100);
    XsImageType::IndexType index;
    index[0] = fusionFilter->GetOutput()->GetLargestPossibleRegion().GetIndex()[0]
      + fusionFilter->GetOutput()->GetLargestPossibleRegion().GetSize()[0]/2 - 50;
    index[1] = fusionFilter->GetOutput()->GetLargestPossibleRegion().GetIndex()[1]
      + fusionFilter->GetOutput()->GetLargestPossibleRegion().GetSize()[1]/2 - 50;
    smallRegion.SetSize(smallSize);
    smallRegion.SetIndex(index);

    extractFilter->SetExtractionRegion(smallRegion);

    bool smallRegionSuccess = smallRegion.Crop(fusionFilter->GetOutput()->GetLargestPossibleRegion());
    
    if( smallRegionSuccess)
      {
      memoryPrintCalculator->SetDataToWrite(extractFilter->GetOutput());
      double regionTrickFactor = (double)fusionFilter->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()
        /(double)(smallRegion.GetNumberOfPixels());
      memoryPrintCalculator->SetBiasCorrectionFactor(1.27 * regionTrickFactor);
      }
    else
      {
      memoryPrintCalculator->SetDataToWrite(fusionFilter->GetOutput());
      memoryPrintCalculator->SetBiasCorrectionFactor(1.27);
      }

      memoryPrintCalculator->SetAvailableMemory(256 / byteToMegabyte);
    
    if (parseResult->IsOptionPresent("AvailableMemory"))
      {
      long long int memory = static_cast <long long int> (parseResult->GetParameterUInt("AvailableMemory"));
      memoryPrintCalculator->SetAvailableMemory(memory / byteToMegabyte);
      }

    memoryPrintCalculator->Compute();

    std::cout<<"Total memory usage: "<<memoryPrintCalculator->GetMemoryPrint()*byteToMegabyte<<" Mb"<<std::endl;
    std::cout<<"Optimal stream division: "<<memoryPrintCalculator->GetOptimalNumberOfStreamDivisions()<<std::endl;


    writer->SetTilingStreamDivisions(memoryPrintCalculator->GetOptimalNumberOfStreamDivisions());
   
    writer->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cout << "Exception itk::ExceptionObject raised !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  catch ( std::bad_alloc & err )
    {
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl;
    return EXIT_FAILURE;
    }
  catch ( ... )
    {
    std::cout << "Unknown exception raised !" << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;


}

}
