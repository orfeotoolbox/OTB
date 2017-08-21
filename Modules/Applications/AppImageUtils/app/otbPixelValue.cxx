/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
  void DoInit() ITK_OVERRIDE
  {
    SetName("PixelValue");
    SetDescription("Get the value of a pixel.");

    // Documentation
    SetDocName("Pixel Value");
    SetDocLongDescription("Get the value of a pixel.\nPay attention, index starts at 0.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

	AddDocTag("Miscellaneous");
    AddDocTag("Utilities");
    AddDocTag("Coordinates");
    AddDocTag("Raster");

    AddParameter(ParameterType_InputImage , "in", "Input Image");
    SetParameterDescription("in" , "Input image");
    SetParameterString("in" , "/home/antoine/dev/otb-data/Examples/QB_1_ortho.tif");

    AddParameter(ParameterType_Float , "coordx" , "X coordinate" );
    SetParameterDescription("coordx" , " ");
    AddParameter(ParameterType_Float , "coordy" , "Y coordinate" );
    SetParameterDescription("coordy" , " ");

    AddParameter(ParameterType_Choice , "mode" , 
          "Coordinate system used to designate the pixel");
    AddChoice( "mode.ind" , "Index");
    AddChoice( "mode.phy" , "Image physical space");
    AddChoice( "mode.epsg" , "EPSG coordinates");

    AddParameter(ParameterType_Int , "epsg" , "EPSG code");
    SetParameterDescription("epsg" , "EPSG code");
    MandatoryOff("epsg");

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

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if ( HasValue("in") )
      {
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");

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

      ExtractROIFilterType::InputImageType::RegionType  
                      largestRegion = inImage->GetLargestPossibleRegion();
      // SetMaximumParameterIntValue("coordx", largestRegion.GetSize(0)-1);
      // SetMaximumParameterIntValue("coordy", largestRegion.GetSize(1)-1);
      }
  }

  void DoExecute() ITK_OVERRIDE
  {
    std::ostringstream ossOutput;
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    ExtractROIFilterType::Pointer extractor = ExtractROIFilterType::New();
    extractor->SetInput(inImage);
    FloatVectorImageType::IndexType id;
    bool isPixelIn = false;
    if (GetParameterString( "mode" ) == "ind" )
      {
      id[0] = static_cast< int >( GetParameterFloat( "coordx" ) );
      id[1] = static_cast< int >( GetParameterFloat( "coordy" ) );
      if (id[0] >= inImage->GetLargestPossibleRegion().GetSize()[0] 
       || id[1] >= inImage->GetLargestPossibleRegion().GetSize()[1]
       || id[0] < 0 || id[1] < 0 )
        {
        isPixelIn = false;
        }
      else
        {
        isPixelIn = true;
        }
      }

    if (GetParameterString( "mode" ) == "phy" )
      {
      itk::Point<float, 2> pixel;
      pixel[ 0 ] = GetParameterFloat( "coordx" );
      pixel[ 1 ] = GetParameterFloat( "coordy" );
      isPixelIn = inImage->TransformPhysicalPointToIndex(pixel,id);
      }
    
    if (GetParameterString( "mode" ) == "epsg" )
      {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      rsTransform->SetOutputKeywordList( inImage->GetImageKeywordlist() );
      rsTransform->SetOutputProjectionRef( inImage->GetProjectionRef() );
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> pixelIn , pixelOut;
      pixelIn[ 0 ] = GetParameterFloat( "coordx" );
      pixelIn[ 1 ] = GetParameterFloat( "coordy" );
      rsTransform->InstantiateTransform();
      pixelOut = rsTransform->TransformPoint(pixelIn);
      isPixelIn = inImage->TransformPhysicalPointToIndex(pixelOut,id);
      }

    FloatVectorImageType::SizeType size;
    size.Fill(0);

    FloatVectorImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(id);


    if ( !isPixelIn )
      {
      otbAppLogFATAL(<<"Specified position out of the input image");
      }

    extractor->SetExtractionRegion(region);
    // Extract the channels if needed
    if ( GetParameterByKey("cl")->GetActive() )
      {
      for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
        {
        extractor->SetChannel(GetSelectedItems("cl")[idx] + 1 );
        }
      }
    
    extractor->Update();
    
    // Display the pixel value
    id.Fill(0);
    std::ostringstream oss;
    oss << extractor->GetOutput()->GetPixel(id)<<std::endl;
    
    SetParameterString("value", oss.str(), false);
    //Display image information in the dedicated logger
    otbAppLogINFO( << oss.str() );
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::PixelValue)
