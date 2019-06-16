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

#include "otbWrapperApplicationFactory.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

#include "otbVectorDataProjectionFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class ConnectedComponentSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConnectedComponentSegmentation  Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConnectedComponentSegmentation, otb::Application);

  /** Typedefs */
  typedef float InputPixelType;
  static const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned int, Dimension>         LabelImageType;
  typedef otb::Image<unsigned int, Dimension>         MaskImageType;

  typedef otb::VectorData<double, Dimension>          VectorDataType;
  typedef VectorDataType::Pointer                     VectorDataPointerType;

  typedef otb::StreamingConnectedComponentSegmentationOBIAToVectorDataFilter
    < InputVectorImageType,
      LabelImageType,
      MaskImageType,
      VectorDataType >  SegmentationFilterType;

  typedef otb::VectorDataProjectionFilter
        <VectorDataType, VectorDataType>                     VectorDataProjectionFilterType;

private:
  void DoInit() override
  {
    SetName("ConnectedComponentSegmentation");
    SetDescription("Connected component segmentation and object based image filtering of the input image according to user-defined criterions.");
    SetDocLongDescription("This application allows one to perform a masking, connected components segmentation and object based image filtering. First and optionally, a mask can be built based on user-defined criterions to select pixels of the image which will be segmented. Then a connected component segmentation is performed with a user defined criterion to decide whether two neighbouring pixels belong to the same segment or not. After this segmentation step, an object based image filtering is applied using another user-defined criterion reasoning on segment properties, like shape or radiometric attributes. " "Criterions are mathematical expressions analysed by the MuParser library (http://muparser.sourceforge.net/). For instance, expression \"((b1>80) and intensity>95)\" will merge two neighbouring pixel in a single segment if their intensity is more than 95 and their value in the first image band is more than 80. See parameters documentation for a list of available attributes. The output of the object based image filtering is vectorized and can be written in shapefile or KML format. If the input image is in raw geometry, resulting polygons will be transformed to WGS84 using sensor modelling before writing, to ensure consistency with GIS software. For this purpose, a Digital Elevation Model can be provided to the application. The whole processing is done on a per-tile basis for large images, so this application can handle images of arbitrary size.");
    SetDocLimitations("Due to the tiling scheme in case of large images, some segments can be arbitrarily split across multiple tiles.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Segmentation);
	AddDocTag(Tags::Analysis);	

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The image to segment.");

    AddParameter(ParameterType_OutputVectorData, "out", "Output Shape");
    SetParameterDescription("out", "The segmentation shape.");


    AddParameter(ParameterType_String, "mask", "Mask expression");
    SetParameterDescription("mask", "Mask mathematical expression (only if support image is given)");
    MandatoryOff("mask");
    DisableParameter("mask");

    AddParameter(ParameterType_String, "expr", "Connected Component Expression");
    SetParameterDescription("expr", "Formula used for connected component segmentation");

    AddParameter(ParameterType_Int, "minsize", "Minimum Object Size");
    SetParameterDescription("minsize", "Min object size (area in pixel)");
    SetDefaultParameterInt("minsize", 2);
    SetMinimumParameterIntValue("minsize", 1);
    MandatoryOff("minsize");

    AddParameter(ParameterType_String, "obia", "OBIA Expression");
    SetParameterDescription("obia", "OBIA mathematical expression");
    MandatoryOff("obia");
    DisableParameter("obia");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddRAMParameter();

   // Doc example parameter settings
   SetDocExampleParameterValue("in", "ROI_QB_MUL_4.tif");
   SetDocExampleParameterValue("mask", "\"((b1>80)*intensity>95)\"");
   SetDocExampleParameterValue("expr", "\"distance<10\"");
   SetDocExampleParameterValue("minsize", "15");
   SetDocExampleParameterValue("obia", "\"SHAPE_Elongation>8\"");
   SetDocExampleParameterValue("out", "ConnectedComponentSegmentation.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() override
  {
    InputVectorImageType::Pointer inputImage = GetParameterImage("in");

    m_Connected  = SegmentationFilterType::FilterType::New();
    m_Connected->GetFilter()->SetInput(inputImage);

    if (IsParameterEnabled("mask") && HasValue("mask"))
      m_Connected->GetFilter()->SetMaskExpression(GetParameterString("mask"));

    m_Connected->GetFilter()->SetConnectedComponentExpression(GetParameterString("expr"));

    m_Connected->GetFilter()->SetMinimumObjectSize(GetParameterInt("minsize"));

    if (IsParameterEnabled("obia") && HasValue("obia"))
      m_Connected->GetFilter()->SetOBIAExpression(GetParameterString("obia"));

    m_Connected->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
    AddProcess(m_Connected->GetStreamer(),"Computing segmentation");
    m_Connected->Update();

    /*
    * Reprojection of the output VectorData
    *
    * The output of the filter is in image physical coordinates,
    * projection WKT applied if the input image has one
    *
    * We need to reproject in WGS84 if the input image is in sensor model geometry
    */

    std::string projRef = inputImage->GetProjectionRef();
    ImageKeywordlist kwl = inputImage->GetImageKeywordlist();

    VectorDataType::Pointer projectedVD = m_Connected->GetFilter()->GetOutputVectorData();

    if ( projRef.empty() && kwl.GetSize() > 0 )
      {
      // image is in sensor model geometry

      // Reproject VectorData in image projection
      m_Vproj = VectorDataProjectionFilterType::New();
      m_Vproj->SetInput(m_Connected->GetFilter()->GetOutputVectorData());
      m_Vproj->SetInputKeywordList(inputImage->GetImageKeywordlist());
      //m_Vproj->SetInputOrigin(inputImage->GetOrigin());
      //m_Vproj->SetInputSpacing(inputImage->GetSignedSpacing());

      // Setup the DEM Handler
      otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");
      m_Vproj->Update();

      projectedVD = m_Vproj->GetOutput();
      }

    SetParameterOutputVectorData("out", projectedVD);
  }

  /** Members */
  SegmentationFilterType::FilterType::Pointer m_Connected;
  VectorDataProjectionFilterType::Pointer m_Vproj;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConnectedComponentSegmentation)
