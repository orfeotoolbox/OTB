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
#include "otbOrthoRectification.h"

#include "otbVectorImage.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"
#include "itkExceptionObject.h"

#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbImageToGenericRSOutputParameters.h"

namespace otb
{

template<typename TMapProjection>
int generic_main(otb::ApplicationOptionsResult* parseResult,
                 TMapProjection* mapProjection)
{
  try
  {
    typedef otb::VectorImage<double, 2>       ImageType;
    typedef otb::ImageFileReader<ImageType>           ReaderType;
    typedef otb::StreamingImageFileWriter<ImageType>  WriterType;

    typedef TMapProjection                            MapProjectionType;
    typedef otb::OrthoRectificationFilter< ImageType, ImageType,  MapProjectionType >    OrthorectifFilterType;

    typedef itk::LinearInterpolateImageFunction<ImageType, double>          LinearInterpolationType;
    typedef itk::NearestNeighborInterpolateImageFunction<ImageType, double> NearestNeighborInterpolationType;
    typedef otb::BCOInterpolateImageFunction<ImageType>                     BCOInterpolationType;


    // Read input image information
    ReaderType::Pointer reader=ReaderType::New();
    reader->SetFileName(parseResult->GetInputImage().c_str());
    reader->GenerateOutputInformation();
    ImageType::SizeType lsize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

    // Orthorectification filter
    typename OrthorectifFilterType::Pointer orthofilter = OrthorectifFilterType::New();

    // If activated, generate RPC model
    if(parseResult->IsOptionPresent("RPC"))
      {
      orthofilter->EstimateInputRpcModelOn();
      orthofilter->SetInputRpcGridSize(parseResult->GetParameterUInt("RPC"));
      }
    
    // Compute the output parameters
    typedef otb::ImageToGenericRSOutputParameters<ImageType> OutputParametersEstimatorType;
    typename OutputParametersEstimatorType::Pointer genericRSEstimator = OutputParametersEstimatorType::New();
    genericRSEstimator->SetInput(reader->GetOutput());
    genericRSEstimator->SetOutputProjectionRef(mapProjection->GetWkt());

    // Set up output image informations
    

    ImageType::SizeType size;
    if(parseResult->IsOptionPresent("OutputSize"))
      {
       size[0]= parseResult->GetParameterDouble("OutputSize",0);
       size[1]= parseResult->GetParameterDouble("OutputSize",1);
       genericRSEstimator->ForceSizeTo(size);
       std::cout << "size : " << size << std::endl;
      }

    ImageType::SpacingType spacing;
    if(parseResult->IsOptionPresent("OutputSpacing"))
      {
       spacing[0]= parseResult->GetParameterDouble("OutputSpacing",0);
       spacing[1]= parseResult->GetParameterDouble("OutputSpacing",1);
       std::cout << "spacing : " << spacing << std::endl;
      }
    else
      {
      typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;
      ImageMetadataInterfaceType::Pointer metadataInterface = ImageMetadataInterfaceFactory::CreateIMI(
        reader->GetOutput()->GetMetaDataDictionary());
      spacing[0] = metadataInterface->GetXPixelSpacing();
      spacing[1] = -(metadataInterface->GetYPixelSpacing());
      }
    genericRSEstimator->ForceSpacingTo(spacing);
    
    genericRSEstimator->Compute();
    // set the start index
    ImageType::IndexType start;
    start[0]=0;
    start[1]=0;
    
    //Update now the orthofilter with output image parameters
    ImageType::PixelType defaultValue;
    itk::PixelBuilder<ImageType::PixelType>::Zero(defaultValue,
                                                  reader->GetOutput()->GetNumberOfComponentsPerPixel());

    orthofilter->SetInput(reader->GetOutput());
    ImageType::PointType origin;
    if(parseResult->IsOptionPresent("UpperLeft"))
      {
       origin[0]= parseResult->GetParameterDouble("UpperLeft",0);
       origin[1]= parseResult->GetParameterDouble("UpperLeft",1);
       std::cout << "origin : " << origin << std::endl;
      }
    else
      {
      origin = genericRSEstimator->GetOutputOrigin();
      }
    orthofilter->SetOutputOrigin(origin);
    orthofilter->SetOutputSpacing(genericRSEstimator->GetOutputSpacing());
    orthofilter->SetOutputSize(genericRSEstimator->GetOutputSize());
    orthofilter->SetOutputStartIndex(start);
    orthofilter->SetEdgePaddingValue(defaultValue);

    // Generate deformation field
    if(parseResult->IsOptionPresent("DEMDirectory"))
      {
      orthofilter->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory",0));
      }

    // Set the output map projection
    orthofilter->SetMapProjection(mapProjection);

    if (parseResult->IsOptionPresent("LocMapSpacing"))
      {
      double defScalarSpacing = parseResult->GetParameterFloat("LocMapSpacing");
      ImageType::SpacingType defSpacing;
      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = -defScalarSpacing;

      orthofilter->SetDeformationFieldSpacing(defSpacing);
      }
    else
      {
      orthofilter->SetDeformationFieldSpacing(spacing);
      }

    // Set the interpolator type
    if (parseResult->IsOptionPresent("InterpolatorType"))
      {
      int nbInterpParams = parseResult->GetNumberOfParameters("InterpolatorType") - 1;
      std::string typeInterpolator = parseResult->GetParameterString("InterpolatorType");

      if (typeInterpolator == "BCO")
        {
        BCOInterpolationType::Pointer interpolator = BCOInterpolationType::New();
        if( nbInterpParams == 1)
          interpolator->SetRadius(parseResult->GetParameterUInt("InterpolatorType",1));
        orthofilter->SetInterpolator(interpolator);
        }
      else if (typeInterpolator == "NEARESTNEIGHBOR" && nbInterpParams == 0)
        {
        NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
        orthofilter->SetInterpolator(interpolator);
        }
      else if (typeInterpolator == "LINEAR" && nbInterpParams == 0)
        {
        LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
        orthofilter->SetInterpolator(interpolator);
        }
      else
        {
        itkGenericExceptionMacro(<< "Interpolator type not recognized, choose one with (parameters) : BCO(0/1), NEARESTNEIGHBOR(0), LINEAR(0)");
        }
      }

    //Instantiate the writer
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage());
    writer->SetInput(orthofilter->GetOutput());

    if ( parseResult->IsOptionPresent("NumStreamDivisions") )
    {
      writer->SetTilingStreamDivisions(parseResult->GetParameterULong("NumStreamDivisions"));
    }
    else
    {
      writer->SetTilingStreamDivisions();
    }

    otb::StandardFilterWatcher watcher(writer,"Orthorectification");

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

}//Fin main()


int OrthoRectification::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("FastOrthoRectif");
  descriptor->SetDescription("Using available image metadata to determine the sensor model, computes a cartographic projection of the image");
  descriptor->AddInputImage();
  descriptor->AddOutputImage();
  descriptor->AddOption("UpperLeft","Cartographic X/Y coordinate of upper left corner ","ul",2, false, otb::ApplicationDescriptor::Real);
  //descriptor->AddOption("yUpperLeft","Cartographic y coordinate of upper left corner","yUL",1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("OutputSize","Size of result image in X/Y","size",2, false, otb::ApplicationDescriptor::Integer);
  // descriptor->AddOption("OutputYSize","Size of result image","ySize",1, false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOption("OutputSpacing","Spacing resolution in meters on X/Y Axis","spacing",2, false, otb::ApplicationDescriptor::Real);
  // descriptor->AddOption("OutputYSpacing","Spacing resolution in meters on y Axis","ySpacing",1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("DEMDirectory","Directory where to find the DEM tiles","dem",1,false, otb::ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("NumStreamDivisions","Number of streaming divisions (optional)","stream",1 , false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOptionNParams("MapProjectionType",
                           "Type (UTM/LAMBERT/LAMBERT2/LAMBERT93/SINUS/ECKERT4/TRANSMERCATOR/MOLLWEID) and parameters of map projection used",
                           "mapProj",false, otb::ApplicationDescriptor::String);
  descriptor->AddOption("RPC","Activate RPC sensor model estimation. Parameter is the number of control points per axis.","rpc",1,false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOption("LocMapSpacing","Generate a coarser deformation field with the given spacing.","lmSpacing",1,false, otb::ApplicationDescriptor::Real);
  descriptor->AddOptionNParams("InterpolatorType",
                               "Type LINEAR/BCO/NEARESTNEIGHBOR (optional, linear by default)","interp", false, otb::ApplicationDescriptor::String);
  return EXIT_SUCCESS;
}

