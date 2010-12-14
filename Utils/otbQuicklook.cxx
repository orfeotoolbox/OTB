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

#include "otbQuicklook.h"

#include <iostream>

#include "otbMultiChannelExtractROI.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingShrinkImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbConfigure.h"

namespace otb
{

int Quicklook::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Quicklook");
  descriptor->SetDescription("Generates a subsampled version of an image");
  descriptor->AddInputImage();
  descriptor->AddOutputImage();
  descriptor->AddOptionNParams("ChannelList", "channel list","cl",false,ApplicationDescriptor::StringList);
  descriptor->AddOption("ROIStartX","first point of ROI in x-direction ","ROIx0",1,false,ApplicationDescriptor::Integer);
  descriptor->AddOption("ROIStartY","first point of ROI in y-direction ","ROIy0",1,false,ApplicationDescriptor::Integer);
  descriptor->AddOption("ROISizeX","size of ROI in x-direction ","ROINx",1,false,ApplicationDescriptor::Integer);
  descriptor->AddOption("ROISizeY","size of ROI in y-direction ","ROINy",1,false,ApplicationDescriptor::Integer);

  descriptor->AddOption("SizeX","size of quicklook in x-direction ","Nx",1,false,ApplicationDescriptor::Integer);
  descriptor->AddOption("SizeY","size of quicklook in y-direction ","Ny",1,false,ApplicationDescriptor::Integer);

  descriptor->AddOption("SamplingRatio","Sampling Ratio ","sr",1,false,ApplicationDescriptor::Real);

  return EXIT_SUCCESS;
}

int Quicklook::Execute(otb::ApplicationOptionsResult* parseResult)
{
  try
    {
    typedef double InputPixelType;
    typedef double OutputPixelType;
    typedef otb::MultiChannelExtractROI< InputPixelType,OutputPixelType >   ExtractROIFilterType;

    // Defining the IO filename
    typedef otb::ImageFileReader< ExtractROIFilterType::InputImageType >    ReaderType;
    typedef otb::ImageFileWriter< ExtractROIFilterType::OutputImageType>    WriterType;

    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    ReaderType::Pointer           reader           = ReaderType::New();
    WriterType::Pointer           writer           = WriterType::New();

    reader->SetFileName( parseResult->GetInputImage().c_str()  );
    writer->SetFileName( parseResult->GetOutputImage().c_str() );

    // Setting the ROIFilter parameters
    //Determine if using streaming
    reader->GenerateOutputInformation();

    unsigned long regionROI[2];

    if ( parseResult->IsOptionPresent("ROIStartX") )
      extractROIFilter->SetStartX(parseResult->GetParameterULong("ROIStartX"));

    if ( parseResult->IsOptionPresent("ROIStartY") )
      extractROIFilter->SetStartY(parseResult->GetParameterULong("ROIStartY"));

    if ( parseResult->IsOptionPresent("ROISizeX") )
      {
      regionROI[0] = parseResult->GetParameterULong("ROISizeX");
      extractROIFilter->SetSizeX(regionROI[0]);
      }
    else
      {
      regionROI[0] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
      }
    if ( parseResult->IsOptionPresent("ROISizeY") )
      {
      regionROI[1] = parseResult->GetParameterULong("ROISizeY");
      extractROIFilter->SetSizeY(regionROI[1]);
      }
    else
      {
      regionROI[1] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
      }

    if ( parseResult->IsOptionPresent("ChannelList") )
      {
      int NumberOfChannel = parseResult->GetNumberOfParameters("ChannelList");
      for (int i = 0 ; i < NumberOfChannel ; i++)
        {
        unsigned int ChannelNumber = parseResult->GetParameterUInt("ChannelList",i);
        extractROIFilter->SetChannel(ChannelNumber);
        }
      }

    if (    (extractROIFilter->GetSizeX() < 0) ||
            (extractROIFilter->GetSizeY() < 0)   )
      {
      std::cout << " Size of ROI in x-direction and y -direction must be positive" << std::endl;
      return EXIT_SUCCESS;
      }

    extractROIFilter->SetInput( reader->GetOutput() );
    //  extractROIFilter->Update();
    // extractROIFilter->GenerateOutputInformation();

    double ROISizeX =  static_cast<double>( regionROI[0] );
    double ROISizeY =  static_cast<double>( regionROI[1] );

    unsigned int Ratio = 2; //default value
    double SamplingRatioX =1.0;
    double SamplingRatioY =1.0;

    if ( parseResult->IsOptionPresent("SamplingRatio") )
      {
      Ratio = static_cast<unsigned int>(parseResult->GetParameterDouble("SamplingRatio"));
      if (Ratio <=1 )
        {
        std::cout << " Error in SamplingRatio value : value must be greater than 1" << std::endl;
        return EXIT_FAILURE;
        }
      }
    else
      {
      if ( parseResult->IsOptionPresent("SizeX") && parseResult->IsOptionPresent("SizeY") )
        {
        SamplingRatioX =  ROISizeX / parseResult->GetParameterDouble("SizeX");
        SamplingRatioY =  ROISizeY / parseResult->GetParameterDouble("SizeY");
        if (SamplingRatioX > Ratio) Ratio = static_cast<unsigned int>(SamplingRatioX);
        if (SamplingRatioY > Ratio) Ratio = static_cast<unsigned int>(SamplingRatioY);
        }
      else
        {
        if ( parseResult->IsOptionPresent("SizeX") )
          {
          Ratio =  static_cast<unsigned int>(ROISizeX / parseResult->GetParameterDouble("SizeX"));
          }

        if ( parseResult->IsOptionPresent("SizeY") )
          {
          Ratio =  static_cast<unsigned int>(ROISizeY / parseResult->GetParameterDouble("SizeY"));
          }
        }
      if ( Ratio <= 1.)
        {
        std::cout << "Error in SizeX and/or SizeY : values must be greater than 1." << std::endl;
        return EXIT_FAILURE;
        }
      }
    
    // Setting the ShrinkImageFilter parameters
    /*        typedef itk::ShrinkImageFilter< ExtractROIFilterType::OutputImageType,
        ExtractROIFilterType::OutputImageType >  ShrinkImageFilterType;*/
    typedef otb::StreamingShrinkImageFilter<ExtractROIFilterType::OutputImageType,ExtractROIFilterType::OutputImageType> ShrinkImageFilterType;

    ShrinkImageFilterType::Pointer ResamplingFilter = ShrinkImageFilterType::New();
    ResamplingFilter->SetInput( extractROIFilter->GetOutput() );
    ResamplingFilter->SetShrinkFactor( Ratio );
    ResamplingFilter->GenerateOutputInformation();
    otbGenericMsgDebugMacro(<<ResamplingFilter->GetOutput()->GetLargestPossibleRegion());
    //ResamplingFilter->Update();
    writer->SetInput(ResamplingFilter->GetOutput() );
    writer->Update();
    
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cout << "Following otbException catch :" << std::endl;
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
    std::cout << "Unknown Exception found !" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
}
