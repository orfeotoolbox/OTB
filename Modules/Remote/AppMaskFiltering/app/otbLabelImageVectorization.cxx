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

#include "PersistentMaskVectorizationFilter.h"
#include "otbMaskStreamStitchingFilter.h"



namespace otb
{
namespace Wrapper
{

class LabelImageVectorization: public Application
{
public:
  /** Standard class typedefs. */
  typedef LabelImageVectorization                      Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  /** Vector data typedefs. */
  typedef otb::ogr::DataSource                            OGRDataSourceType;
  typedef otb::ogr::Layer                                 OGRLayerType;
  typedef otb::ogr::Feature                               OGRFeatureType;

  /** Raster data typedef */ 
  typedef unsigned int                                                PixelType;
  typedef otb::Image<PixelType, 2>                                 LabelImageType;

  /** Filters typedefs */
  typedef MaskStreamStitchingFilter< LabelImageType >             MaskStreamStitchingFilterType;
  typedef MaskFilter<LabelImageType>                         MaskFilterType;
  
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LabelImageVectorization, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("LabelImageVectorization");
    SetDescription("This application performs the vectorization of an input label image");

    // Documentation
    SetDocName("Mask Filtering");
    SetDocLongDescription("Given an input label raster image, this application will output a vector data file containing a polygon for each connected component of the input raster, where two components are connected if they share the same label");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_Int,"connectivity","Full connectivity");
    SetParameterDescription("connectivity","Set full connectivity");
    SetMinimumParameterIntValue("connectivity",0);
    SetMaximumParameterIntValue("connectivity",1);
    MandatoryOff("connectivity");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file ");
    SetParameterDescription("out","Output vector");

    AddParameter(ParameterType_Choice,"outmode","Writing mode for the output vector file");
    SetParameterDescription("outmode","This allows one to set the writing behaviour for the output vector file. Please note that the actual behaviour depends on the file format.");
    
    AddChoice("outmode.overwrite","Overwrite output vector file if existing.");
    SetParameterDescription("outmode.overwrite","If the output vector file already exists, it is completely destroyed (including all its layers) and recreated from scratch.");

    AddChoice("outmode.update","Update output vector file");
    SetParameterDescription("outmode.update","The output vector file is opened in update mode if existing");


    AddParameter(ParameterType_Int,"tile","Tile Size");
    SetParameterDescription("tile","Size of the tiles used for streaming");
    MandatoryOff("tile");
     

    AddParameter(ParameterType_Int,"fusion","fusion");
    SetParameterDescription("fusion","Perform polygon fusion");
    MandatoryOff("fusion");
     
    AddParameter(ParameterType_StringList,  "feat", "Output Image channels");
    SetParameterDescription("feat","Channels to write in the output image.");
    MandatoryOff("feat");
	
    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("connectivity", 0);
    SetDefaultParameterInt("tile", 0);
    SetDefaultParameterInt("fusion", 0);
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
    clock_t tic = clock();
    
    
    // Create the OGR DataSource with the appropriate fields
    std::string projRef = this->GetParameterUInt32Image("in")->GetProjectionRef();
    OGRSpatialReference oSRS(projRef.c_str());
    std::string layer_name = "layer";
    std::string field_name = "field";
    std::string outmode = GetParameterString("outmode");
    std::string dataSourceName = GetParameterString("out");
    OGRDataSourceType::Pointer ogrDS;
    
    if (outmode == "overwrite")
    {
      // Create the datasource
      ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Overwrite);
    }
    else if (outmode == "update")
    {
      // Create the datasource
      ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerUpdate);
    }
    else
    {
      otbAppLogFATAL(<<"invalid outmode"<< outmode);
    }
    //ogrDS = otb::ogr::DataSource::New(GetParameterString("out"), otb::ogr::DataSource::Modes::Overwrite);
    OGRLayerType layer = ogrDS->CreateLayer(layer_name, &oSRS, wkbPolygon);
    OGRFieldDefn field(field_name.c_str(), OFTInteger);
    layer.CreateField(field, true);

    // Mask FIlter : Vectorization of the input raster
    MaskFilterType::Pointer maskFilter = MaskFilterType::New();
    
    // Labeled image to be vectorized
    maskFilter->SetInput(this->GetParameterUInt32Image("in"));
    maskFilter->SetOGRLayer(layer) ;
    maskFilter->GetStreamer()->SetTileDimensionTiledStreaming(this->GetParameterInt("tile"));
    
    
    // Input labels : Labels in the input image that will be vectorized
    std::vector<int> inputLabels;
    if(IsParameterEnabled("feat") )
    {
      
      std::vector<std::string> inputLabelsStr = GetParameterStringList("feat");
      for (std::vector<std::string>::iterator it = inputLabelsStr.begin(); it != inputLabelsStr.end(); it++)
      {
        inputLabels.push_back(std::stoi(*it));
      }

    }
    else
    {
      // if no labels are given to the filter, they are all used
      inputLabels.clear();
    }
    
    maskFilter->SetLabels(inputLabels);
    maskFilter->SetUse8Connected(this->GetParameterInt("connectivity"));
    maskFilter->SetFieldName( field_name);
    maskFilter->Initialize();
    maskFilter->Update();
    ogrDS->SyncToDisk();
    
    
    otbAppLogINFO("Vectorization done")
    
    // Fusion Filter : Regroup polygons splitted across tiles.
    if (this->GetParameterInt("fusion") == 1)
    {
      MaskStreamStitchingFilterType::Pointer fusionFilter = MaskStreamStitchingFilterType::New();
      fusionFilter->SetInput(this->GetParameterUInt32Image("in"));
      fusionFilter->SetOGRLayer(layer);
      fusionFilter->SetStreamSize(maskFilter->GetStreamSize());
      fusionFilter->GenerateData();
    }
    
    clock_t toc = clock();
    otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");

  }
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LabelImageVectorization)
