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
    SetDescription("Rasterize a vector dataset.");

    SetDocName("Rasterization");
    SetDocLongDescription("This application allows reprojecting and rasterize a vector dataset. The grid of the rasterized output can be set by using a reference image, or by setting all parmeters (origin, size, spacing) by hand. In the latter case, at least the spacing (ground sampling distance) is needed (other parameters are computed automatically). The rasterized output can also be in a different projection reference system than the input dataset.\n There are two rasterize mode available in the application. The first is the binary mode: it allows rendering all pixels belonging to a geometry of the input dataset in the foreground color, while rendering the other in background color. The second one allows rendering pixels belonging to a geometry woth respect to an attribute of this geometry. The field of the attribute to render can be set by the user. In the second mode, the background value is still used for unassociated pixels.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("For now, support of input dataset with multiple layers having different projection reference system is limited.");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputVectorData,  "in",   "Input vector dataset");
    SetParameterDescription( "in", "The input vector dataset to be rasterized" );

    AddParameter(ParameterType_OutputImage,  "out",   "Ouptut image");
    SetParameterDescription( "out", "An output image containing the rasterized vector dataset" );

    AddParameter(ParameterType_InputImage,  "im",   "Input reference image");
    SetParameterDescription( "im", "A reference image from which to import output grid and projection reference system information." );
    MandatoryOff("im");

    AddParameter(ParameterType_Int,  "szx",   "Output size x");
    SetParameterDescription( "szx", "Output size along x axis (useless if support image is given)" );
    MandatoryOff("szx");
    SetMinimumParameterIntValue("szx",1);

    AddParameter(ParameterType_Int,  "szy",   "Output size y");
    SetParameterDescription( "szy", "Output size along y axis (useless if support image is given)" );
    MandatoryOff("szy");
    SetMinimumParameterIntValue("szy",1);

    AddParameter(ParameterType_Int,  "epsg",   "Output EPSG code");
    SetParameterDescription( "epsg", "EPSG code for the output projection reference system (EPSG 4326 for WGS84, 32631 for UTM31N...,useless if support image is given)" );
    MandatoryOff("epsg");

    AddParameter(ParameterType_Float,  "orx",   "Output Upper-left x");
    SetParameterDescription( "orx", "Output upper-left corner x coordinate (useless if support image is given)" );
    MandatoryOff("orx");

    AddParameter(ParameterType_Float,  "ory",   "Output Upper-left y");
    SetParameterDescription( "ory", "Output upper-left corner y coordinate (useless if support image is given)" );
    MandatoryOff("ory");

    AddParameter(ParameterType_Float,  "spx",   "Spacing (GSD) x");
    SetParameterDescription( "spx", "Spacing (ground sampling distance) along x axis (useless if support image is given)" );
    MandatoryOff("spx");

    AddParameter(ParameterType_Float,  "spy",   "Spacing (GSD) y");
    SetParameterDescription( "spy", "Spacing (ground sampling distance) along y axis (useless if support image is given)" );
    MandatoryOff("spy");

    AddParameter(ParameterType_Float,"background", "Background value");
    SetParameterDescription("background","Default value for pixels not belonging to any geometry");
    SetDefaultParameterFloat("background",0.);

    AddParameter(ParameterType_Choice,"mode","Rasterization mode");
    SetParameterDescription("mode","Choice of rasterization modes");

    AddChoice("mode.binary","Binary mode");
    SetParameterDescription("mode.binary","In this mode, pixels within a geometry will hold the user-defined foreground value");

    AddParameter(ParameterType_Float,"mode.binary.foreground","Foreground value");
    SetParameterDescription("mode.binary.foreground","Value for pixels inside a geometry");
    SetDefaultParameterFloat("mode.binary.foreground",255);

    AddChoice("mode.attribute","Attribute burning mode");
    SetParameterDescription("mode.attribute","In this mode, pixels within a geometry will hold the value of a user-defined field extracted from this geometry.");

    AddParameter(ParameterType_String,"mode.attribute.field","The attribute field to burn");
    SetParameterDescription("mode.attribute.field","Name of the attribute field to burn");
    SetParameterString("mode.attribute.field","DN");

    AddRAMParameter();

    SetDocExampleParameterValue("in","qb_RoadExtract_classification.shp");
    SetDocExampleParameterValue("out", "rasterImage.tif");
    SetDocExampleParameterValue("spx","1.");
    SetDocExampleParameterValue("spy","1.");
    }

  void DoUpdateParameters()
    {
    // Nothing to do
    }


  void DoExecute()
    {
    otb::ogr::DataSource::Pointer ogrDS;
    UInt8ImageType::Pointer referenceImage;

    ogrDS = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Read);

    bool validInputProjRef = false;
    std::string inputProjectionRef = "";

    // Retrieve extent
    double ulx, uly, lrx, lry;
    bool extentAvailable = true;

    try
      {
      inputProjectionRef = ogrDS->GetGlobalExtent(ulx,uly,lrx,lry);
      }
    catch(const itk::ExceptionObject&)
      {
      extentAvailable = false;
      }

    if(!extentAvailable &&
       (!(HasValue("spx") && HasValue("spy"))
        || (!(HasValue("orx") && HasValue("ory")))))
      {
      otbAppLogWARNING(<<"Failed to retrieve the spatial extent of the dataset. The application will retry in force mode, which means it might have to walk the entire dataset to determine extent. This might be a long process for large datasets. Consider setting the orx, ory, spx and spy parameters.");

      try
        {
        inputProjectionRef = ogrDS->GetGlobalExtent(ulx,uly,lrx,lry,true);
        extentAvailable = true;
        }
      catch(itk::ExceptionObject & err)
        {
        extentAvailable = false;

        otbAppLogFATAL(<<"Failed to retrieve the spatial extent of the dataset in force mode. The spatial extent is mandatory when orx, ory, spx and spy parameters are not set, consider setting them. Error from library: "<<err.GetDescription());
        }
      }

    if(extentAvailable)
      {
      otbAppLogINFO("Input dataset extent is ("<<ulx<<", "<<uly<<") ("<<lrx<<", "<<lry<<")");
      }

    if(inputProjectionRef == "")
      {
      otbAppLogWARNING("Failed to find a valid projection ref in dataset. The application will assume that the given reference image or origin, spacing and size are consistent with the dataset geometry. Output EPSG code will be ignored.");
      validInputProjRef = false;
      }
    else
      {
      validInputProjRef = true;
      otbAppLogINFO("Input dataset projection reference system is: "<<inputProjectionRef);
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

      PointType corner;
      spacing[0] = GetParameterFloat("spx");
      spacing[1] = GetParameterFloat("spy");

      if ( HasValue("orx") && HasValue("ory"))
        {
        corner[0] = GetParameterFloat("orx");
        corner[1] = GetParameterFloat("ory");
        }
      else if(extentAvailable)
        {
        corner[0] = (spacing[0] > 0 ? ulx : lrx);
        corner[1] = (spacing[1] > 0 ? uly : lry);

        // Transform to output EPSG

        if(validInputProjRef)
          {
          RSTransformType::Pointer rsTransform = RSTransformType::New();
          rsTransform->SetInputProjectionRef(inputProjectionRef);
          rsTransform->SetOutputProjectionRef(outputProjectionRef);
          rsTransform->InstanciateTransform();

          corner = rsTransform->TransformPoint(corner);
          }
        }
      else
        {
        otbAppLogFATAL(<<"The orx and ory parameters are not set and the dataset extent could not be retrieved. The application can not determine the origin of the output raster");
        }

      origin[0] = corner[0] + 0.5*spacing[0];
      origin[1] = corner[1] + 0.5*spacing[1];

      if (HasValue("szx") && HasValue("szy"))
        {
        size[0] = GetParameterInt("szx");
        size[1] = GetParameterInt("szy");
        }
      else if(extentAvailable)
        {
        // Transform to output EPSG
        PointType lrout;
        lrout[0] = (spacing[0] > 0 ? lrx : ulx);
        lrout[1] = (spacing[1] > 0 ? lry : uly);

        if(validInputProjRef)
          {
          RSTransformType::Pointer rsTransform = RSTransformType::New();
          rsTransform->SetInputProjectionRef(inputProjectionRef);
          rsTransform->SetOutputProjectionRef(outputProjectionRef);
          rsTransform->InstanciateTransform();

          lrout = rsTransform->TransformPoint(lrout);
          }
        size[0]=static_cast<unsigned int>((lrout[0] - corner[0])/spacing[0]);
        size[1]=static_cast<unsigned int>((lrout[1] - corner[1])/spacing[1]);
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
      m_OGRDataSourceRendering->AddOGRDataSource(ogrDS);
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

  OGRDataSourceToMapFilterType::Pointer m_OGRDataSourceRendering;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rasterization)
