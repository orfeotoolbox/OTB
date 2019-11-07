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

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

/**
 * \class LargeScaleMeanShift
 *
 * \brief All-in-one application for the LSMS framework
 *
 * This application gathers the 4 steps of the large-scale MeanShift
 * segmentation framework.
 *
 */
class LargeScaleMeanShift : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef LargeScaleMeanShift           Self;
  typedef CompositeApplication          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LargeScaleMeanShift, otb::CompositeApplication);

private:
  void DoInit() override
  {
    SetName("LargeScaleMeanShift");
    SetDescription("Large-scale segmentation using MeanShift");

    // Documentation
    SetDocLongDescription(
        "This application chains together the 4 steps of the "
        "MeanShit framework, that is the MeanShiftSmoothing, the "
        "LSMSSegmentation, the LSMSSmallRegionsMerging and the "
        "LSMSVectorization.\n\n"
        "This application can be a preliminary step for an object-based analysis.\n\n"
        "It generates a vector data file containing the regions extracted with "
        "the MeanShift algorithm. The spatial and range radius parameters allow "
        "adapting the sensitivity of the algorithm depending on the image dynamic "
        "and resolution. There is a step to remove small regions whose size "
        "(in pixels) is less than the given 'minsize' parameter. These regions "
        "are merged to a similar neighbor region. In the output vectors, there "
        "are additional fields to describe each region. In particular the mean "
        "and standard deviation (for each band) is computed for each region "
        "using the input image as support. If an optional 'imfield' image is "
        "given, it will be used as support image instead.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "MeanShiftSmoothing\n"
        "LSMSSegmentation\n"
        "LSMSSmallRegionsMerging\n"
        "LSMSVectorization");

    AddDocTag(Tags::Segmentation);
    AddDocTag("LSMS");

    ClearApplications();
    AddApplication("MeanShiftSmoothing", "smoothing", "Smoothing step");
    AddApplication("LSMSSegmentation", "segmentation", "Segmentation step");
    AddApplication("SmallRegionsMerging", "merging", "Small region merging step");
    AddApplication("LSMSVectorization", "vectorization", "Vectorization step");

    ShareParameter("in", "smoothing.in");
    ShareParameter("spatialr", "smoothing.spatialr");
    ShareParameter("ranger", "smoothing.ranger");
    ShareParameter("minsize", "merging.minsize");

    ShareParameter("tilesizex", "segmentation.tilesizex");
    ShareParameter("tilesizey", "segmentation.tilesizey");

    AddParameter(ParameterType_Choice, "mode", "Output mode");
    SetParameterDescription("mode", "Type of segmented output");

    AddChoice("mode.vector", "Segmentation as vector output");
    SetParameterDescription("mode.vector",
                            "In this mode, the application will "
                            "produce a vector file or database and compute field values for each "
                            "region");

    AddParameter(ParameterType_InputImage, "mode.vector.imfield", "Support image for field computation");
    SetParameterDescription("mode.vector.imfield",
                            "This is an optional support image "
                            "that can be used to compute field values in each region. Otherwise, the "
                            "input image is used as support.");
    MandatoryOff("mode.vector.imfield");

    ShareParameter("mode.vector.out", "vectorization.out");

    AddChoice("mode.raster", "Standard segmentation with labeled raster output");
    SetParameterDescription("mode.raster", "In this mode, the application will produce a standard labeled raster.");

    ShareParameter("mode.raster.out", "merging.out", "The output raster image", "It corresponds to the output of the small region merging step.");

    AddParameter(ParameterType_Bool, "cleanup", "Temporary files cleaning");
    SetParameterDescription("cleanup", "If activated, the application will try to clean all temporary files it created");
    SetParameterInt("cleanup", 1);

    // Setup RAM
    ShareParameter("ram", "smoothing.ram");
    Connect("merging.ram", "smoothing.ram");
    Connect("vectorization.ram", "smoothing.ram");

    Connect("vectorization.tilesizex", "segmentation.tilesizex");
    Connect("vectorization.tilesizey", "segmentation.tilesizey");

    // TODO : this is not exactly true, we used to choose the smoothed image instead
    Connect("merging.in", "smoothing.in");

    // Setup constant parameters
    GetInternalApplication("smoothing")->SetParameterString("foutpos", "foo");
    GetInternalApplication("smoothing")->EnableParameter("foutpos");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("spatialr", "4");
    SetDocExampleParameterValue("ranger", "80");
    SetDocExampleParameterValue("minsize", "16");
    SetDocExampleParameterValue("mode.vector.out", "regions.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    bool                     isVector(GetParameterString("mode") == "vector");
    std::string              outPath(isVector ? GetParameterString("mode.vector.out") : GetParameterString("mode.raster.out"));
    std::vector<std::string> tmpFilenames;
    tmpFilenames.push_back(outPath + std::string("_labelmap.tif"));
    tmpFilenames.push_back(outPath + std::string("_labelmap.geom"));
    ExecuteInternal("smoothing");
    // in-memory connexion here (saves 1 additional update for foutpos)
    GetInternalApplication("segmentation")->SetParameterInputImage("in", GetInternalApplication("smoothing")->GetParameterOutputImage("fout"));
    GetInternalApplication("segmentation")->SetParameterInputImage("inpos", GetInternalApplication("smoothing")->GetParameterOutputImage("foutpos"));
    // temporary file output here
    GetInternalApplication("segmentation")->SetParameterString("out", tmpFilenames[0]);
    // take half of previous radii
    GetInternalApplication("segmentation")->SetParameterFloat("spatialr", 0.5 * (double)GetInternalApplication("smoothing")->GetParameterInt("spatialr"));
    GetInternalApplication("segmentation")->SetParameterFloat("ranger", 0.5 * GetInternalApplication("smoothing")->GetParameterFloat("ranger"));
    GetInternalApplication("segmentation")->ExecuteAndWriteOutput();

    GetInternalApplication("merging")->SetParameterString("inseg", tmpFilenames[0]);
    EnableParameter("mode.raster.out");
    if (isVector)
    {
      tmpFilenames.push_back(outPath + std::string("_labelmap_merged.tif"));
      tmpFilenames.push_back(outPath + std::string("_labelmap_merged.geom"));
      GetInternalApplication("merging")->SetParameterString("out", tmpFilenames[2]);
      GetInternalApplication("merging")->ExecuteAndWriteOutput();
      if (IsParameterEnabled("mode.vector.imfield") && HasValue("mode.vector.imfield"))
      {
        GetInternalApplication("vectorization")->SetParameterInputImage("in", GetParameterImageBase("mode.vector.imfield"));
      }
      else
      {
        GetInternalApplication("vectorization")->SetParameterInputImage("in", GetParameterImageBase("in"));
      }
      GetInternalApplication("vectorization")->SetParameterString("inseg", tmpFilenames[2]);
      ExecuteInternal("vectorization");
    }
    else
    {
      GetInternalApplication("merging")->ExecuteAndWriteOutput();
    }
    DisableParameter("mode.raster.out");

    if (GetParameterInt("cleanup"))
    {
      otbAppLogINFO(<< "Final clean-up ...");
      for (unsigned int i = 0; i < tmpFilenames.size(); ++i)
      {
        if (itksys::SystemTools::FileExists(tmpFilenames[i]))
        {
          itksys::SystemTools::RemoveFile(tmpFilenames[i]);
        }
      }
    }
  }
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleMeanShift)
