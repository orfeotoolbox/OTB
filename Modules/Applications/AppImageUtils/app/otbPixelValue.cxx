/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "otbGenericRSTransform.h"

namespace otb
{
namespace Wrapper
{

class PixelValue : public Application
{
public:
  /** Standard class typedefs. */
  typedef PixelValue                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType,
              FloatVectorImageType::InternalPixelType> ExtractROIFilterType;

  typedef otb::GenericRSTransform<>  RSTransformType;
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(PixelValue, otb::Application);

private:
  void DoInit() override
  {
    SetName("PixelValue");
    SetDescription("Get the value of a pixel.");

    // Documentation
    SetDocName("Pixel Value");
    SetDocLongDescription("This application gives the value of a selected "
      "pixel. There are three ways to designate a pixel, with its index, "
      "its physical coordinate (in the physical space attached to the image), "
      "and with geographical coordinate system. Coordinates will be "
      "interpreted differently depending on which mode is chosen.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);
    AddDocTag(Tags::Coordinates);

    AddParameter(ParameterType_InputImage , "in", "Input Image");
    SetParameterDescription("in" , "Input image");

    AddParameter(ParameterType_Float , "coordx" , "X coordinate" );
    SetParameterDescription("coordx" ,
          "This will be the X coordinate interpreted depending on the "
          "chosen mode");
    AddParameter(ParameterType_Float , "coordy" , "Y coordinate" );
    SetParameterDescription("coordy" ,
          "This will be the Y coordinate interpreted depending on the "
          "chosen mode");

    AddParameter(ParameterType_Choice , "mode" ,
          "Coordinate system used to designate the pixel");
    SetParameterDescription( "mode" ,
          "Different modes can be selected, default mode is Index.");
    AddChoice( "mode.index" , "Index");
    SetParameterDescription( "mode.index" ,
          "This mode uses the given coordinates as index to locate the pixel.");
    AddChoice( "mode.physical" , "Image physical space");
    SetParameterDescription( "mode.physical" ,
          "This mode interprets the given coordinates in the image "
          "physical space.");
    AddChoice( "mode.epsg" , "EPSG coordinates");
    SetParameterDescription( "mode.epsg" ,
          "This mode interprets the given coordinates in the specified "
          "geographical coordinate system by the EPSG code.");

    AddParameter(ParameterType_Int , "mode.epsg.code" , "EPSG code");
    SetParameterDescription("mode.epsg.code" ,
          "This code is used to define a geographical coordinate system. "
          "If no system is specified, WGS84 (EPSG: 4326) is used by default.");
    MandatoryOff("mode.epsg.code");

    AddParameter(ParameterType_ListView,"cl","Channels");
    SetParameterDescription("cl","Displayed channels");
    MandatoryOff("cl");

    AddParameter(ParameterType_String,"value","Pixel Value");
    SetParameterDescription("value", "Pixel radiometric value");
    SetParameterRole("value", Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("coordx", "50");
    SetDocExampleParameterValue("coordy", "100");
    SetDocExampleParameterValue("cl", "Channel1");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if ( HasValue("in") )
      {
      ExtractROIFilterType::InputImageType * inImage = GetParameterImage("in");

      // Update the values of the channels to be selected
      unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
      ClearChoices("cl");
      for (unsigned int idx = 0; idx < nbComponents; ++idx)
        {
        std::ostringstream key, item;
        key<<"cl.channel"<<idx+1;
        item<<"Channel"<<idx+1;
        AddChoice(key.str(), item.str());
        }
      }
  }

  std::string CreateBoundaryBox( const std::string & mode )
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    FloatVectorImageType::IndexType min , max;
    min.Fill(0);
    max[0] = inImage->GetLargestPossibleRegion().GetSize()[0] - 1;
    max[1] = inImage->GetLargestPossibleRegion().GetSize()[1] - 1;
    std::string boundaries[4];
    if (mode == "index")
      {
      boundaries[0] = std::to_string(max[0]);
      boundaries[1] = std::to_string(max[1]);
      boundaries[2] = std::to_string(min[0]);
      boundaries[3] = std::to_string(min[1]);
      }
    else if (mode == "physical")
      {
      itk::Point< float, 2 > minP(0) , maxP(0);
      inImage->TransformIndexToPhysicalPoint(min,minP);
      inImage->TransformIndexToPhysicalPoint(max,maxP);
      boundaries[0] = std::to_string(std::max(minP[0],maxP[0]));
      boundaries[1] = std::to_string(std::max(minP[1],maxP[1]));
      boundaries[2] = std::to_string(std::min(minP[0],maxP[0]));
      boundaries[3] = std::to_string(std::min(minP[1],maxP[1]));
      }
    else if (mode == "epsg")
      {
      RSTransformType::Pointer inverse = RSTransformType::New();
      if ( HasUserValue("mode.epsg.code") )
        {
        std::string wktFromEpsg = otb::SpatialReference::FromEPSG(GetParameterInt( "mode.epsg.code" )).ToWkt();
        inverse->SetOutputProjectionRef(wktFromEpsg);
        }
      inverse->SetInputKeywordList( inImage->GetImageKeywordlist() );
      inverse->SetInputProjectionRef( inImage->GetProjectionRef() );
      inverse->InstantiateTransform();
      itk::Point< float, 2 > minPOut(0) , maxPOut(0) , minP(0) , maxP(0);
      inImage->TransformIndexToPhysicalPoint(min,minP);
      inImage->TransformIndexToPhysicalPoint(max,maxP);
      minPOut = inverse->TransformPoint( minP );
      maxPOut = inverse->TransformPoint( maxP );
      boundaries[0] = std::to_string(std::max(minPOut[0],maxPOut[0]));
      boundaries[1] = std::to_string(std::max(minPOut[1],maxPOut[1]));
      boundaries[2] = std::to_string(std::min(minPOut[0],maxPOut[0]));
      boundaries[3] = std::to_string(std::min(minPOut[1],maxPOut[1]));
      }

    for (int i = 0 ; i<4 ; i++)
      {
      std::size_t coma = boundaries[i].find(".");
      if ( coma != std::string::npos )
        {
        std::size_t zero = boundaries[i].find_last_not_of("0");
        if ( zero != std::string::npos )
          boundaries[i].erase(zero + 1);
        else
          boundaries[i] = "0";
        }
      }

    std::string box = "";
    box += "["+boundaries[2]+" , "+boundaries[0]+"] x ";
    box += "["+boundaries[3]+" , "+boundaries[1]+"]";
    return box;
  }

  void DoExecute() override
  {
    std::string mode = GetParameterString( "mode" );
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    FloatVectorImageType::IndexType id ;
    id.Fill(0);
    bool isPixelIn( false );
    if ( mode == "index" )
      {
      id[0] = static_cast< int >( GetParameterFloat( "coordx" ) );
      id[1] = static_cast< int >( GetParameterFloat( "coordy" ) );
      if (static_cast< unsigned int >( id[0] ) >=
                    inImage->GetLargestPossibleRegion().GetSize()[0]
       || static_cast< unsigned int >( id[1] ) >=
                    inImage->GetLargestPossibleRegion().GetSize()[1]
       || id[0] < 0 || id[1] < 0 )
        {
        id.Fill(0);
        isPixelIn = false;
        }
      else
        {
        isPixelIn = true;
        }
      }

    else if ( mode == "physical" )
      {
      itk::Point<float, 2> pixel(0);
      pixel[ 0 ] = GetParameterFloat( "coordx" );
      pixel[ 1 ] = GetParameterFloat( "coordy" );
      isPixelIn = inImage->TransformPhysicalPointToIndex(pixel,id);
      }

    else if ( mode == "epsg" )
      {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      if ( HasUserValue("mode.epsg.code") )
        {
        std::string wktFromEpsg  = otb::SpatialReference::FromEPSG(GetParameterInt( "mode.epsg.code" )).ToWkt();
        rsTransform->SetInputProjectionRef(wktFromEpsg);
        }
      rsTransform->SetOutputKeywordList( inImage->GetImageKeywordlist() );
      rsTransform->SetOutputProjectionRef( inImage->GetProjectionRef() );
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> pixelIn(0) , pixelOut(0);
      pixelIn[ 0 ] = GetParameterFloat( "coordx" );
      pixelIn[ 1 ] = GetParameterFloat( "coordy" );
      rsTransform->InstantiateTransform();
      pixelOut = rsTransform->TransformPoint(pixelIn);
      isPixelIn = inImage->TransformPhysicalPointToIndex(pixelOut,id);
      }

    if ( !isPixelIn )
      {
      std::string why = "Accessible pixels are in ";
      why += CreateBoundaryBox( GetParameterString( "mode" ) );
      why += " for the selected mode.";
      otbAppLogFATAL(<<"Specified position out of bound.\n" + why);
      }

    ExtractROIFilterType::Pointer extractor = ExtractROIFilterType::New();
    extractor->SetInput(inImage);

    // Extract the channels if needed
    if ( GetParameterByKey("cl")->GetActive() )
      {
      for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
        {
        extractor->SetChannel(GetSelectedItems("cl")[idx] + 1 );
        }
      }
    FloatVectorImageType::SizeType size;
    size.Fill(1);
    FloatVectorImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(id);

    extractor->SetExtractionRegion(region);
    extractor->Update();

    // Display the pixel value
    id.Fill(0);
    std::ostringstream oss;
    oss << extractor->GetOutput()->GetPixel(id);
    SetParameterString("value", oss.str());
    //Display image information in the dedicated logger
    otbAppLogINFO( << oss.str() );
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::PixelValue)
