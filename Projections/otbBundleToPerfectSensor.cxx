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

#include "otbMetaDataKey.h"
#include "otbCommandLineArgumentParser.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbGenericRSTransform.h"
#include "otbImage.h"
#include "otbDEMHandler.h"
#include "otbVectorImage.h"
#include "otbMacro.h"

#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkTransform.h"

#include "otbStandardFilterWatcher.h"
#include "itkTransformToDeformationFieldSource.h"
#include "otbStreamingWarpImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

#include "otbMultiChannelExtractROI.h"

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
//    parser->AddOption("--DEMDirectory","Directory were to find the DEM tiles","-dem",1,false);
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
    typedef otb::GenericRSTransform<>                  RSTransformType;
    typedef RSTransformType::InputPointType   PointType;

    typedef itk::FixedArray<double,2> DeformationValueType;
    typedef otb::Image<DeformationValueType> DeformationFieldType;
    typedef otb::StreamingImageFileWriter<DeformationFieldType> DeformationFieldWriterType;

    typedef itk::TransformToDeformationFieldSource<DeformationFieldType,double> DeformationFieldGeneratorType;
    typedef otb::StreamingWarpImageFilter<XsImageType,XsImageType,DeformationFieldType> WarpFilterType;
    typedef itk::LinearInterpolateImageFunction<XsImageType,double> InterpolatorType;

    typedef otb::SimpleRcsPanSharpeningFusionImageFilter<PanImageType,XsImageType,XsImageType> FusionFilterType;

    // Read input images information
    PanReaderType::Pointer preader= PanReaderType::New();
    preader->SetFileName(parseResult->GetParameterString("--InputPanchro"));
    preader->GenerateOutputInformation();

    XsReaderType::Pointer xsreader= XsReaderType::New();
    xsreader->SetFileName(parseResult->GetParameterString("--InputXS"));
    xsreader->GenerateOutputInformation();

    // Generate RS Transform
    RSTransformType::Pointer rsTransform = RSTransformType::New();
    rsTransform->SetOutputKeywordList(xsreader->GetOutput()->GetImageKeywordlist());
    rsTransform->SetInputKeywordList(preader->GetOutput()->GetImageKeywordlist());

//    if(parseResult->IsOptionPresent("--DEMDirectory"))
//      {
//      rsTransform->SetDEMDirectory(parseResult->GetParameterString("--DEMDirectory",0));
//      gcp2sensor->SetUseDEM(true);
//      otb::DEMHandler::Pointer demHandler = otb::DEMHandler::New();
//      demHandler->OpenDEMDirectory(parseResult->GetParameterString("--DEMDirectory",0).c_str());
//      gcp2sensor->SetDEMHandler(demHandler);
//      }

    rsTransform->InstanciateTransform();


    // Set up output image informations
    XsImageType::SpacingType spacing = preader->GetOutput()->GetSpacing();
    XsImageType::IndexType start = preader->GetOutput()->GetLargestPossibleRegion().GetIndex();
    XsImageType::SizeType size = preader->GetOutput()->GetLargestPossibleRegion().GetSize();
    XsImageType::PointType origin = preader->GetOutput()->GetOrigin();

    DeformationFieldGeneratorType::Pointer dfGenerator = DeformationFieldGeneratorType::New();
    dfGenerator->SetOutputOrigin(origin);
    dfGenerator->SetOutputIndex(start);
    if(parseResult->IsOptionPresent("--LocMapSpacing"))
      {
      double defScalarSpacing = parseResult->GetParameterFloat("--LocMapSpacing");
      std::cout<<"Generating coarse deformation field (spacing="<<defScalarSpacing<<")"<<std::endl;
      XsImageType::SpacingType defSpacing;
      XsImageType::SizeType defSize;

      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = defScalarSpacing;

      for(unsigned int dim = 0; dim < XsImageType::ImageDimension;++dim)
        {
        defSize[dim] = static_cast<unsigned long>(size[dim]*vcl_abs(spacing[dim]/defSpacing[dim]));
        }
      std::cout<<"Deformation field spacing: "<<defSpacing<<std::endl;
      std::cout<<"Deformation field size: "<<defSize<<std::endl;
      dfGenerator->SetOutputSpacing(defSpacing);
      dfGenerator->SetOutputSize(defSize);
      }
    else
      {
      dfGenerator->SetOutputSpacing(spacing);
      dfGenerator->SetOutputSize(size);
      }
    dfGenerator->SetTransform(rsTransform);

    WarpFilterType::Pointer warper = WarpFilterType::New();

    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    interpolator->SetInputImage(xsreader->GetOutput());
    warper->SetInterpolator(interpolator);

    XsImageType::PixelType defaultValue;
    itk::PixelBuilder<XsImageType::PixelType>::Zero(defaultValue,xsreader->GetOutput()->GetNumberOfComponentsPerPixel());

    warper->SetInput(xsreader->GetOutput());
    warper->SetDeformationField(dfGenerator->GetOutput());
    warper->SetOutputOrigin(origin);
    warper->SetOutputSpacing(spacing);
    warper->SetOutputSize(size);
    warper->SetOutputStartIndex(start);
    warper->SetEdgePaddingValue(defaultValue);

    FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
    fusionFilter->SetPanInput(preader->GetOutput());
    fusionFilter->SetXsInput(warper->GetOutput());
    fusionFilter->GetOutput()->UpdateOutputInformation();

    std::cout<<"Output size: "<<fusionFilter->GetOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;


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
