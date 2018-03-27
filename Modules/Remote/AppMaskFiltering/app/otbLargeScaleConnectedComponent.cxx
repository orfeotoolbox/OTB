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

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

/**
 * \class LargeScaleConnectedComponent
 *
 * \brief All-in-one application for the Large Scale connected component framework
 *
 * 
 */
class LargeScaleConnectedComponent : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef LargeScaleConnectedComponent            Self;
  typedef CompositeApplication                    Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

/** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LargeScaleConnectedComponent, otb::CompositeApplication);

private:
  void DoInit() ITK_OVERRIDE
    {
    SetName("LargeScaleConnectedComponent");
    SetDescription("Large-scale segmentation using MeanShift");

    // Documentation
    SetDocName("Large-Scale MeanShift");
    SetDocLongDescription("This application chains together the 5 steps of the "
      "Connected components framework, that is the ImageConnectedComponentSegmentation [1], the "
      "LabelImageVectorization [2], the ComputePolygonsGeometricFeatures [3], the ComputePolygonsSpectralFeatures [4] and the "
      "ObjectBasedFiltering [5].\n\n"
      "It generates a vector data file containing the regions extracted with "
      "the Connected component algorithm using a user defined criterion, features (size, perimeter, min, max and mean for each band, and the covariance matrix). "
      );
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] ImageConnectedComponentSegmentation\n"
      "[2] ComputePolygonsGeometricFeatures\n"
      "[3] ComputePolygonsSpectralFeatures\n"
      "[4] ObjectBasedFiltering");

    AddDocTag(Tags::Segmentation);
    AddDocTag("LSCC");

    ClearApplications();
    AddApplication("ImageConnectedComponentSegmentation", "segmentation", "connected component segmentation step");
    AddApplication("LabelImageVectorization", "vectorization", "Vectorization step");
    AddApplication("ComputePolygonsGeometricFeatures", "geometric", "Geometric features computation step");
    AddApplication("ComputePolygonsSpectralFeatures", "spectral", "Spectral features computation step");
    AddApplication("ObjectBasedFiltering", "filtering", "Vectorization step");

    ShareParameter("in","segmentation.in");
    ShareParameter("expr","segmentation.expr");
    ShareParameter("mask","segmentation.mask");
    MandatoryOff("mask");
    ShareParameter("out","vectorization.out");
    ShareParameter("tile","vectorization.tile");
    MandatoryOff("tile");
    
    Connect("spectral.tile","vectorization.tile");
    Connect("geometric.in","vectorization.out");
    Connect("spectral.in","segmentation.in");
    Connect("spectral.vec","vectorization.out");
/*
    ShareParameter("tilesizex","segmentation.tilesizex");
    ShareParameter("tilesizey","segmentation.tilesizey");
*/
/*    AddParameter(ParameterType_Choice, "mode","Output mode");
    SetParameterDescription("mode", "Type of segmented output");

    AddChoice("mode.vector", "Segmentation as vector output");
    SetParameterDescription("mode.vector","In this mode, the application will "
      "produce a vector file or database and compute field values for each "
      "region");

    AddParameter(ParameterType_InputImage, "mode.vector.imfield", "Support image for field computation");
    SetParameterDescription( "mode.vector.imfield", "This is an optional support image "
      "that can be used to compute field values in each region. Otherwise, the "
      "input image is used as support." );
    MandatoryOff("mode.vector.imfield");

    ShareParameter("mode.vector.out","vectorization.out");

    AddChoice("mode.raster", "Standard segmentation with labeled raster output");
    SetParameterDescription("mode.raster","In this mode, the application will produce a standard labeled raster.");

    ShareParameter("mode.raster.out","merging.out",
      "The output raster image",
      "It corresponds to the output of the small region merging step.");

    AddParameter( ParameterType_Empty, "cleanup", "Temporary files cleaning" );
    EnableParameter( "cleanup" );
    SetParameterDescription( "cleanup",
      "If activated, the application will try to clean all temporary files it created" );
    MandatoryOff( "cleanup" );
*/
    // Setup RAM
    ShareParameter("ram","segmentation.ram");
    Connect("vectorization.ram","segmentation.ram");
    Connect("geometric.ram","segmentation.ram");
    Connect("spectral.ram","segmentation.ram");
    Connect("filtering.ram","segmentation.ram");
    
