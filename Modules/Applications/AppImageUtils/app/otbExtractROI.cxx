/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbMultiChannelExtractROI.h"
#include "otbStandardFilterWatcher.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperTypes.h"
#include "otbOGRDataSourceToLabelImageFilter.h"
#include <algorithm>

#include "otbWrapperElevationParametersHandler.h"

#include "otbGenericRSTransform.h"

namespace otb
{
namespace Wrapper
{

class ExtractROI : public Application
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::GenericRSTransform<>       RSTransformType;
  typedef RSTransformType::InputPointType Point3DType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ExtractROI, otb::Application);

  /** Filters typedef */
  typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType> ExtractROIFilterType;

  typedef ExtractROIFilterType::InputImageType ImageType;

protected:
  ExtractROI() : m_IsExtentInverted(false)
  {
  }

private:
  void DoInit() override
  {
    SetName("ExtractROI");
    SetDescription("Extract a ROI defined by the user.");

    // Documentation
    SetDocLongDescription(
        "This application extracts a Region Of Interest with "
        "user parameters. There are four mode of extraction. The standard mode "
        "allows the user to enter one point (upper left corner of the region to "
        "extract) and a size. The extent mode needs two points (upper left "
        "corner and lower right) and the radius mode need the center of the "
        "region and the radius: it will extract the rectangle containing the "
        "circle defined and limited by the image dimension. The fit mode "
        "needs a reference image or vector and the dimension of the extracted "
        "region will be the same as the extent of the reference. "
        "Different units are available such as pixel, image physical space "
        "or longitude and latitude.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    // Set parameter input
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Image to be processed.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Region of interest from the input image");


    AddParameter(ParameterType_Choice, "mode", "Extraction mode");
    AddChoice("mode.standard", "Standard");
    SetParameterDescription("mode.standard",
                            "In standard mode extraction is done with 2 parameters: the "
                            "upper left corner and the size of the region, decomposed "
                            "in X and Y coordinates.");

    // Fit mode: the ROI is computed through a ref vector dataset or a ref image
    AddChoice("mode.fit", "Fit");
    SetParameterDescription("mode.fit", "In fit mode, extract is made from a reference: image or vector dataset.");

    AddParameter(ParameterType_InputImage, "mode.fit.im", "Reference image");
    SetParameterDescription("mode.fit.im", "Reference image to define the ROI");
    AddParameter(ParameterType_InputVectorData, "mode.fit.vect", "Reference vector");
    SetParameterDescription("mode.fit.vect",
                            "The extent of the input vector file is computed and then "
                            "gives a region of interest that will be extracted.");

    // Extent mode: ROI is computed with two point (upper left and lower left corners)
    AddChoice("mode.extent", "Extent");
    SetParameterDescription("mode.extent",
                            "In extent mode, the ROI is defined by two points, the upper left corner "
                            "and the lower right corner, decomposed in 2 coordinates: "
                            "X and Y. The unit for those coordinates can be set.");

    AddParameter(ParameterType_Float, "mode.extent.ulx", "X coordinate of the Upper left corner");
    SetParameterDescription("mode.extent.ulx", "X coordinate of upper left corner point.");

    AddParameter(ParameterType_Float, "mode.extent.uly", "Y coordinate of Upper Left corner point");
    SetParameterDescription("mode.extent.uly", "Y coordinate of upper left corner point.");

    AddParameter(ParameterType_Float, "mode.extent.lrx", "X coordinate of Lower Right corner point");
    SetParameterDescription("mode.extent.lrx", "X coordinate of lower right corner point.");

    AddParameter(ParameterType_Float, "mode.extent.lry", "Y coordinate of Lower Right corner point");
    SetParameterDescription("mode.extent.lry", "Y coordinate of lower right corner point.");

    // Unit of extent mode
    AddParameter(ParameterType_Choice, "mode.extent.unit", "Unit");
    AddChoice("mode.extent.unit.pxl", "Pixel");
    SetParameterDescription("mode.extent.unit.pxl",
                            "The unit for the parameters coordinates will be the pixel, meaning the "
                            "index of the two points.");
    AddChoice("mode.extent.unit.phy", "Image physical space");
    SetParameterDescription("mode.extent.unit.phy",
                            "The unit for the parameters coordinates will be the physical "
                            "measure of the image.");
    AddChoice("mode.extent.unit.lonlat", "Longitude and latitude");
    SetParameterDescription("mode.extent.unit.lonlat",
                            "The unit for the parameters coordinates will be the longitude and "
                            "the latitude.");


    // Radius mode: ROI is computed through a point and a radius
    AddChoice("mode.radius", "Radius");
    SetParameterDescription("mode.radius",
                            "In radius mode, the ROI is defined by a point and a radius. The "
                            "unit of those two parameters can be chosen independently.");

    AddParameter(ParameterType_Float, "mode.radius.r", "Radius");
    SetParameterDescription("mode.radius.r", "This is the radius parameter of the radius mode.");
    AddParameter(ParameterType_Choice, "mode.radius.unitr", "Radius unit");
    AddChoice("mode.radius.unitr.pxl", "Pixel");
    SetParameterDescription("mode.radius.unitr.pxl", "The unit of the radius will be the pixel.");
    AddChoice("mode.radius.unitr.phy", "Image physical space");
    SetParameterDescription("mode.radius.unitr.phy", "The unit of the radius will be the physical measure of the image.");

    AddParameter(ParameterType_Float, "mode.radius.cx", "X coordinate of the center");
    SetParameterDescription("mode.radius.cx",
                            "This is the center coordinate of the radius mode, it will be either an "
                            "abscissa or a longitude.");
    AddParameter(ParameterType_Float, "mode.radius.cy", "Y coordinate of the center");
    SetParameterDescription("mode.radius.cx",
                            "This is the center coordinate of the radius mode, it will be either an "
                            "ordinate or a latitude.");
    AddParameter(ParameterType_Choice, "mode.radius.unitc", "Center unit");
    AddChoice("mode.radius.unitc.pxl", "Pixel");
    SetParameterDescription("mode.radius.unitc.pxl", "The unit for the center coordinates will be the pixel");
    AddChoice("mode.radius.unitc.phy", "Image physical space");
    SetParameterDescription("mode.radius.unitc.phy",
                            "The unit for the center coordinates will be the physical "
                            "measure of the image.");
    AddChoice("mode.radius.unitc.lonlat", "Lon/Lat");
    SetParameterDescription("mode.radius.unitc.lonlat",
                            "The unit for the center coordinates will be the longitude and "
                            "the latitude.");


    // Standard parameter
    AddParameter(ParameterType_Int, "startx", "Start X");
    SetParameterDescription("startx", "ROI start x position.");
    AddParameter(ParameterType_Int, "starty", "Start Y");
    SetParameterDescription("starty", "ROI start y position.");
    AddParameter(ParameterType_Int, "sizex", "Size X");
    SetParameterDescription("sizex", "size along x in pixels.");
    AddParameter(ParameterType_Int, "sizey", "Size Y");
    SetParameterDescription("sizey", "size along y in pixels.");

    // Channelist Parameters
    AddParameter(ParameterType_Band, "cl", "Output Image channels");
    SetParameterDescription("cl", "Channels to write in the output image.");
    SetRasterData("cl", "in");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");
    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("startx", 0);
    SetDefaultParameterInt("starty", 0);
    SetDefaultParameterInt("sizex", 0);
    SetDefaultParameterInt("sizey", 0);

    SetDefaultParameterInt("mode.extent.ulx", 0);
    SetDefaultParameterInt("mode.extent.uly", 0);
    SetDefaultParameterInt("mode.extent.lry", 0);
    SetDefaultParameterInt("mode.extent.lrx", 0);

    SetDefaultParameterInt("mode.radius.r", 0);
    SetDefaultParameterInt("mode.radius.cx", 0);
    SetDefaultParameterInt("mode.radius.cy", 0);


    // Doc example parameter settings
    SetDocExampleParameterValue("in", "VegetationIndex.hd");
    SetDocExampleParameterValue("mode", "extent");
    SetDocExampleParameterValue("mode.extent.ulx", "40");
    SetDocExampleParameterValue("mode.extent.uly", "40");
    SetDocExampleParameterValue("mode.extent.lrx", "150");
    SetDocExampleParameterValue("mode.extent.lry", "150");
    SetDocExampleParameterValue("out", "ExtractROI.tif");
    SetMinimumParameterIntValue("sizex", 0);
    SetMinimumParameterIntValue("sizey", 0);
    SetMinimumParameterIntValue("startx", 0);
    SetMinimumParameterIntValue("starty", 0);
    SetMinimumParameterFloatValue("mode.radius.r", 0);

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if (HasValue("in"))
    {
      ImageType* inImage = GetParameterImage("in");
      inImage->UpdateOutputInformation();
      ImageType::RegionType largestRegion = inImage->GetLargestPossibleRegion();

      ImageType::RegionType currentLargest;
      currentLargest.SetSize(0, GetDefaultParameterInt("sizex"));
      currentLargest.SetSize(1, GetDefaultParameterInt("sizey"));
      currentLargest.SetIndex(1, GetDefaultParameterInt("starty"));
      currentLargest.SetIndex(0, GetDefaultParameterInt("startx"));
      // Update default only if largest has changed
      if (currentLargest != largestRegion)
      {
        // Setting maximum value
        SetMaximumParameterIntValue("sizex", largestRegion.GetSize(0));
        SetMaximumParameterIntValue("sizey", largestRegion.GetSize(1));
        SetMaximumParameterIntValue("startx", largestRegion.GetIndex(0) + largestRegion.GetSize(0));
        SetMaximumParameterIntValue("starty", largestRegion.GetIndex(1) + largestRegion.GetSize(1));
        // Setting default value
        SetDefaultParameterInt("sizex", largestRegion.GetSize(0));
        SetDefaultParameterInt("sizey", largestRegion.GetSize(1));
        SetDefaultParameterInt("startx", largestRegion.GetIndex(0));
        SetDefaultParameterInt("starty", largestRegion.GetIndex(1));

        // Setting actual value
        if (!HasUserValue("sizex"))
        {
          SetParameterInt("sizex", GetDefaultParameterInt("sizex"));
        }
        if (!HasUserValue("sizey"))
        {
          SetParameterInt("sizey", GetDefaultParameterInt("sizey"));
        }

        // Compute radius parameters default value
        ComputeRadiusFromIndex(inImage, largestRegion);
        // Compute extent parameters default value
        ComputeExtentFromIndex(inImage, largestRegion);
      }

      unsigned int       nbComponents = inImage->GetNumberOfComponentsPerPixel();
      ListViewParameter* clParam      = dynamic_cast<ListViewParameter*>(GetParameterByKey("cl"));
      // Update the values of the channels to be selected if nbComponents is changed
      if (clParam != nullptr && clParam->GetNbChoices() != nbComponents)
      {
        ClearChoices("cl");
        for (unsigned int idx = 0; idx < nbComponents; ++idx)
        {
          std::ostringstream key, item;
          key << "cl.channel" << idx + 1;
          item << "Channel" << idx + 1;
          AddChoice(key.str(), item.str());
        }
      }

      // Update the start and size parameter depending on the mode
      if (GetParameterString("mode") == "extent")
        ComputeIndexFromExtent();
      if (GetParameterString("mode") == "radius")
        ComputeIndexFromRadius();

      if (GetParameterString("mode") == "fit")
      {
        SetParameterRole("startx", Role_Output);
        SetParameterRole("starty", Role_Output);
        SetParameterRole("sizex", Role_Output);
        SetParameterRole("sizey", Role_Output);
      }
      else
      {
        SetParameterRole("startx", Role_Input);
        SetParameterRole("starty", Role_Input);
        SetParameterRole("sizex", Role_Input);
        SetParameterRole("sizey", Role_Input);
      }
    }

    // If not standard mode start and size parameter will be computed by the application
    if (GetParameterString("mode") != "standard")
    {
      MandatoryOff("startx");
      MandatoryOff("starty");
      MandatoryOff("sizex");
      MandatoryOff("sizey");
      DisableParameter("startx");
      DisableParameter("starty");
      DisableParameter("sizex");
      DisableParameter("sizey");
    }
    else
    {
      MandatoryOn("startx");
      MandatoryOn("starty");
      MandatoryOn("sizex");
      MandatoryOn("sizey");
      EnableParameter("startx");
      EnableParameter("starty");
      EnableParameter("sizex");
      EnableParameter("sizey");
    }

    if (GetParameterString("mode") == "fit" && HasValue("mode.fit.im"))
    {
      MandatoryOff("mode.fit.vect");
      MandatoryOn("mode.fit.im");
    }
    else if (GetParameterString("mode") == "fit" && HasValue("mode.fit.vect"))
    {
      MandatoryOff("mode.fit.im");
      MandatoryOn("mode.fit.vect");
    }
  }

  // This method is called in DoExecute() once
  bool CropRegionOfInterest()
  {
    FloatVectorImageType::RegionType region;

    region.SetSize(0, GetParameterInt("sizex"));
    region.SetSize(1, GetParameterInt("sizey"));
    region.SetIndex(0, GetParameterInt("startx"));
    region.SetIndex(1, GetParameterInt("starty"));
    ImageType* inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();
    if (region.Crop(inImage->GetLargestPossibleRegion()))
    {
      /* SetParameterInt() resets UserValue flag when called from DoExecute(). Disable this behaviour.*/
      DisableInPrivateDo();
      SetParameterInt("sizex", region.GetSize(0), IsParameterEnabled("sizex") && HasUserValue("sizex"));
      SetParameterInt("sizey", region.GetSize(1), IsParameterEnabled("sizey") && HasUserValue("sizey"));
      SetParameterInt("startx", region.GetIndex(0), IsParameterEnabled("startx") && HasUserValue("startx"));
      SetParameterInt("starty", region.GetIndex(1), IsParameterEnabled("starty") && HasUserValue("starty"));
      EnableInPrivateDo(); // Restore default
      return true;
    }
    return false;
  }

  FloatVectorImageType::IndexType GetOffset(FloatVectorImageType::IndexType const& uli)
  {
    FloatVectorImageType::IndexType offset;
    offset.Fill(0);
    if (uli[0] < 0)
      offset[0] = uli[0];
    if (uli[1] < 0)
      offset[1] = uli[1];
    return offset;
  }

  void ComputeIndexFromExtent()
  {
    assert(GetParameterString("mode") == "extent");
    // Compute standard parameter depending on the unit chosen by the user
    // Reference is index
    if (!(HasValue("mode.extent.ulx") && HasValue("mode.extent.uly") && HasValue("mode.extent.lrx") && HasValue("mode.extent.lry")))
    {
      return;
    }
    FloatVectorImageType::IndexType raw_uli, raw_lri;
    if (GetParameterString("mode.extent.unit") == "pxl")
    {
      raw_uli[0]         = std::round(GetParameterFloat("mode.extent.ulx"));
      raw_uli[1]         = std::round(GetParameterFloat("mode.extent.uly"));
      raw_lri[0]         = std::round(GetParameterFloat("mode.extent.lrx"));
      raw_lri[1]         = std::round(GetParameterFloat("mode.extent.lry"));
      m_IsExtentInverted = (raw_lri[0] < raw_uli[0] || raw_lri[1] < raw_uli[1]);
    }
    else if (GetParameterString("mode.extent.unit") == "phy")
    {
      itk::Point<float, 2> ulp, lrp;
      ulp[0]                                  = GetParameterFloat("mode.extent.ulx");
      ulp[1]                                  = GetParameterFloat("mode.extent.uly");
      lrp[0]                                  = GetParameterFloat("mode.extent.lrx");
      lrp[1]                                  = GetParameterFloat("mode.extent.lry");
      m_IsExtentInverted                      = (lrp[0] < ulp[0] || lrp[1] < ulp[1]);
      ImageType*                      inImage = GetParameterImage("in");
      inImage->TransformPhysicalPointToIndex(ulp, raw_uli);
      inImage->TransformPhysicalPointToIndex(lrp, raw_lri);
    }
    else // if( GetParameterString( "mode.extent.unit" ) == "lonlat" )
    {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      ImageType*               inImage     = GetParameterImage("in");
      rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
      rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> ulp_in, lrp_in, ulp_out, lrp_out;
      ulp_in[0] = GetParameterFloat("mode.extent.ulx");
      ulp_in[1] = GetParameterFloat("mode.extent.uly");
      lrp_in[0] = GetParameterFloat("mode.extent.lrx");
      lrp_in[1] = GetParameterFloat("mode.extent.lry");
      // Is this working?
      m_IsExtentInverted = (lrp_in[0] < ulp_in[0] || lrp_in[1] < ulp_in[1]);
      ulp_out            = rsTransform->TransformPoint(ulp_in);
      lrp_out            = rsTransform->TransformPoint(lrp_in);
      inImage->TransformPhysicalPointToIndex(ulp_out, raw_uli);
      inImage->TransformPhysicalPointToIndex(lrp_out, raw_lri);
    }
    // Here we have an extent but we need to make sure we have the two corners
    // we are interested in uli and lri!
    FloatVectorImageType::IndexType uli, lri;
    uli[0] = std::min(raw_uli[0], raw_lri[0]);
    uli[1] = std::min(raw_uli[1], raw_lri[1]);
    lri[0] = std::max(raw_uli[0], raw_lri[0]);
    lri[1] = std::max(raw_uli[1], raw_lri[1]);
    SetParameterInt("startx", uli[0]);
    SetParameterInt("starty", uli[1]);
    // In the case of a negative index the size will be wrong without the
    // offset
    FloatVectorImageType::IndexType offset = GetOffset(uli);
    SetParameterInt("sizex", lri[0] - uli[0] + 1 + offset[0]);
    SetParameterInt("sizey", lri[1] - uli[1] + 1 + offset[1]);
  }

  void ComputeExtentFromIndex(const ImageType* input, const ImageType::RegionType& largestRegion)
  {
    FloatVectorImageType::IndexType uli, lri;
    uli.Fill(0);
    lri[0] = largestRegion.GetSize()[0];
    lri[1] = largestRegion.GetSize()[1];
    if (GetParameterString("mode.extent.unit") == "pxl")
    {
      SetDefaultParameterFloat("mode.extent.ulx", uli[0]);
      SetDefaultParameterFloat("mode.extent.uly", uli[1]);
      SetDefaultParameterFloat("mode.extent.lrx", lri[0]);
      SetDefaultParameterFloat("mode.extent.lry", lri[1]);
    }
    else if (GetParameterString("mode.extent.unit") == "phy")
    {
      itk::Point<float, 2> ulp, lrp;

      input->TransformIndexToPhysicalPoint(uli, ulp);
      SetDefaultParameterFloat("mode.extent.ulx", ulp[0]);
      SetDefaultParameterFloat("mode.extent.uly", ulp[1]);

      input->TransformIndexToPhysicalPoint(lri, lrp);
      SetDefaultParameterFloat("mode.extent.lrx", lrp[0]);
      SetDefaultParameterFloat("mode.extent.lry", lrp[1]);
    }
    else // if ( GetParameterString( "mode.extent.unit" ) == "lonlat" )
    {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      rsTransform->SetInputKeywordList(input->GetImageKeywordlist());
      rsTransform->SetInputProjectionRef(input->GetProjectionRef());
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> ulp_in, lrp_in, ulp_out, lrp_out;
      input->TransformIndexToPhysicalPoint(uli, ulp_in);
      ulp_out = rsTransform->TransformPoint(ulp_in);
      SetDefaultParameterFloat("mode.extent.ulx", ulp_out[0]);
      SetDefaultParameterFloat("mode.extent.uly", ulp_out[1]);

      input->TransformIndexToPhysicalPoint(lri, lrp_in);
      lrp_out = rsTransform->TransformPoint(lrp_in);
      SetDefaultParameterFloat("mode.extent.lrx", lrp_out[0]);
      SetDefaultParameterFloat("mode.extent.lry", lrp_out[1]);
    }
    // We should not do this
    if (!HasUserValue("mode.extent.ulx"))
      SetParameterFloat("mode.extent.ulx", GetDefaultParameterFloat("mode.extent.ulx"));
    if (!HasUserValue("mode.extent.uly"))
      SetParameterFloat("mode.extent.uly", GetDefaultParameterFloat("mode.extent.uly"));
    if (!HasUserValue("mode.extent.lrx"))
      SetParameterFloat("mode.extent.lrx", GetDefaultParameterFloat("mode.extent.lrx"));
    if (!HasUserValue("mode.extent.lry"))
      SetParameterFloat("mode.extent.lry", GetDefaultParameterFloat("mode.extent.lry"));
  }

  void ComputeIndexFromRadius()
  {
    assert(GetParameterString("mode") == "radius");
    // In radius mode we can only compute the extent if we have a center
    // and a radius
    if (!(HasValue("mode.radius.r") && HasValue("mode.radius.cx") && HasValue("mode.radius.cy")))
    {
      return;
    }
    if (GetParameterString("mode.radius.unitr") == "pxl")
    {
      // reference is pixel
      FloatVectorImageType::IndexType centeri, radiusi;
      radiusi[0] = GetParameterFloat("mode.radius.r");
      radiusi[1] = GetParameterFloat("mode.radius.r");
      if (GetParameterString("mode.radius.unitc") == "pxl")
      {
        centeri[0] = GetParameterFloat("mode.radius.cx");
        centeri[1] = GetParameterFloat("mode.radius.cy");
      }
      else if (GetParameterString("mode.radius.unitc") == "phy")
      {
        // transform the center in index
        itk::Point<float, 2> centerp;
        centerp[0]         = GetParameterFloat("mode.radius.cx");
        centerp[1]         = GetParameterFloat("mode.radius.cy");
        ImageType* inImage = GetParameterImage("in");
        inImage->TransformPhysicalPointToIndex(centerp, centeri);
      }
      else // if ( GetParameterString( "mode.radius.unitc" ) == "lon/lat" )
      {
        RSTransformType::Pointer rsTransform = RSTransformType::New();
        ImageType*               inImage     = GetParameterImage("in");
        rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
        rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
        rsTransform->InstantiateTransform();
        itk::Point<float, 2> centerp_in, centerp_out;
        centerp_in[0] = GetParameterFloat("mode.radius.cx");
        centerp_in[1] = GetParameterFloat("mode.radius.cy");
        centerp_out   = rsTransform->TransformPoint(centerp_in);
        inImage->TransformPhysicalPointToIndex(centerp_out, centeri);
      }
      // Corner case like negative index or too high index are taking care
      // of thanks to the DoUpdate() which set min and max
      SetParameterInt("startx", centeri[0] - radiusi[0]);
      SetParameterInt("starty", centeri[1] - radiusi[1]);
      // Don't forget to compute the offset
      FloatVectorImageType::IndexType offset_helper;
      offset_helper[0] = centeri[0] - radiusi[0];
      offset_helper[1] = centeri[1] - radiusi[1];
      auto offset      = GetOffset(offset_helper);
      SetParameterInt("sizex", 2 * radiusi[0] + 1 + offset[0]);
      SetParameterInt("sizey", 2 * radiusi[1] + 1 + offset[1]);
    }
    else // if ( GetParameterString( "mode.radius.unitr" ) == "phy" )
    {
      // reference is physical space
      ImageType* inImage = GetParameterImage("in");
      itk::Point<float, 2> centerp, radiusp;
      radiusp.Fill(GetParameterFloat("mode.radius.r"));
      if (GetParameterString("mode.radius.unitc") == "pxl")
      {
        FloatVectorImageType::IndexType centeri;
        centeri[0] = GetParameterInt("mode.radius.cx");
        centeri[1] = GetParameterInt("mode.radius.cy");
        inImage->TransformIndexToPhysicalPoint(centeri, centerp);
      }
      else if (GetParameterString("mode.radius.unitc") == "phy")
      {
        centerp[0] = GetParameterFloat("mode.radius.cx");
        centerp[1] = GetParameterFloat("mode.radius.cy");
      }
      else // if ( GetParameterString( "mode.radius.unitc" ) == "lon/lat" )
      {
        RSTransformType::Pointer rsTransform = RSTransformType::New();
        rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
        rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
        rsTransform->InstantiateTransform();
        itk::Point<float, 2> centerp_in;
        centerp_in[0] = GetParameterFloat("mode.radius.cx");
        centerp_in[1] = GetParameterFloat("mode.radius.cy");
        centerp       = rsTransform->TransformPoint(centerp_in);
      }
      // Here we have physical point. We are going to construct a physical
      // extent and transform it in index
      itk::Point<float, 2> ulp, lrp;
      FloatVectorImageType::IndexType raw_uli, uli, raw_lri, lri;
      ulp[0] = centerp[0] - radiusp[0];
      ulp[1] = centerp[1] - radiusp[1];
      lrp[0] = centerp[0] + radiusp[0];
      lrp[1] = centerp[1] + radiusp[1];
      inImage->TransformPhysicalPointToIndex(ulp, raw_uli);
      inImage->TransformPhysicalPointToIndex(lrp, raw_lri);
      // Here the extend might be inverted in the case of a negative spacing
      uli[0] = std::min(raw_uli[0], raw_lri[0]);
      uli[1] = std::min(raw_uli[1], raw_lri[1]);
      lri[0] = std::max(raw_uli[0], raw_lri[0]);
      lri[1] = std::max(raw_uli[1], raw_lri[1]);
      SetParameterInt("startx", uli[0]);
      SetParameterInt("starty", uli[1]);
      // In the case of a negative index the size will be wrong without the
      // offset
      auto offset = GetOffset(uli);
      SetParameterInt("sizex", lri[0] - uli[0] + 1 + offset[0]);
      SetParameterInt("sizey", lri[1] - uli[1] + 1 + offset[1]);
    }
  }

  void ComputeRadiusFromIndex(const ImageType* input, const ImageType::RegionType& largestRegion)
  {
    FloatVectorImageType::IndexType centeri, uli;
    centeri[0] = largestRegion.GetSize()[0] / 2 - (largestRegion.GetSize()[0] + 1) % 2;
    centeri[1] = largestRegion.GetSize()[1] / 2 - (largestRegion.GetSize()[1] + 1) % 2;
    uli.Fill(0);
    if (GetParameterString("mode.radius.unitr") == "pxl")
    {
      // Default value take the whole image
      int rad = std::max(centeri[0], centeri[1]);
      SetDefaultParameterFloat("mode.radius.r", rad);
    }
    else // if  ( GetParameterString("mode.radius.unitr") == "phy" )
    {
      itk::Point<float, 2> centerp, ulp;
      input->TransformIndexToPhysicalPoint(centeri, centerp);
      input->TransformIndexToPhysicalPoint(uli, ulp);
      float rad = std::sqrt(std::pow((centerp[0] - ulp[0]), 2) + std::pow((centerp[1] - ulp[1]), 2));
      SetDefaultParameterFloat("mode.radius.r", rad);
    }

    if (!HasUserValue("mode.radius.r"))
      SetParameterFloat("mode.radius.r", GetDefaultParameterFloat("mode.radius.r"));

    // Center
    if (GetParameterString("mode.radius.unitc") == "pxl")
    {
      SetDefaultParameterFloat("mode.radius.cx", centeri[0]);
      SetDefaultParameterFloat("mode.radius.cy", centeri[1]);
    }
    else if (GetParameterString("mode.radius.unitc") == "phy")
    {
      itk::Point<float, 2> centerp;
      input->TransformIndexToPhysicalPoint(centeri, centerp);
      SetDefaultParameterFloat("mode.radius.cx", centerp[0]);
      SetDefaultParameterFloat("mode.radius.cy", centerp[1]);
    }
    else // if ( GetParameterString("mode.radius.unitc") == "lonlat" )
    {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      rsTransform->SetInputKeywordList(input->GetImageKeywordlist());
      rsTransform->SetInputProjectionRef(input->GetProjectionRef());
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> centerp_in, centerp_out;
      input->TransformIndexToPhysicalPoint(centeri, centerp_in);
      centerp_out = rsTransform->TransformPoint(centerp_in);
      SetDefaultParameterFloat("mode.radius.cx", centerp_out[0]);
      SetDefaultParameterFloat("mode.radius.cy", centerp_out[1]);
    }
    if (!HasUserValue("mode.radius.cx"))
      SetParameterFloat("mode.radius.cx", GetDefaultParameterFloat("mode.radius.cx"));
    if (!HasUserValue("mode.radius.cy"))
      SetParameterFloat("mode.radius.cy", GetDefaultParameterFloat("mode.radius.cy"));
  }

  void DoExecute() override
  {
    ImageType* inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();

    if (GetParameterString("mode") == "fit")
    {
      // In fit mode we compute the extend in the physical space of the input
      // from a vector or an other image
      FloatVectorImageType::PointType ulp_out, urp_out, llp_out, lrp_out;
      if (HasValue("mode.fit.vect"))
      {
        otb::ogr::DataSource::Pointer ogrDS;
        ogrDS = otb::ogr::DataSource::New(GetParameterString("mode.fit.vect"), otb::ogr::DataSource::Modes::Read);
        double      ulx, uly, lrx, lry;
        bool        extentAvailable    = true;
        std::string inputProjectionRef = "";
        // First try to get the extent in the metadata
        try
        {
          inputProjectionRef = ogrDS->GetGlobalExtent(ulx, uly, lrx, lry);
        }
        catch (const itk::ExceptionObject&)
        {
          extentAvailable = false;
        }
        // If no extent available force the computation of the extent
        if (!extentAvailable)
        {
          try
          {
            inputProjectionRef = ogrDS->GetGlobalExtent(ulx, uly, lrx, lry, true);
          }
          catch (itk::ExceptionObject& err)
          {

            otbAppLogFATAL(<< "Failed to retrieve the spatial extent of the dataset "
                              "in force mode. The spatial extent is mandatory when "
                              "using mod.fit . Error from library: "
                           << err.GetDescription());
          }
          catch (...)
          {
            otbAppLogFATAL(<< "Failed to retrieve the spatial extent of the dataset "
                              "in force mode. The spatial extent is mandatory when "
                              "using mod.fit");
          }
        }
        RSTransformType::Pointer rsTransform = RSTransformType::New();
        rsTransform->SetInputProjectionRef(inputProjectionRef);
        rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
        rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
        rsTransform->InstantiateTransform();
        itk::Point<float, 2> ulp_in, urp_in, llp_in, lrp_in;
        ulp_in[0] = ulx;
        ulp_in[1] = uly;
        urp_in[0] = ulx;
        urp_in[1] = lry;
        llp_in[0] = lrx;
        llp_in[1] = uly;
        lrp_in[0] = lrx;
        lrp_in[1] = lry;
        ulp_out   = rsTransform->TransformPoint(ulp_in);
        urp_out   = rsTransform->TransformPoint(urp_in);
        llp_out   = rsTransform->TransformPoint(llp_in);
        lrp_out   = rsTransform->TransformPoint(lrp_in);
      }
      else // if( HasValue( "mode.fit.im" ) )
      {
        // Setup the DEM Handler
        otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

        FloatVectorImageType::Pointer referencePtr = GetParameterImage("mode.fit.im");
        referencePtr->UpdateOutputInformation();

        FloatVectorImageType::IndexType uli_ref, uri_ref, lli_ref, lri_ref;

        uli_ref = referencePtr->GetLargestPossibleRegion().GetIndex();
        uri_ref = uli_ref;
        uri_ref[0] += referencePtr->GetLargestPossibleRegion().GetSize()[0];
        lli_ref = uli_ref;
        lli_ref[1] += referencePtr->GetLargestPossibleRegion().GetSize()[1];
        lri_ref = lli_ref;
        lri_ref[0] += referencePtr->GetLargestPossibleRegion().GetSize()[0];

        FloatVectorImageType::PointType ulp_ref, urp_ref, llp_ref, lrp_ref;

        referencePtr->TransformIndexToPhysicalPoint(uli_ref, ulp_ref);
        referencePtr->TransformIndexToPhysicalPoint(uri_ref, urp_ref);
        referencePtr->TransformIndexToPhysicalPoint(lli_ref, llp_ref);
        referencePtr->TransformIndexToPhysicalPoint(lri_ref, lrp_ref);


        RSTransformType::Pointer rsTransform = RSTransformType::New();
        rsTransform->SetInputKeywordList(referencePtr->GetImageKeywordlist());
        rsTransform->SetInputProjectionRef(referencePtr->GetProjectionRef());
        rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
        rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
        rsTransform->InstantiateTransform();

        ulp_out = rsTransform->TransformPoint(ulp_ref);
        urp_out = rsTransform->TransformPoint(urp_ref);
        llp_out = rsTransform->TransformPoint(llp_ref);
        lrp_out = rsTransform->TransformPoint(lrp_ref);
      }
      FloatVectorImageType::IndexType uli_out, uri_out, lli_out, lri_out;

      inImage->TransformPhysicalPointToIndex(ulp_out, uli_out);
      inImage->TransformPhysicalPointToIndex(urp_out, uri_out);
      inImage->TransformPhysicalPointToIndex(llp_out, lli_out);
      inImage->TransformPhysicalPointToIndex(lrp_out, lri_out);

      FloatVectorImageType::IndexType uli, lri;

      uli[0] = std::min({uli_out[0], uri_out[0], lli_out[0], lri_out[0]});
      uli[1] = std::min({uli_out[1], uri_out[1], lli_out[1], lri_out[1]});

      lri[0] = std::max({uli_out[0], uri_out[0], lli_out[0], lri_out[0]});
      lri[1] = std::max({uli_out[1], uri_out[1], lli_out[1], lri_out[1]});

      SetParameterInt("startx", uli[0]);
      SetParameterInt("starty", uli[1]);
      // In the case of a negative index the size will be wrong without the
      // offset
      FloatVectorImageType::IndexType offset = GetOffset(uli);
      SetParameterInt("sizex", lri[0] - uli[0] + offset[0]);
      SetParameterInt("sizey", lri[1] - uli[1] + offset[1]);
    }
    else if (GetParameterString("mode") == "extent")
    {
      if (m_IsExtentInverted)
      {
        otbAppLogFATAL(<< "Extent coordinates are inverted!");
      }
    }

    if (!CropRegionOfInterest())
      otbAppLogWARNING(<< "Could not extract the ROI as it is out of the "
                       "input image.");

    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    extractROIFilter->SetInput(inImage);
    extractROIFilter->SetStartX(GetParameterInt("startx"));
    extractROIFilter->SetStartY(GetParameterInt("starty"));
    extractROIFilter->SetSizeX(GetParameterInt("sizex"));
    extractROIFilter->SetSizeY(GetParameterInt("sizey"));

    for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
    {
      extractROIFilter->SetChannel(GetSelectedItems("cl")[idx] + 1);
    }

    SetParameterOutputImage("out", extractROIFilter->GetOutput());
    RegisterPipeline();
  }

  /** flag to check if given extent was inverted */
  bool m_IsExtentInverted;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ExtractROI)
