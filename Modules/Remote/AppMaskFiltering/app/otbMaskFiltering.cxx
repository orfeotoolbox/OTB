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

//#include <itkBinaryShapeOpeningImageFilter.h>
#include "PersistentMaskFilter.h"
#include "otbMaskStreamStitchingFilter.h"


namespace otb
{
namespace Wrapper
{

class MaskFiltering : public Application
{
public:
  /** Standard class typedefs. */
  typedef MaskFiltering                       Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  typedef otb::ogr::DataSource                            OGRDataSourceType;
  typedef otb::ogr::Layer                                 OGRLayerType;
  typedef otb::ogr::Feature                               OGRFeatureType;

  //typedef itk::BinaryShapeM_BinaryOpeningFilterImageFilter<UInt8ImageType>    BinaryM_BinaryOpeningFilterFilterType;
  typedef FloatImageType                                ImageType;
  //typedef PersistentMaskFilter< ImageType >             PersistentMaskFilterType;
  typedef MaskStreamStitchingFilter< ImageType >             MaskStreamStitchingFilterType;
  typedef MaskFilter<ImageType>                         MaskFilterType;
  
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MaskFiltering, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("MaskFiltering");
    SetDescription("This application performs the vectorization of an input binary mask");

    // Documentation
    SetDocName("Mask Filtering");
    SetDocLongDescription("Given an input binary raster image, this application will output a vector data file containing a polygon for each connected component of the input raster. Additionnaly, the size of each polygon will be computed and added to the raster");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");

    AddParameter(ParameterType_Int,"background","Background value");
    SetParameterDescription("background","Background value of the binary mask");
    MandatoryOff("background");
    
    AddParameter(ParameterType_Int,"foreground","Foreground value");
    SetParameterDescription("foreground","Foreground value of the binary mask");
    MandatoryOff("foreground");
    
    AddParameter(ParameterType_Int,"connectivity","Full connectivity");
    SetParameterDescription("connectivity","Set full connectivity");
    SetMinimumParameterIntValue("connectivity",0);
    SetMaximumParameterIntValue("connectivity",1);
    MandatoryOff("connectivity");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file ");
    SetParameterDescription("out","Output vector");

    AddParameter(ParameterType_Int,"tile","Tile Size");
    SetParameterDescription("tile","Size of the tiles used for streaming");
    MandatoryOff("tile");
     
    AddParameter(ParameterType_StringList,  "feat", "Output Image channels");
    SetParameterDescription("feat","Channels to write in the output image.");
	MandatoryOff("feat");
	
    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("background", 0);
    SetDefaultParameterInt("foreground", 1);
    SetDefaultParameterInt("connectivity", 0);
    SetDefaultParameterInt("tile", 0);
    // Doc example parameter settings
    
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("out","test.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
	// Create the OGR DataSource with the appropriate fields
    std::string projRef = this->GetParameterFloatImage("in")->GetProjectionRef();
    OGRSpatialReference oSRS(projRef.c_str());
    std::string layer_name = "layer";
    std::string field_name = "field";
    OGRDataSourceType::Pointer ogrDS = otb::ogr::DataSource::New(GetParameterString("out"), otb::ogr::DataSource::Modes::Overwrite);
    OGRLayerType layer = ogrDS->CreateLayer(layer_name, &oSRS, wkbMultiPolygon);
    OGRFieldDefn field(field_name.c_str(), OFTInteger);
    layer.CreateField(field, true);
    OGRFieldDefn fieldSize("size", OFTReal);
    
    layer.CreateField(fieldSize, true);
	
	OGRFieldDefn fieldPerimeter("perimeter", OFTReal);
	layer.CreateField(fieldPerimeter, true);
	
    // Mask FIlter : Vectorization of the input raster
    MaskFilterType::Pointer maskFilter = MaskFilterType::New();
    // Labeled image to be vectorized
    maskFilter->SetInput(this->GetParameterFloatImage("in"));
    
    maskFilter->SetOGRLayer(layer) ;
    maskFilter->GetStreamer()->SetTileDimensionTiledStreaming(this->GetParameterInt("tile"));
    
    // Input labels : Labels in the input image that will be vectorized
    
    std::vector<int> inputLabels;
    if(IsParameterEnabled("feat")  )
    {
		std::vector<std::string> inputLabelsStr = GetParameterStringList("feat");
		for (std::vector<std::string>::iterator it = inputLabelsStr.begin(); it != inputLabelsStr.end(); it++)
		{
		  inputLabels.push_back(std::stoi(*it));
		}
	}
	else
	{
		inputLabels.push_back(1);
	}
    maskFilter->SetLabels(inputLabels);
    
    maskFilter->SetUse8Connected(this->GetParameterInt("connectivity"));
    maskFilter->SetFieldName( field_name);
    maskFilter->Initialize();
    maskFilter->Update();
    ogrDS->SyncToDisk();
    // Fusion Filter : Regroup polygons splitted across tiles.
    /*MaskStreamStitchingFilterType::Pointer fusionFilter = MaskStreamStitchingFilterType::New();
    fusionFilter->SetInput(this->GetParameterFloatImage("in"));
    fusionFilter->SetOGRLayer(layer);
    fusionFilter->SetStreamSize(maskFilter->GetStreamSize());
    fusionFilter->GenerateData();*/
  }

}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::MaskFiltering)
