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
#include "otbGenericRSTransform.h"

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
  typedef otb::GenericRSTransform<>          RSTransformType;
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
    
    AddParameter(ParameterType_Int,  "szx",   "SizeX");
    SetParameterDescription( "szx", "OutputSize[0] (useless if support image is given)" );
    MandatoryOff("szx");
    SetMinimumParameterIntValue("szx",1);
    
    AddParameter(ParameterType_Int,  "szy",   "SizeY");
    SetParameterDescription( "szy", "OutputSize[1] (useless if support image is given)" );
    MandatoryOff("szy");
    SetMinimumParameterIntValue("szy",1);
    
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
        
    AddParameter(ParameterType_Float,"background", "Background value");
    SetParameterDescription("background","Default value for pixels which do not belong to any geometry");
    SetDefaultParameterFloat("background",0.);

    AddParameter(ParameterType_Choice,"mode","Rasterization mode");
    SetParameterDescription("mode","This parameter allows to choose between rasterization modes");
    
    AddChoice("mode.binary","Binary mode");
    SetParameterDescription("mode.binary","In this mode, pixels within a geometry will hold the user-defined foreground value");

    AddParameter(ParameterType_Float,"mode.binary.foreground","Foreground value");
    SetParameterDescription("mode.binary.foreground","Value of the pixels inside a geometry");
    SetDefaultParameterFloat("mode.binary.foreground",255);
    
    AddChoice("mode.attribute","Attribute burning mode");
    SetParameterDescription("mode.attribute","In this mode, pixels within a geometry will hold the value of a user-defined field extracted from this geometry.");

    AddParameter(ParameterType_String,"mode.attribute.field","The attribute field to burn");
    SetParameterDescription("mode.attribute.field","Name of the attribute field to burn");
    SetParameterString("mode.attribute.field","DN");
    
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

    m_OgrDS = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::read);

    bool validInputProjRef = true;

    otb::ogr::DataSource::const_iterator lit = m_OgrDS->begin();

    if(lit==m_OgrDS->end())
      {
      otbAppLogFATAL("There are no layers in the input dataset.");
      }
    
    std::string inputProjectionRef = lit->GetProjectionRef();

    // Check if we have layers with different projection ref
    for(;lit != m_OgrDS->end();++lit)
      {
      if(lit->GetProjectionRef() != inputProjectionRef)
        {
        otbAppLogWARNING("The input dataset have layers with different projection reference system. The application will base all extent computation on the srs of the first layer only.");
        }
      }

    if(inputProjectionRef == "")
      {
      otbAppLogWARNING("Failed to find a valid projection ref in dataset. The application will asume that the given reference image or origin, spacing and size are consistent with the dataset geometry. Output EPSG code will be ignored.");
      validInputProjRef = false;
      }
    else
      {
      otbAppLogINFO("Input dataset projection reference system is: "<<inputProjectionRef);
      }

    // Retrieve extent
    double ulx, uly, lrx, lry;
    bool extentAvailable = true;

    try
      {
      m_OgrDS->GetGlobalExtent(ulx,uly,lrx,lry);
      }
    catch(itk::ExceptionObject & err)
      {
      extentAvailable = false;
      }

    if(!extentAvailable && 
       (!(HasValue("spx") && HasValue("spy")) 
        || (!(HasValue("orx") && HasValue("ory")))))
      {
      otbAppLogWARNING(<<"Failed to retrieve the spatial extent of the dataset. The application will retry in force mode,  which means it might have to walk the entire dataset to determine extent. This might be a long process for large datasets. Consider setting the orx, ory, spx and spy parameters.");

      try
        {
        m_OgrDS->GetGlobalExtent(ulx,uly,lrx,lry);
        extentAvailable = true;
        }
      catch(itk::ExceptionObject & err)
        {
        extentAvailable = false;

        otbAppLogFATAL(<<"Failed to retrieve the sapatial extent of the dataset in force mode. The spatial extent is mandatory when orx, ory, spx and spy parameters are not set, consider setting them.");
        }
      }

    if(extentAvailable)
      {
      otbAppLogINFO("Input dataset extent is ("<<ulx<<", "<<uly<<") ("<<lrx<<", "<<lry<<")");
      }


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
  
      size = referenceImage->GetLargestPossibleRegion().GetSize();
  
      origin = referenceImage->GetOrigin();
  
      spacing = referenceImage->GetSpacing();
      }
    else if (HasValue("spx") && HasValue("spy"))
      {
      if (HasValue("epsg"))
        {
        unsigned int RSID = GetParameterInt("epsg");
        outputProjectionRef = otb::GeoInformationConversion::ToWKT(RSID);
        }
      else
        {
        outputProjectionRef = inputProjectionRef;
        }

      spacing[0] = GetParameterFloat("spx");
      spacing[1] = GetParameterFloat("spy");

      if ( HasValue("orx") && HasValue("ory"))
        {
        origin[0] = GetParameterFloat("orx");
        origin[1] = GetParameterFloat("ory");
        }
      else if(extentAvailable)
        {
        origin[0] = ulx;
        origin[1] = uly;
        
        // Transform to output EPSG
       
        if(validInputProjRef)
          {
          RSTransformType::Pointer rsTransform = RSTransformType::New();
          rsTransform->SetInputProjectionRef(inputProjectionRef);
          rsTransform->SetOutputProjectionRef(outputProjectionRef);
          rsTransform->InstanciateTransform();

          origin = rsTransform->TransformPoint(origin);
          }
        }
      else
        {
        otbAppLogFATAL(<<"The orx and ory parameters are not set and the dataset extent could not be retrieved. The application can not deterimine the origin of the output raster");
        }

      if (HasValue("szx") && HasValue("szy"))
        {
        size[0] = GetParameterInt("szx");
        size[1] = GetParameterInt("szy");
        }
      else if(extentAvailable)
        {
        // Transform to output EPSG
        PointType lrout;
        lrout[0] = lrx;
        lrout[1] = lry;

        if(validInputProjRef)
          {
          RSTransformType::Pointer rsTransform = RSTransformType::New();
          rsTransform->SetInputProjectionRef(inputProjectionRef);
          rsTransform->SetOutputProjectionRef(outputProjectionRef);
          rsTransform->InstanciateTransform();

          lrout = rsTransform->TransformPoint(lrout);
          }
        size[0]=static_cast<unsigned int>((lrout[0] - origin[0])/spacing[0]);
        size[1]=static_cast<unsigned int>((lrout[1] - origin[1])/spacing[1]);
        }
      else
        {
        otbAppLogFATAL(<<"The szx and szy parameters are not set and the dataset extent could not be retrieved. The application can not deterimine the size of the output raster");

        }
      }
    else
      {
      otbAppLogFATAL("No reference image was given, at least spx and spy parameters must be set.");
      }
  
      m_OGRDataSourceRendering = OGRDataSourceToMapFilterType::New();
      m_OGRDataSourceRendering->AddOGRDataSource(m_OgrDS);
      m_OGRDataSourceRendering->SetOutputSize(size);
      m_OGRDataSourceRendering->SetOutputOrigin(origin);
      m_OGRDataSourceRendering->SetOutputSpacing(spacing);
      m_OGRDataSourceRendering->SetBackgroundValue(GetParameterFloat("background"));
      
      if(GetParameterString("mode") == "binary")
        {
        m_OGRDataSourceRendering->SetBurnAttributeMode(false);
        m_OGRDataSourceRendering->SetForegroundValue(GetParameterFloat("mode.binary.foreground"));
        }
      else if(GetParameterString("mode") == "attribute")
        {
        m_OGRDataSourceRendering->SetBurnAttributeMode(true);
        m_OGRDataSourceRendering->SetBurnAttribute(GetParameterString("mode.attribute.field"));
        }

      if(validInputProjRef)
        {
        m_OGRDataSourceRendering->SetOutputProjectionRef(outputProjectionRef);
        }
      
      otbAppLogINFO("Output projection reference system is: "<<outputProjectionRef);

      otbAppLogINFO("Output origin: "<<origin);
      otbAppLogINFO("Output size: "<<size);
      otbAppLogINFO("Output spacing: "<<spacing);

      SetParameterOutputImage<FloatImageType>("out", m_OGRDataSourceRendering->GetOutput());
    
    }
  
  otb::ogr::DataSource::Pointer m_OgrDS;
  OGRDataSourceToMapFilterType::Pointer m_OGRDataSourceRendering;
  
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rasterization)
