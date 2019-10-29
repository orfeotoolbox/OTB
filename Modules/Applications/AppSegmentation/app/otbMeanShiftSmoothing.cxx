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

#include "otbMeanShiftSmoothingImageFilter.h"

namespace otb
{
namespace Wrapper
{


class MeanShiftSmoothing : public Application
{
public:
  /** Standard class typedefs. */
  typedef MeanShiftSmoothing            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::MeanShiftSmoothingImageFilter<FloatVectorImageType, FloatVectorImageType> MSFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MeanShiftSmoothing, otb::Application);

private:
  void DoInit() override
  {
    SetName("MeanShiftSmoothing");
    SetDescription("This application smooths an image using the MeanShift algorithm.");

    // Documentation
    SetDocLongDescription(
        "MeanShift [1,2,3] is an iterative edge-preserving image smoothing"
        " algorithm often used in image processing and as a first step for"
        " image segmentation. The MeanShift algorithm can be applied to"
        " multispectral images.\n\n"
        "At first iteration, for any given pixel of the input image, the"
        " filtered value correspond to the average spectral signature of"
        " neighborhood pixels that are both spatially closer than the spatial"
        " radius parameter (spatialr) and with spectral signature that have an euclidean"
        " distance to the input pixel lower than the range radius (ranger), that is,"
        " pixels that are both close in space and in spectral signatures."
        " Subsequent iterations will repeat this process by considering that"
        " the pixel signature corresponds to the average spectral signature"
        " computed during previous iteration, and that the pixel position"
        " corresponds to the average position of pixels used to compute the"
        " average signature."
        "The algorithm stops when the maximum number of iterations (maxiter) is reached,"
        " or when the position and spectral signature does not change much"
        " between iterations, according to the convergence threshold (thres). If the"
        " modesearch option is used then convergence will also stops if the"
        " spatial position reaches a pixel that has already converged. This"
        " will speed-up convergence, at the expense of stability of the result.\n\n"

        "The application outputs the image of the final averaged spectral"
        " signatures (fout), and can also optionally output the 2D"
        " displacement field between input pixel position and final pixel"
        " position after convergence (foutpos).\n\n"

        "Note that computing an euclidean distance between spectral signatures"
        " may be inaccurate and that techniques such as color space transform or image"
        " normalisation could be applied before using this application. Also"
        " note that most satellite images noise model is not gaussian, since"
        " noise variance linearly depends on radiance (the higher the"
        " radiance, the higher the noise variance). To account for such noise"
        " model, the application provides the range radius ramp option"
        " (rangeramp), which will vary the range radius linearly with the"
        " central pixel intensity. Default value is 1. (no ramp).\n\n"

        "This application is the first step of the large scale MeanShift method"
        " depicted in [4]. Both outputs (fout and foutpos) can be passed to the"
        " large scale MeanShift segmentation application [5]. If the"
        " application is used for large scale MeanShift, modesearch option should be off.");

    SetDocLimitations(
        "When modesearch is on, the application will yield slightly different"
        " results between executions, due to multi-threading. Results will also"
        " not be stable [4].");

    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "[1] Comaniciu, D., & Meer, P. (2002). Mean shift: A robust approach"
        " toward feature space analysis. IEEE Transactions on pattern analysis"
        " and machine intelligence, 24(5), 603-619.\n"
        "[2] Comaniciu, D., & Meer, P. (1997, June). Robust analysis of feature"
        " spaces: color image segmentation. In Computer Vision and Pattern"
        " Recognition, 1997. Proceedings., 1997 IEEE Computer Society Conference"
        " on (pp. 750-755). IEEE.\n"
        "[3] Comaniciu, D., & Meer, P. (1999). Mean shift analysis and"
        " applications. In Computer Vision, 1999. The Proceedings of the Seventh"
        " IEEE International Conference on (Vol. 2, pp. 1197-1203). IEEE.\n"
        "[4] Michel, J., Youssefi, D., & Grizonnet, M. (2015). Stable"
        " mean-shift algorithm and its application to the segmentation of"
        " arbitrarily large remote sensing images. IEEE Transactions on"
        " Geoscience and Remote Sensing, 53(2), 952-964.\n"
        "[5] LSMSSegmentation application");

    AddDocTag(Tags::Filter);
    AddDocTag("LSMS");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription(
        "in", "The input image can be any single or multiband image. Beware of pontential imbalance between bands ranges as it may alter euclidean distance.");

    AddParameter(ParameterType_OutputImage, "fout", "Spectral filtered output");
    SetParameterDescription("fout",
                            "This output image contains the final average spectral signatures of each pixel. The output type should be at least as wide as the "
                            "input image type. Floating point encoding is advised. This output can be used as input image (in) of the LSMSSegmentation "
                            "application [4,5].");

