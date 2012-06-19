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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbGeoInformationConversion.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRDataSourceToLabelImageFilter.h"
// #include "otbGenericRSTransform.h"

namespace otb
{
namespace Wrapper
{

class Rasterization : public Application
{
public:
  /** Standard class typedefs. */
  typedef Rasterization                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Rasterization, otb::Application);

  /** Filters typedef */
  // the application produces a binary mask : no need to use a FloatVectorImageType
  typedef UInt8ImageType::PointType          PointType;
  typedef UInt8ImageType::SizeType           SizeType;
  typedef UInt8ImageType::SpacingType        SpacingType;
  typedef UInt8ImageType::IndexType          IndexType;
  
  // Misc
  // typedef otb::GenericRSTransform<>          RSTransformType;
  typedef otb::PipelineMemoryPrintCalculator MemoryCalculatorType;

  // Exact rasterization mode
  typedef otb::OGRDataSourceToLabelImageFilter<FloatImageType> OGRDataSourceToMapFilterType;

private:
  void DoInit()
    {
    SetName("Rasterization");
    SetDescription("Reproject and Rasterize a Vector Data.");

    SetDocName("Rasterization");
    SetDocLongDescription("Reproject and Rasterize a Vector Data according to a support image if any, or following the output image propreties set by the user. The application generates an output image in a binary style");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
   
    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputFilename,  "in",   "InputVectorData");
    SetParameterDescription( "in", "The input vector data to be rasterized" );
    
    AddParameter(ParameterType_OutputImage,  "out",   "OutputImage");
    SetParameterDescription( "out", "An output image containing the rasterized vector data" );
    
    AddParameter(ParameterType_InputImage,  "im",   "InputImage");
    SetParameterDescription( "im", "A reference image for extraction region and projection information (optional)" );
    MandatoryOff("im");
    
    AddParameter(ParameterType_Float,  "szx",   "SizeX");
    SetParameterDescription( "szx", "OutputSize[0] (useless if support image is given)" );
    //MandatoryOff("szx");
    
    AddParameter(ParameterType_Float,  "szy",   "SizeY");
    SetParameterDescription( "szy", "OutputSize[1] (useless if support image is given)" );
    //MandatoryOff("szy");
    
    AddParameter(ParameterType_Int,  "epsg",   "RSID");
    SetParameterDescription( "epsg", "Projection System RSID number (RSID 4326 for WGS84 32631 for UTM31N)  (useless if support image is given)" );
    MandatoryOff("epsg");
    
    AddParameter(ParameterType_Float,  "orx",   "OriginX");
    SetParameterDescription( "orx", "OutputOrigin[0] (useless if support image is given)" );
    //MandatoryOff("orx");
    
    AddParameter(ParameterType_Float,  "ory",   "OriginY");
    SetParameterDescription( "ory", "OutputOrigin[1] (useless if support image is given)" );
    //MandatoryOff("ory");
    
    AddParameter(ParameterType_Float,  "spx",   "SpacingX");
    SetParameterDescription( "spx", "OutputSpacing[0] (useless if support image is given)" );
    MandatoryOff("spx");
    
    AddParameter(ParameterType_Float,  "spy",   "SpacingY");
    SetParameterDescription( "spy", "OutputSpacing[1] (useless if support image is given)" );
    MandatoryOff("spy");
    
    AddParameter(ParameterType_Choice,"mode","Rasterization mode");
    SetParameterDescription("mode","This parameter allows to choose between rasterization modes");
    
    AddParameter(ParameterType_String,"field","The attribute field to burn");
    SetParameterDescription("field","Name of the attribute field to burn");
    SetParameterString("field","DN");

    AddRAMParameter();
    
    SetDocExampleParameterValue("in","qb_RoadExtract_classification.shp");
    SetDocExampleParameterValue("out", "rasterImage.tif");
    SetDocExampleParameterValue("szx","1100");
    SetDocExampleParameterValue("szy","1100");
    
    }

  void DoUpdateParameters()
    {
    // Nothing to do
    }
  
  
  void DoExecute()
    {    
    UInt8ImageType::Pointer referenceImage;

    m_OgrDS = otb::ogr::DataSource::New(GetParameterString("inputVData"), otb::ogr::DataSource::Modes::read);

    // region information
    SizeType size;
    PointType origin;
    SpacingType spacing;
  
    // reading projection information
    // two choice :
    std::string outputProjectionRef;
    // a reference image is given as input
    if (HasValue("im"))
      {
      if (HasValue("szx") || HasValue("szy") || HasValue("orx") || HasValue("ory")
          || HasValue("spx") || HasValue("spy") || HasValue("epsg"))
        {
        otbAppLogWARNING("A reference image has been given, other parameters "
                         "regarding the output image will be ignored");
        }
      
      referenceImage = GetParameterUInt8Image("im");
      outputProjectionRef = referenceImage->GetProjectionRef();
  
      size[0] = referenceImage->GetLargestPossibleRegion().GetSize(0);
      size[1] = referenceImage->GetLargestPossibleRegion().GetSize(1);
  
      origin = referenceImage->GetOrigin();
  
      spacing = referenceImage->GetSpacing();
      }
    else if (HasValue("szx") && HasValue("szy"))
      {
      if (HasValue("epsg"))
        {
        unsigned int RSID = GetParameterInt("epsg");
        outputProjectionRef = otb::GeoInformationConversion::ToWKT(RSID);
        }
      else
        {
        outputProjectionRef = m_OgrDS->GetProjectionRef();
        }

      size[0] = GetParameterFloat("szx");
      size[1] = GetParameterFloat("szy");

      if ( HasValue("orx") && HasValue("ory"))
        {
        origin[0] = GetParameterFloat("orx");
        origin[1] = GetParameterFloat("ory");
        }
      else
        {
        // Not handled for now, parameter is mandatory
        }

      if (HasValue("spx") && HasValue("spy"))
        {
        spacing[0] = GetParameterFloat("spx");
        spacing[1] = GetParameterFloat("spy");
        }
      else
        {
        // Not handled for now, parameter is mandatory
        }
      }
    else
      {
      otbAppLogFATAL("Input entry problem, if no reference image given, region size is needed ");
      }
  
      m_OGRDataSourceRendering = OGRDataSourceToMapFilterType::New();
      m_OGRDataSourceRendering->AddOGRDataSource(m_OgrDS);
      m_OGRDataSourceRendering->SetBurnAttribute(GetParameterString("field"));
      m_OGRDataSourceRendering->SetOutputSize(size);
      m_OGRDataSourceRendering->SetOutputOrigin(origin);
      m_OGRDataSourceRendering->SetOutputSpacing(spacing);
      m_OGRDataSourceRendering->SetOutputProjectionRef(outputProjectionRef);
      
      SetParameterOutputImage<FloatImageType>("out", m_OGRDataSourceRendering->GetOutput());
    
    }
  
  otb::ogr::DataSource::Pointer m_OgrDS;
  OGRDataSourceToMapFilterType::Pointer m_OGRDataSourceRendering;
  
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rasterization)