/*
    Connect("merging.tilesizex","segmentation.tilesizex");
    Connect("merging.tilesizey","segmentation.tilesizey");
    Connect("vectorization.tilesizex","segmentation.tilesizex");
    Connect("vectorization.tilesizey","segmentation.tilesizey");

    // TODO : this is not exactly true, we used to choose the smoothed image instead
    Connect("merging.in","smoothing.in");

    // Setup constant parameters
    GetInternalApplication("smoothing")->SetParameterString("foutpos","foo");
    GetInternalApplication("smoothing")->EnableParameter("foutpos");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("spatialr", "4");
    SetDocExampleParameterValue("ranger", "80");
    SetDocExampleParameterValue("minsize", "16");
    SetDocExampleParameterValue("mode.vector.out", "regions.shp");
*/
    SetOfficialDocLink();
    }

  void DoUpdateParameters() ITK_OVERRIDE
  {}

  void DoExecute() ITK_OVERRIDE
    {
      //GetInternalApplication("segmentation")->SetParameterString("expr", );
      ExecuteInternal("segmentation");
      
      GetInternalApplication("vectorization")->SetParameterInputImage("in", GetInternalApplication("segmentation")->GetParameterOutputImage("out"));
      
      ExecuteInternal("vectorization");
      GetInternalApplication("geometric")->SetParameterString("sizefield", "size");
      GetInternalApplication("geometric")->SetParameterString("perimeterfield", "perimeter");
      ExecuteInternal("geometric");
      
      ExecuteInternal("spectral");
      
      
    /*bool isVector(GetParameterString("mode") == "vector");
    std::string outPath(isVector ?
      GetParameterString("mode.vector.out"):
      GetParameterString("mode.raster.out"));
    std::vector<std::string> tmpFilenames;
    tmpFilenames.push_back(outPath+std::string("_labelmap.tif"));
    tmpFilenames.push_back(outPath+std::string("_labelmap.geom"));
    ExecuteInternal("smoothing");
    // in-memory connexion here (saves 1 additional update for foutpos)
    GetInternalApplication("segmentation")->SetParameterInputImage("in",
      GetInternalApplication("smoothing")->GetParameterOutputImage("fout"));
    GetInternalApplication("segmentation")->SetParameterInputImage("inpos",
      GetInternalApplication("smoothing")->GetParameterOutputImage("foutpos"));
    // temporary file output here
    GetInternalApplication("segmentation")->SetParameterString("out",
      tmpFilenames[0]);
    // take half of previous radii
    GetInternalApplication("segmentation")->SetParameterFloat("spatialr",
      0.5 * (double)GetInternalApplication("smoothing")->GetParameterInt("spatialr"));
    GetInternalApplication("segmentation")->SetParameterFloat("ranger",
      0.5 * GetInternalApplication("smoothing")->GetParameterFloat("ranger"));
    GetInternalApplication("segmentation")->ExecuteAndWriteOutput();

    GetInternalApplication("merging")->SetParameterString("inseg",
      tmpFilenames[0]);
    EnableParameter("mode.raster.out");
    if (isVector)
      {
      tmpFilenames.push_back(outPath+std::string("_labelmap_merged.tif"));
      tmpFilenames.push_back(outPath+std::string("_labelmap_merged.geom"));
      GetInternalApplication("merging")->SetParameterString("out",
        tmpFilenames[2]);
      GetInternalApplication("merging")->ExecuteAndWriteOutput();
      if (IsParameterEnabled("mode.vector.imfield") &&
          HasValue("mode.vector.imfield"))
        {
        GetInternalApplication("vectorization")->SetParameterString("in",
          GetParameterString("mode.vector.imfield"));
        }
      else
        {
        GetInternalApplication("vectorization")->SetParameterString("in",
          GetParameterString("in"));
        }
      GetInternalApplication("vectorization")->SetParameterString("inseg",
        tmpFilenames[2]);
      ExecuteInternal("vectorization");
      }
    else
      {
      GetInternalApplication("merging")->ExecuteAndWriteOutput();
      }
    DisableParameter("mode.raster.out");

    if( IsParameterEnabled( "cleanup" ) )
      {
      otbAppLogINFO( <<"Final clean-up ..." );
      for (unsigned int i=0 ; i<tmpFilenames.size() ; ++i)
        {
        if(itksys::SystemTools::FileExists(tmpFilenames[i].c_str()))
          {
          itksys::SystemTools::RemoveFile(tmpFilenames[i].c_str());
          }
        }
      }*/
    }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleConnectedComponent)