int OrthoRectification::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Code
  if ( parseResult->IsOptionPresent("MapProjectionType"))
    {
    std::string typeMap = parseResult->GetParameterString("MapProjectionType",0);
    int nbParams = parseResult->GetNumberOfParameters("MapProjectionType");
    nbParams--;

    if ((typeMap == "UTM")&&(nbParams==2))
      {
      int numZone = parseResult->GetParameterUInt("MapProjectionType",1);
      char hemisphere = parseResult->GetParameterChar("MapProjectionType",2);

      typedef otb::UtmInverseProjection UtmProjectionType;
      UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();

      utmProjection->SetZone(numZone);
      utmProjection->SetHemisphere(hemisphere);

      return generic_main<UtmProjectionType>(parseResult,utmProjection );
      }
    else
      {
      std::vector<double> parameters ;

      for (int i=1; i<nbParams+1; i++)
        {
        parameters.push_back(parseResult->GetParameterDouble("MapProjectionType",i));
        }

      if ((typeMap == "LAMBERT")&&(nbParams==4))
        {
        typedef otb::LambertConformalConicInverseProjection LambertProjectionType;
        LambertProjectionType::Pointer lambertProjection = LambertProjectionType::New();

        lambertProjection->SetParameters(parameters[0],parameters[1],parameters[2],parameters[3]);

        return generic_main<LambertProjectionType>(parseResult, lambertProjection);
        }
      else if ((typeMap == "LAMBERT2")&&(nbParams==0))
        {
        typedef otb::Lambert2EtenduInverseProjection Lambert2ProjectionType;
        Lambert2ProjectionType::Pointer lambert2Projection = Lambert2ProjectionType::New();

        return generic_main<Lambert2ProjectionType>(parseResult,lambert2Projection);
        }
      else if ((typeMap == "LAMBERT93")&&(nbParams==0))
        {
        typedef otb::Lambert93InverseProjection Lambert93ProjectionType;
        Lambert93ProjectionType::Pointer lambert93Projection = Lambert93ProjectionType::New();

        return generic_main<Lambert93ProjectionType>(parseResult,lambert93Projection);
        }
      else if ((typeMap == "SINUS")&&(nbParams==2))
        {
        typedef otb::SinusoidalInverseProjection SinusoidalProjectionType;
        SinusoidalProjectionType::Pointer sinusoidalProjection = SinusoidalProjectionType::New();

        sinusoidalProjection->SetParameters(parameters[0],parameters[1]);

        return generic_main<SinusoidalProjectionType>(parseResult,sinusoidalProjection);
        }
      else if ((typeMap == "ECKERT4")&&(nbParams==2))
        {
        typedef otb::Eckert4InverseProjection Eckert4ProjectionType;
        Eckert4ProjectionType::Pointer eckert4Projection = Eckert4ProjectionType::New();

        eckert4Projection->SetParameters(parameters[0],parameters[1]);

        return generic_main<Eckert4ProjectionType>(parseResult,eckert4Projection);
        }
      else if ((typeMap == "TRANSMERCATOR")&&(nbParams==3))
        {
        typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
        TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();

        transMercatorProjection->SetParameters(parameters[0],parameters[1],parameters[2]);

        return generic_main<TransMercatorProjectionType>(parseResult, transMercatorProjection);
        }
      else if ((typeMap == "MOLLWEID")&&(nbParams==2))
        {
        typedef otb::MollweidInverseProjection MollweidProjectionType;
        MollweidProjectionType::Pointer mollweidProjection = MollweidProjectionType::New();

        mollweidProjection->SetParameters(parameters[0],parameters[1]);

        return generic_main<MollweidProjectionType>(parseResult,mollweidProjection);
        }
      else
        {
        itkGenericExceptionMacro(<< "TypeMap not recognized, choose one with (parameters) : UTM(2), LAMBERT(4), LAMBERT2(0), LAMBERT93(0), SINUS(2), ECKERT4(2), TRANSMERCATOR(3), MOLLWEID(2)");
        }
      }
    }
  else
    {
    //TODO get utm zone and hemispher and build wkt
    }
}
} // namespace otb
