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

#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataProperties.h"
#include "otbVectorDataToImageFilter.h"
#include "otbGeoInformationConversion.h"
#include "otbRemoteSensingRegion.h"


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
  typedef UInt8ImageType::PointType   PointType;
  typedef UInt8ImageType::SizeType    SizeType;
  typedef UInt8ImageType::SpacingType SpacingType;
  typedef UInt8ImageType::IndexType   IndexType;
  
  typedef VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType> VectorDataExtractROIType;
  typedef VectorDataProperties<VectorDataType> VectorDataPropertiesType;

  // Rasterization
  typedef otb::VectorDataToImageFilter<VectorDataType, UInt8ImageType> VectorDataToImageFilterType;

  // Misc
  typedef otb::RemoteSensingRegion<double> RemoteSensingRegionType;
  typedef RemoteSensingRegionType::SizeType SizePhyType;
  typedef otb::PipelineMemoryPrintCalculator MemoryCalculatorType;

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

    AddParameter(ParameterType_InputVectorData,  "in",   "InputVectorData");
    SetParameterDescription( "in", "The input vector data to be rasterized" );
    
    AddParameter(ParameterType_OutputImage,  "out",   "OutputImage");
    SetParameterDescription( "out", "An output image containing the rasterized vector data" );
    
    AddParameter(ParameterType_InputImage,  "im",   "InputImage");
    SetParameterDescription( "im", "A reference image for extraction region and projection information (optional)" );
    MandatoryOff("im");
    
    AddParameter(ParameterType_Float,  "szx",   "SizeX");
    SetParameterDescription( "szx", "OutputSize[0] (useless if support image is given)" );
    MandatoryOff("szx");
    
    AddParameter(ParameterType_Float,  "szy",   "SizeY");
    SetParameterDescription( "szy", "OutputSize[1] (useless if support image is given)" );
    MandatoryOff("szy");
    
    AddParameter(ParameterType_Int,  "epsg",   "RSID");
    SetParameterDescription( "epsg", "Projection System RSID number (RSID 4326 for WGS84 32631 for UTM31N)  (useless if support image is given)" );
    MandatoryOff("epsg");
    
    AddParameter(ParameterType_Float,  "orx",   "OriginX");
    SetParameterDescription( "orx", "OutputOrigin[0] (useless if support image is given)" );
    MandatoryOff("orx");
    
    AddParameter(ParameterType_Float,  "ory",   "OriginY");
    SetParameterDescription( "ory", "OutputOrigin[1] (useless if support image is given)" );
    MandatoryOff("ory");
    
    AddParameter(ParameterType_Float,  "spx",   "SpacingX");
    SetParameterDescription( "spx", "OutputSpacing[0] (useless if support image is given)" );
    MandatoryOff("spx");
    
    AddParameter(ParameterType_Float,  "spy",   "SpacingY");
    SetParameterDescription( "spy", "OutputSpacing[1] (useless if support image is given)" );
    MandatoryOff("spy");
    
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");
    
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
    VectorDataType::Pointer inputVData = GetParameterVectorData("in");
    inputVData->Update();
    
    UInt8ImageType::Pointer referenceImage;
    
    // region information
    SizeType size;
    PointType origin;
    SpacingType spacing;
    SizePhyType sizePhy;
  
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
  
      sizePhy[0] = size[0] * spacing[0];
      sizePhy[1] = size[1] * spacing[1];
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
        outputProjectionRef = inputVData->GetProjectionRef();
        }

      m_VdProperties = VectorDataPropertiesType::New();
      m_VdProperties->SetVectorDataObject(inputVData);
      m_VdProperties->ComputeBoundingRegion();

      size[0] = GetParameterFloat("szx");
      size[1] = GetParameterFloat("szy");

      if ( HasValue("orx") && HasValue("ory"))
        {
        origin[0] = GetParameterFloat("orx");
        origin[1] = GetParameterFloat("ory");
        }
      else
        {
        origin = m_VdProperties->GetBoundingRegion().GetIndex();
        }

      if (HasValue("spx") && HasValue("spy"))
        {
        spacing[0] = GetParameterFloat("spx");
        spacing[1] = GetParameterFloat("spy");
        }
      else
        {
        spacing[0] = m_VdProperties->GetBoundingRegion().GetSize()[0] / size[0];
        spacing[1] = m_VdProperties->GetBoundingRegion().GetSize()[1] / size[1];
        }

      sizePhy[0] = size[0] * spacing[0];
      sizePhy[1] = size[1] * spacing[1];

      }
    else
      {
      otbAppLogFATAL("Input entry problem, if no reference image given, region size is needed ");
      }
  
    // Reprojecting the VectorData
    m_Vproj = VectorDataProjectionFilterType::New();
    m_Vproj->SetInput(inputVData);
    m_Vproj->SetInputProjectionRef(inputVData->GetProjectionRef());
    m_Vproj->SetOutputProjectionRef(outputProjectionRef);
    if (HasValue("im"))
      {
      m_Vproj->SetOutputKeywordList(referenceImage->GetImageKeywordlist());
      }
  
    RemoteSensingRegionType region;
    region.SetSize(sizePhy);
    region.SetOrigin(origin);
    region.SetRegionProjection(outputProjectionRef);
    if (HasValue("im"))
      {
      region.SetKeywordList(referenceImage->GetImageKeywordlist());
      }
    
    m_Vdextract = VectorDataExtractROIType::New();
    m_Vdextract->SetRegion(region);
    m_Vdextract->SetInput(m_Vproj->GetOutput());
    
    m_VectorDataRendering = VectorDataToImageFilterType::New();
    m_VectorDataRendering->SetInput(m_Vdextract->GetOutput());
    m_VectorDataRendering->SetSize(size);
    m_VectorDataRendering->SetOrigin(origin);
    m_VectorDataRendering->SetSpacing(spacing);
    m_VectorDataRendering->SetVectorDataProjectionWKT(outputProjectionRef);
    m_VectorDataRendering->SetRenderingStyleType(VectorDataToImageFilterType::Binary);
  
    UInt8ImageType::Pointer outputImage = m_VectorDataRendering->GetOutput();
    if (HasValue("im"))
      {
      outputImage->SetMetaDataDictionary(referenceImage->GetMetaDataDictionary());
      }
    
    AddProcess(m_VectorDataRendering,"Rasterization");
    
    SetParameterOutputImage<UInt8ImageType>("out", outputImage);
    
    }
  
  VectorDataPropertiesType::Pointer m_VdProperties;
  VectorDataProjectionFilterType::Pointer m_Vproj;
  VectorDataExtractROIType::Pointer m_Vdextract;
  VectorDataToImageFilterType::Pointer m_VectorDataRendering;
  
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rasterization)
