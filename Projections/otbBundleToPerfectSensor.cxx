/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


#include <iostream>

#include "otbCommandLineArgumentParser.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbGenericRSResampleImageFilter.h"

#include "itkExceptionObject.h"

#include "otbStandardFilterWatcher.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "itkPixelBuilder.h"

#include "init/ossimInit.h"
#include "itkFixedArray.h"

int main(int argc, char* argv[])
{
  try
  {

    ossimInit::instance()->initialize(argc, argv);

    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("Using available image metadata to determine the sensor model, computes a cartographic projection of the image");
    parser->AddOutputImage();
    parser->AddOption("--DEMDirectory","Directory were to find the DEM tiles","-dem",1,false);
    parser->AddOption("--NumStreamDivisions","Number of streaming divisions (optional)","-stream",1,false);
    parser->AddOption("--LocMapSpacing","Generate a coarser deformation field with the given spacing.","-lmSpacing",1,false);
    parser->AddOption("--InputPanchro","The input panchromatic image","-inP", 1,true);
    parser->AddOption("--InputXS","The input multi-spectral image","-inXS", 1,true);


    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer  parseResult = ParserResultType::New();

    try
    {
      parser->ParseCommandLine(argc,argv,parseResult);
    }
    catch ( itk::ExceptionObject & err )
    {
      std::string descriptionException = err.GetDescription();
      if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
        {
        std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
        return EXIT_SUCCESS;
        }
      if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
        {
        return EXIT_SUCCESS;
        }
      return EXIT_FAILURE;
    }

    typedef unsigned short int PixelType;

    typedef otb::VectorImage<PixelType, 2>          XsImageType;
    typedef otb::Image<PixelType,2>                 PanImageType;
    typedef otb::ImageFileReader<XsImageType>          XsReaderType;
    typedef otb::ImageFileReader<PanImageType>         PanReaderType;
    typedef otb::StreamingImageFileWriter<XsImageType> WriterType;

    typedef otb::GenericRSResampleImageFilter<XsImageType,XsImageType>  ResamplerType;

    typedef otb::SimpleRcsPanSharpeningFusionImageFilter<PanImageType,XsImageType,XsImageType> FusionFilterType;

    // Read input images information
    PanReaderType::Pointer preader= PanReaderType::New();
    preader->SetFileName(parseResult->GetParameterString("--InputPanchro"));
    preader->GenerateOutputInformation();

    XsReaderType::Pointer xsreader= XsReaderType::New();
    xsreader->SetFileName(parseResult->GetParameterString("--InputXS"));
    xsreader->GenerateOutputInformation();
    
    // Resample filter 
    ResamplerType::Pointer    resampler = ResamplerType::New();
    
    // Add DEM if any
    if(parseResult->IsOptionPresent("--DEMDirectory"))
      {
      resampler->SetDEMDirectory(parseResult->GetParameterString("--DEMDirectory",0));
      }
    
    // Set up output image informations
    XsImageType::SpacingType spacing = preader->GetOutput()->GetSpacing();
    XsImageType::IndexType start = preader->GetOutput()->GetLargestPossibleRegion().GetIndex();
    XsImageType::SizeType size = preader->GetOutput()->GetLargestPossibleRegion().GetSize();
    XsImageType::PointType origin = preader->GetOutput()->GetOrigin();

    if(parseResult->IsOptionPresent("--LocMapSpacing"))
      {
      double defScalarSpacing = parseResult->GetParameterFloat("--LocMapSpacing");
      std::cout<<"Generating coarse deformation field (spacing="<<defScalarSpacing<<")"<<std::endl;
      XsImageType::SpacingType defSpacing;

      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = defScalarSpacing;
      
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    else
      {
      resampler->SetDeformationFieldSpacing(spacing);
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
    resampler->SetEdgePaddingValue(defaultValue);

    FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
    fusionFilter->SetPanInput(preader->GetOutput());
    fusionFilter->SetXsInput(resampler->GetOutput());
    fusionFilter->GetOutput()->UpdateOutputInformation();
    
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage());
    writer->SetInput(fusionFilter->GetOutput());

    otb::StandardFilterWatcher w4(writer,"Perfect sensor fusion");


    if ( parseResult->IsOptionPresent("--NumStreamDivisions") )
    {
      std::cout<<"Setting number of stream division to "<<parseResult->GetParameterULong("--NumStreamDivisions")<<std::endl;
      writer->SetTilingStreamDivisions(parseResult->GetParameterULong("--NumStreamDivisions"));
    }
    else
    {
      writer->SetTilingStreamDivisions();
    }

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
