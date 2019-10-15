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


#include "itkComposeImageFilter.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryCrossStructuringElement.h"

#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbMorphologicalProfilesSegmentationFilter.h"
#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MorphologicalMultiScaleDecomposition : public Application
{
public:
  /** Standard class typedefs. */
  typedef MorphologicalMultiScaleDecomposition Self;
  typedef Application                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  typedef FloatVectorImageType::InternalPixelType InputVectorPixelType;

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, InputVectorPixelType> ExtractorFilterType;

  typedef itk::BinaryBallStructuringElement<InputVectorPixelType, 2>  BallStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<InputVectorPixelType, 2> CrossStructuringElementType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MorphologicalMultiScaleDecomposition, otb::Application);

private:
  void DoInit() override
  {
    SetName("MorphologicalMultiScaleDecomposition");
    SetDescription("Perform a geodesic morphology based image analysis on an input image channel");

    // Documentation
    SetDocLongDescription(
        "This application recursively apply geodesic decomposition. \n"
        "\n"
        "This algorithm is derived from the following publication:\n"
        "\n"
        "Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE: A new approach "
        "for the morphological segmentation of high resolution satellite imagery.\n"
        "IEEE Transactions on geoscience and remote sensing, vol. 39, NO. 2, "
        "February 2001, p. 309-320.\n"
        "\n"
        "It provides a geodesic decomposition of the input image, with the "
        "following scheme. Let :math:`f_0` denote the input image, :math:`\\stackrel{\\smile}{\\mu}_{N}(f)` denote the convex membership function, "
        ":math:`\\stackrel{\\frown}{\\mu}_{N}(f)` denote the concave membership function and :math:`\\psi_{N}(f)` denote the leveling function, for a given "
        "radius :math:`N` as defined in the documentation\n"
        "of the GeodesicMorphologyDecompositionImageFilter. Let :math:`[N_{1},\\ldots, N_{n}]` denote a range of increasing radius (or scales). The iterative "
        "decomposition is defined as follows:\n"
        "\n"
        ":math:`f_i` = :math:`\\psi_{N_i}(f_{i-1})` \n"
        "\n"
        ":math:`\\stackrel{\\frown}{f}_i` = :math:`\\stackrel{\\frown}{\\mu}_{N_i}(f_i)` \n"
        "\n"
        ":math:`\\stackrel{\\smile}{f}_i` = :math:`\\stackrel{\\smile}{\\mu}_{N_i}(f_i)` \n"
        "\n"
        "The :math:`\\stackrel{\\smile}{f}_{i}` and :math:`\\stackrel{\\frown}{f}_{i}` are membership function for the convex\n"
        "(resp. concave) objects whose size is comprised between :math:`N_{i-1}` and :math:`N_i`\n"
        "\n"
        "Output convex, concave and leveling images with B bands, where n is the number of levels.");

    SetDocLimitations("Generation of the multi scale decomposition is not streamable, pay attention to this fact when setting the number of iterating levels.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbGeodesicMorphologyDecompositionImageFilter class");

    AddDocTag(Tags::FeatureExtraction);
    AddDocTag("Morphology");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to be classified.");

    AddParameter(ParameterType_OutputImage, "outconvex", "Output Convex Image");
    SetParameterDescription("outconvex", "The output convex image with N bands");
    AddParameter(ParameterType_OutputImage, "outconcave", "Output Concave Image");
    SetParameterDescription("outconcave", "The output concave concave with N bands");
    AddParameter(ParameterType_OutputImage, "outleveling", "Output Image");
    SetParameterDescription("outleveling", "The output leveling image with N bands");

    AddParameter(ParameterType_Int, "channel", "Selected Channel");
    SetParameterDescription("channel", "The selected channel index for input image");
    SetDefaultParameterInt("channel", 1);
    SetMinimumParameterIntValue("channel", 1);

    // Strucring Element (Ball | Cross)
    AddParameter(ParameterType_Choice, "structype", "Structuring Element Type");
    SetParameterDescription("structype", "Choice of the structuring element type");
    AddChoice("structype.ball", "Ball");
    AddChoice("structype.cross", "Cross");

    AddParameter(ParameterType_Int, "radius", "Initial radius");
    SetParameterDescription("radius", "Initial radius of the structuring element (in pixels)");
    SetDefaultParameterInt("radius", 5);
    SetMinimumParameterIntValue("radius", 1);

    AddParameter(ParameterType_Int, "step", "Radius step");
    SetParameterDescription("step", "Radius step along the profile (in pixels)");
    SetDefaultParameterInt("step", 1);
    SetMinimumParameterIntValue("step", 1);

    AddParameter(ParameterType_Int, "levels", "Number of levels use for multi scale");
    SetParameterDescription("levels", "Number of levels use for multi scale");
    SetDefaultParameterInt("levels", 1);
    SetMinimumParameterIntValue("levels", 1);

    AddRAMParameter();

    SetDocExampleParameterValue("in", "ROI_IKO_PAN_LesHalles.tif");
    SetDocExampleParameterValue("structype", "ball");
    SetDocExampleParameterValue("channel", "1");
    SetDocExampleParameterValue("radius", "2");
    SetDocExampleParameterValue("levels", "2");
    SetDocExampleParameterValue("step", "3");
    SetDocExampleParameterValue("outconvex", "convex.tif");
    SetDocExampleParameterValue("outconcave", "concave.tif");
    SetDocExampleParameterValue("outleveling", "leveling.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    FloatVectorImageType::Pointer inImage         = GetParameterImage("in");
    int                           nBComp          = inImage->GetNumberOfComponentsPerPixel();
    int                           selectedChannel = GetParameterInt("channel");

    if (selectedChannel > nBComp)
    {
      itkExceptionMacro(<< "The specified channel index for input image is invalid.");
    }

    m_ExtractorFilter = ExtractorFilterType::New();
    m_ExtractorFilter->SetInput(inImage);
    m_ExtractorFilter->SetStartX(static_cast<unsigned int>(inImage->GetLargestPossibleRegion().GetIndex(0)));
    m_ExtractorFilter->SetStartY(static_cast<unsigned int>(inImage->GetLargestPossibleRegion().GetIndex(1)));
    m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
    m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
    m_ExtractorFilter->SetChannel(static_cast<unsigned int>(GetParameterInt("channel")));

    unsigned int numberOfLevels = static_cast<unsigned int>(GetParameterInt("levels"));
    unsigned int initValue      = static_cast<unsigned int>(GetParameterInt("radius"));
    unsigned int step           = static_cast<unsigned int>(GetParameterInt("step"));


    if (GetParameterString("structype") == "ball")
    {
      performDecomposition<BallStructuringElementType>(numberOfLevels, step, initValue);
    }
    else // Cross
    {
      performDecomposition<CrossStructuringElementType>(numberOfLevels, step, initValue);
    }
  }

  template <typename StructuringElement>
  void performDecomposition(unsigned int numberOfLevels, unsigned int step, unsigned int initValue)
  {

    typedef otb::GeodesicMorphologyIterativeDecompositionImageFilter<FloatImageType, StructuringElement> TDecompositionImageFilter;
    typedef typename TDecompositionImageFilter::OutputImageListType TImageList;
    typedef otb::ImageListToVectorImageFilter<TImageList, FloatVectorImageType> TListToVectorImageFilter;

    typename TDecompositionImageFilter::Pointer decompositionImageFilter;
    decompositionImageFilter = TDecompositionImageFilter::New();
    decompositionImageFilter->SetInput(m_ExtractorFilter->GetOutput());
    decompositionImageFilter->SetNumberOfIterations(numberOfLevels);
    decompositionImageFilter->SetInitialValue(initValue);
    decompositionImageFilter->SetStep(step);
    AddProcess(decompositionImageFilter, "Image Decomposition");
    decompositionImageFilter->Update();

    typename TListToVectorImageFilter::Pointer levelingListToVectorImageFilter = TListToVectorImageFilter::New();
    typename TListToVectorImageFilter::Pointer concaveListToVectorImageFilter  = TListToVectorImageFilter::New();
    typename TListToVectorImageFilter::Pointer convexListToVectorImageFilter   = TListToVectorImageFilter::New();

    levelingListToVectorImageFilter->SetInput(decompositionImageFilter->GetOutput());
    levelingListToVectorImageFilter->Update();
    SetParameterOutputImage("outleveling", levelingListToVectorImageFilter->GetOutput());

    concaveListToVectorImageFilter->SetInput(decompositionImageFilter->GetConcaveOutput());
    concaveListToVectorImageFilter->Update();
    SetParameterOutputImage("outconcave", concaveListToVectorImageFilter->GetOutput());

    convexListToVectorImageFilter->SetInput(decompositionImageFilter->GetConvexOutput());
    convexListToVectorImageFilter->Update();
    SetParameterOutputImage("outconvex", convexListToVectorImageFilter->GetOutput());
  }

  ExtractorFilterType::Pointer m_ExtractorFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MorphologicalMultiScaleDecomposition)
