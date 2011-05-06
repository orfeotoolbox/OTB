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
#include "otbStandardWriterWatcher.h"

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"
#include "itkExceptionObject.h"
#include "otbMacro.h"

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
    typedef otb::VectorImage<unsigned short int, 2>   ImageType;
    typedef otb::ImageFileReader<ImageType>           ReaderType;
    typedef otb::StreamingImageFileWriter<ImageType>  WriterType;

    typedef TMapProjection                                                            MapProjectionType;
    typedef otb::OrthoRectificationFilter< ImageType, ImageType,  MapProjectionType > OrthorectifFilterType;

    typedef itk::LinearInterpolateImageFunction<ImageType, double>          LinearInterpolationType;
    typedef itk::NearestNeighborInterpolateImageFunction<ImageType, double> NearestNeighborInterpolationType;
    typedef otb::BCOInterpolateImageFunction<ImageType>                     BCOInterpolationType;

    typedef otb::PipelineMemoryPrintCalculator           MemoryCalculatorType;
    typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractFilterType;

    // Read input image information
    ReaderType::Pointer reader=ReaderType::New();
    reader->SetFileName(parseResult->GetInputImage().c_str());
    reader->GenerateOutputInformation();
    ImageType::SizeType lsize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

    // Orthorectification filter
    typename OrthorectifFilterType::Pointer orthoFilter = OrthorectifFilterType::New();
    orthoFilter->SetInput(reader->GetOutput());

    // If activated, generate RPC model
    if(parseResult->IsOptionPresent("RPC"))
      {
      orthoFilter->EstimateInputRpcModelOn();
      orthoFilter->SetInputRpcGridSize(parseResult->GetParameterUInt("RPC"));
      }

    // Compute the output parameters
    typedef otb::ImageToGenericRSOutputParameters<ImageType> OutputParametersEstimatorType;
    typename OutputParametersEstimatorType::Pointer genericRSEstimator = OutputParametersEstimatorType::New();
    genericRSEstimator->SetInput(reader->GetOutput());
    genericRSEstimator->SetOutputProjectionRef(mapProjection->GetWkt());

    // Set up output image informations

    ImageType::SpacingType spacing;
    if(parseResult->IsOptionPresent("OutputSpacing"))
      {
      spacing[0]= parseResult->GetParameterDouble("OutputSpacing", 0);
      spacing[1]= parseResult->GetParameterDouble("OutputSpacing", 1);
      }
    else
      {
      typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;
      ImageMetadataInterfaceType::Pointer metadataInterface = ImageMetadataInterfaceFactory::CreateIMI(
          reader->GetOutput()->GetMetaDataDictionary());
      double isotropicSpacing = std::max(metadataInterface->GetXPixelSpacing(), metadataInterface->GetYPixelSpacing());

      spacing[0] =  isotropicSpacing;
      spacing[1] = -isotropicSpacing;
      }
    genericRSEstimator->ForceSpacingTo(spacing);

    otbMsgDevMacro(<< "Output image spacing: " << spacing);

    genericRSEstimator->Compute();
    // set the start index
    ImageType::IndexType start;
    start[0]=0;
    start[1]=0;

    //Update now the orthoFilter with output image parameters
    ImageType::PixelType defaultValue;
    itk::PixelBuilder<ImageType::PixelType>::Zero(defaultValue,
                                                  reader->GetOutput()->GetNumberOfComponentsPerPixel());

    orthoFilter->SetInput(reader->GetOutput());

    ImageType::PointType origin;
    if(parseResult->IsOptionPresent("UpperLeft"))
      {
      origin[0]= parseResult->GetParameterDouble("UpperLeft", 0);
      origin[1]= parseResult->GetParameterDouble("UpperLeft", 1);
      }
    else
      {
      origin = genericRSEstimator->GetOutputOrigin();
      }

    otbMsgDevMacro(<< "Output image origin: " << origin);

    ImageType::SizeType size;
    if(parseResult->IsOptionPresent("OutputSize"))
      {
      size[0]= parseResult->GetParameterDouble("OutputSize", 0);
      size[1]= parseResult->GetParameterDouble("OutputSize", 1);
      genericRSEstimator->ForceSizeTo(size);
      }
    else
      {
      size = genericRSEstimator->GetOutputSize();
      }

    otbMsgDevMacro(<< "Output image size: " << size);

    orthoFilter->SetOutputOrigin(origin);
    orthoFilter->SetOutputSpacing(genericRSEstimator->GetOutputSpacing());
    orthoFilter->SetOutputSize(size);
    orthoFilter->SetOutputStartIndex(start);
    orthoFilter->SetEdgePaddingValue(defaultValue);

    // Generate deformation field
    if(parseResult->IsOptionPresent("DEMDirectory"))
      {
      orthoFilter->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory", 0));
      }

    // Set the output map projection
    orthoFilter->SetMapProjection(mapProjection);

    if (parseResult->IsOptionPresent("LocMapSpacing"))
      {
      double defScalarSpacing = parseResult->GetParameterFloat("LocMapSpacing");
      ImageType::SpacingType defSpacing;
      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = -defScalarSpacing;

      orthoFilter->SetDeformationFieldSpacing(defSpacing);
      }
    else
      {
      // By default, generate a 10 times coarser deformation field
      ImageType::SpacingType defSpacing;
      defSpacing[0] = 10*spacing[0];
      defSpacing[1] = 10*spacing[1];
      orthoFilter->SetDeformationFieldSpacing(defSpacing);
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
          interpolator->SetRadius(parseResult->GetParameterUInt("InterpolatorType", 1));
        orthoFilter->SetInterpolator(interpolator);
        }
      else if (typeInterpolator == "NEARESTNEIGHBOR" && nbInterpParams == 0)
        {
        NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
        orthoFilter->SetInterpolator(interpolator);
        }
      else if (typeInterpolator == "LINEAR" && nbInterpParams == 0)
        {
        LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
        orthoFilter->SetInterpolator(interpolator);
        }
      else
        {
        itkGenericExceptionMacro(<< "Interpolator type not recognized, choose one with (parameters) : BCO(0/1), NEARESTNEIGHBOR(0), LINEAR(0)");
        }
      }
    else
      {
      // Default is BCO
      BCOInterpolationType::Pointer interpolator = BCOInterpolationType::New();
      orthoFilter->SetInterpolator(interpolator);
      }

    //Update informations from orthorectification filter
    orthoFilter->GetOutput()->UpdateOutputInformation();

    //Instantiate the writer
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage());
    writer->SetInput(orthoFilter->GetOutput());

    unsigned int ram = 256;
    if (parseResult->IsOptionPresent("AvailableMemory"))
      {
      ram = parseResult->GetParameterUInt("AvailableMemory");
      }
    writer->SetAutomaticTiledStreaming(ram);

    otb::StandardWriterWatcher watcher(writer, orthoFilter,"Orthorectification");
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


int OrthoRectification::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Orthorectification");
  descriptor->SetDescription("Using available image metadata to determine the sensor model, computes a cartographic projection of the image");
  descriptor->AddInputImage();
  descriptor->AddOutputImage();
  descriptor->AddOption("UpperLeft","Cartographic X/Y coordinate of upper left corner ","ul", 2, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("OutputSize","Size of result image in X/Y","size", 2, false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOption("OutputSpacing","Spacing resolution in meters on X/Y Axis","spacing", 2, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("DEMDirectory","Directory where to find the DEM tiles","dem", 1, false, otb::ApplicationDescriptor::DirectoryName);
  descriptor->AddOptionNParams("MapProjectionType",
                               "Type (UTM/LAMBERT/LAMBERT2/LAMBERT93/SINUS/ECKERT4/TRANSMERCATOR/MOLLWEID) and parameters of map projection used",
                               "mapProj", false, otb::ApplicationDescriptor::String);
  descriptor->AddOption("RPC","Activate RPC sensor model estimation. Parameter is the number of control points per axis.","rpc", 1, false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOption("LocMapSpacing","Generate a coarser deformation field with the given spacing.","lmSpacing", 1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOptionNParams("InterpolatorType",
                               "Type LINEAR/BCO/NEARESTNEIGHBOR (optional, BCO by default)","interp", false, otb::ApplicationDescriptor::String);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);

  return EXIT_SUCCESS;
}

int OrthoRectification::Execute(otb::ApplicationOptionsResult* parseResult)
{
  if ( parseResult->IsOptionPresent("MapProjectionType"))
    {
    std::string typeMap = parseResult->GetParameterString("MapProjectionType", 0);
    int nbParams = parseResult->GetNumberOfParameters("MapProjectionType");
    nbParams--;

    if ((typeMap == "UTM")&&(nbParams==2))
      {
      int numZone = parseResult->GetParameterUInt("MapProjectionType", 1);
      char hemisphere = parseResult->GetParameterChar("MapProjectionType", 2);

      typedef otb::UtmInverseProjection UtmProjectionType;
      UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();

      utmProjection->SetZone(numZone);
      utmProjection->SetHemisphere(hemisphere);

      return generic_main<UtmProjectionType>(parseResult, utmProjection );
      }
    else
      {
      std::vector<double> parameters;

      for (int i=1; i<nbParams+1; i++)
        {
        parameters.push_back(parseResult->GetParameterDouble("MapProjectionType", i));
        }

      if ((typeMap == "LAMBERT")&&(nbParams==4))
        {
        typedef otb::LambertConformalConicInverseProjection LambertProjectionType;
        LambertProjectionType::Pointer lambertProjection = LambertProjectionType::New();

        lambertProjection->SetParameters(parameters[0], parameters[1], parameters[2], parameters[3]);

        return generic_main<LambertProjectionType>(parseResult, lambertProjection);
        }
      else if ((typeMap == "LAMBERT2")&&(nbParams==0))
        {
        typedef otb::Lambert2EtenduInverseProjection Lambert2ProjectionType;
        Lambert2ProjectionType::Pointer lambert2Projection = Lambert2ProjectionType::New();

        return generic_main<Lambert2ProjectionType>(parseResult, lambert2Projection);
        }
      else if ((typeMap == "LAMBERT93")&&(nbParams==0))
        {
        typedef otb::Lambert93InverseProjection Lambert93ProjectionType;
        Lambert93ProjectionType::Pointer lambert93Projection = Lambert93ProjectionType::New();

        return generic_main<Lambert93ProjectionType>(parseResult, lambert93Projection);
        }
      else if ((typeMap == "SINUS")&&(nbParams==2))
        {
        typedef otb::SinusoidalInverseProjection SinusoidalProjectionType;
        SinusoidalProjectionType::Pointer sinusoidalProjection = SinusoidalProjectionType::New();

        sinusoidalProjection->SetParameters(parameters[0], parameters[1]);

        return generic_main<SinusoidalProjectionType>(parseResult, sinusoidalProjection);
        }
      else if ((typeMap == "ECKERT4")&&(nbParams==2))
        {
        typedef otb::Eckert4InverseProjection Eckert4ProjectionType;
        Eckert4ProjectionType::Pointer eckert4Projection = Eckert4ProjectionType::New();

        eckert4Projection->SetParameters(parameters[0], parameters[1]);

        return generic_main<Eckert4ProjectionType>(parseResult, eckert4Projection);
        }
      else if ((typeMap == "TRANSMERCATOR")&&(nbParams==3))
        {
        typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
        TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();

        transMercatorProjection->SetParameters(parameters[0], parameters[1], parameters[2]);

        return generic_main<TransMercatorProjectionType>(parseResult, transMercatorProjection);
        }
      else if ((typeMap == "MOLLWEID")&&(nbParams==2))
        {
        typedef otb::MollweidInverseProjection MollweidProjectionType;
        MollweidProjectionType::Pointer mollweidProjection = MollweidProjectionType::New();

        mollweidProjection->SetParameters(parameters[0], parameters[1]);

        return generic_main<MollweidProjectionType>(parseResult, mollweidProjection);
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
    typedef otb::VectorImage<unsigned short int, 2>       ImageType;
    typedef otb::ImageFileReader<ImageType>           ReaderType;

    typedef otb::UtmInverseProjection UtmProjectionType;
    typedef otb::GenericRSResampleImageFilter< ImageType, ImageType>    ResamplerImageType;
    typedef ResamplerImageType::GenericRSTransformType                   TransformType;

    // Read input image information
    ReaderType::Pointer reader=ReaderType::New();
    reader->SetFileName(parseResult->GetInputImage().c_str());
    reader->GenerateOutputInformation();

    ImageType::Pointer input = reader->GetOutput();
    TransformType::Pointer transform=TransformType::New();

    transform->SetOutputDictionary(input->GetMetaDataDictionary());
    transform->SetOutputProjectionRef(input->GetProjectionRef());
    transform->SetOutputKeywordList(input->GetImageKeywordlist());
    //TODO is DEM informations mandatory here?
    //transform->SetDEMDirectory();

    // Needed variable
    std::string projectionRef;
    // The inverse transform is need here
    TransformType::Pointer invTransform = TransformType::New();
    transform->GetInverse(invTransform);

    // Build the UTM transform : Need the zone & the hemisphere
    // For this we us the geographic coordinate of the input UL corner
    typedef itk::Point<double, 2>                  GeoPointType;

    // get the utm zone and hemisphere using the input UL corner
    // geographic coordinates
    ImageType::PointType  pSrc;
    ImageType::IndexType      index;
    GeoPointType   geoPoint;
    index[0] = input->GetLargestPossibleRegion().GetIndex()[0];
    index[1] = input->GetLargestPossibleRegion().GetIndex()[1];
    input->TransformIndexToPhysicalPoint(index, pSrc);

    // The first transform of the inverse transform : input -> WGS84
    geoPoint = invTransform->GetTransform()->GetFirstTransform()->TransformPoint(pSrc);

    // Guess the zone and the hemisphere
    int zone = otb::Utils::GetZoneFromGeoPoint(geoPoint[0], geoPoint[1]);
    bool hem = (geoPoint[1]>1e-10)?true:false;

    typedef otb::UtmInverseProjection UtmProjectionType;
    UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();

    utmProjection->SetZone(zone);
    utmProjection->SetHemisphere(hem);

    otbMsgDevMacro(<< "Guess the UTM parameters, zone: " << zone << " hemisphere: " << hem);

    return generic_main<UtmProjectionType>(parseResult, utmProjection);
    }
}
} // namespace otb