    AddParameter(ParameterType_OutputImage, "foutpos", "Spatial filtered displacement output");
    SetParameterDescription("foutpos",
                            " This output image contains the 2D displacement between the input pixel spatial position and the final position after "
                            "convergence. Floating point encoding is mandatory. This output can be used as input image (in) of the LSMSSegmentation "
                            "application [4,5].");
    MandatoryOff("foutpos");

    AddParameter(ParameterType_Int, "spatialr", "Spatial radius");
    SetParameterDescription("spatialr",
                            "Radius of the spatial neighborhood for averaging. Higher values will result in more smoothing and higher processing time.");
    SetDefaultParameterInt("spatialr", 5);
    MandatoryOff("spatialr");

    AddParameter(ParameterType_Float, "ranger", "Range radius");
    SetParameterDescription("ranger",
                            "Threshold on spectral signature euclidean distance (expressed in radiometry unit) to consider neighborhood pixel for averaging. "
                            "Higher values will be less edge-preserving (more similar to simple average in neighborhood), whereas lower values will result in "
                            "less noise smoothing. Note that this parameter has no effect on processing time.");
    SetDefaultParameterFloat("ranger", 15.0);
    MandatoryOff("ranger");

    AddParameter(ParameterType_Float, "thres", "Mode convergence threshold");
    SetParameterDescription("thres", "Algorithm will stop if update of average spectral signature and spatial position is below this threshold.");
    SetMinimumParameterFloatValue("thres", 0.0);
    SetDefaultParameterFloat("thres", 0.1);
    MandatoryOff("thres");

    AddParameter(ParameterType_Int, "maxiter", "Maximum number of iterations");
    SetParameterDescription("maxiter", "Algorithm will stop if convergence threshold is not met after the maximum number of iterations.");
    SetDefaultParameterInt("maxiter", 100);
    SetMinimumParameterIntValue("maxiter", 1);
    MandatoryOff("maxiter");

    AddParameter(ParameterType_Float, "rangeramp", "Range radius ramp coefficient");
    SetParameterDescription("rangeramp", "Vary the range radius linearly with the central pixel intensity (experimental).");
    SetDefaultParameterFloat("rangeramp", 0.);
    SetMinimumParameterFloatValue("rangeramp", 0);
    MandatoryOff("rangeramp");

    AddParameter(ParameterType_Bool, "modesearch", "Mode search");
    SetParameterDescription("modesearch",
                            "If activated pixel iterative convergence is stopped if the path crosses an already converged pixel. Be careful, with this option, "
                            "the result will slightly depend on thread number and the results will not be stable (see [4] for more details).");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "maur_rgb.png");
    SetDocExampleParameterValue("fout", "smooth.tif");
    SetDocExampleParameterValue("foutpos", "position.tif");
    SetDocExampleParameterValue("spatialr", "16");
    SetDocExampleParameterValue("ranger", "16");
    SetDocExampleParameterValue("thres", "0.1");
    SetDocExampleParameterValue("maxiter", "100");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    FloatVectorImageType* input = GetParameterImage("in");

    MSFilterType::Pointer filter = MSFilterType::New();

    filter->SetInput(input);

    filter->SetSpatialBandwidth(GetParameterInt("spatialr"));
    filter->SetRangeBandwidth(GetParameterFloat("ranger"));
    filter->SetThreshold(GetParameterFloat("thres"));
    filter->SetMaxIterationNumber(GetParameterInt("maxiter"));
    filter->SetRangeBandwidthRamp(GetParameterFloat("rangeramp"));
    filter->SetModeSearch(GetParameterInt("modesearch"));

    // Compute the margin used to ensure exact results (tile wise smoothing)
    // This margin is valid for the default uniform kernel used by the
    // MeanShiftSmoothing filter (bandwidth equal to radius in this case)
    const unsigned int margin = (filter->GetMaxIterationNumber() * filter->GetSpatialBandwidth()) + 1;

    otbAppLogINFO(<< "Margin of " << margin << " pixels applied to each tile to stabilized mean shift filtering." << std::endl);

    if (margin > std::min(input->GetLargestPossibleRegion().GetSize()[0], input->GetLargestPossibleRegion().GetSize()[1]))
    {
      otbAppLogWARNING(<< "Margin value exceed the input image size." << std::endl);
    }

    SetParameterOutputImage("fout", filter->GetOutput());
    if (IsParameterEnabled("foutpos") && HasValue("foutpos"))
    {
      SetParameterOutputImage("foutpos", filter->GetSpatialOutput());
    }
    if (!GetParameterInt("modesearch"))
    {
      otbAppLogINFO(<< "Mode Search is disabled." << std::endl);
    }
    RegisterPipeline();
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MeanShiftSmoothing)
