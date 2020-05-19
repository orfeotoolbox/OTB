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


#include "otbGeodesicMorphologyDecompositionImageFilter.h"
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

namespace otb
{
namespace Wrapper
{

class MorphologicalClassification : public Application
{
public:
  /** Standard class typedefs. */
  typedef MorphologicalClassification   Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType::InternalPixelType InputPixelType;
  typedef otb::Image<InputPixelType, 2> FloatImageType;

  typedef unsigned short LabeledPixelType;
  typedef otb::Image<LabeledPixelType, 2> LabeledImageType;

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, InputPixelType> ExtractorFilterType;

  typedef otb::ConvexOrConcaveClassificationFilter<FloatImageType, LabeledImageType> ClassificationFilterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, 2>  BallStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<InputPixelType, 2> CrossStructuringElementType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MorphologicalClassification, otb::Application);

private:
  void DoInit() override
  {
    SetName("MorphologicalClassification");
    SetDescription(
        "Performs morphological convex, concave and flat "
        "classification on an input image channel");

    // Documentation
    SetDocLongDescription(
        "This algorithm is based on the following publication:\n"
        "Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE: A new approach "
        "for the morphological segmentation of high resolution satellite imagery.\n"
        "IEEE Transactions on geoscience and remote sensing, vol. 39, NO. 2, "
        "February 2001, p. 309-320.\n"
        "\n"
        "This application perform the following decision rule to classify a pixel "
        "between the three classes Convex, Concave and Flat. Let :math:`f` denote "
        "the input image and :math:`\\psi_{N}(f)` the geodesic leveling of "
        ":math:`f` with a structuring element of size :math:`N`. One can derive "
        "the following decision rule to classify :math:`f` into Convex (label "
        ":math:`\\stackrel{\\smile}{k}`), Concave (label "
        ":math:`\\stackrel{\\frown}{k}`) and Flat (label :math:`\\bar{k}`):  "
        "\n"
        ":math:`f(n) = \\begin{cases} \\stackrel{\\smile}{k} & : f-\\psi_{N}(f)>\\sigma \\\\ \\stackrel{\\frown}{k} & : \\psi_{N}(f)-f>\\sigma \\\\ \\bar{k} & "
        ": \\mid f - \\psi_{N}(f) \\mid \\leq \\sigma \\end{cases}`"
        "\n\n"
        "The output is a labeled image (0: Flat, 1: Convex, 2: Concave)");
    SetDocLimitations(
        "Generation of the morphological classification is not streamable, "
        "pay attention to this fact when setting the radius size of the "
        "structuring element.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbConvexOrConcaveClassificationFilter class");

    AddDocTag(Tags::FeatureExtraction);
    AddDocTag("Morphology");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to be classified.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "The output classified image with 3 different values (0: Flat, 1: Convex, 2: Concave)");

    AddParameter(ParameterType_Int, "channel", "Selected Channel");
    SetParameterDescription("channel", "The selected channel index for input image");
    SetDefaultParameterInt("channel", 1);
    SetMinimumParameterIntValue("channel", 1);

    // Structuring Element (Ball | Cross)
    AddParameter(ParameterType_Choice, "structype", "Structuring Element Type");
    SetParameterDescription("structype", "Choice of the structuring element type");
    AddChoice("structype.ball", "Ball");
    AddChoice("structype.cross", "Cross");

    AddParameter(ParameterType_Int, "radius", "Radius");
    SetParameterDescription("radius", "Radius of the structuring element (in pixels), default value is 5.");
    SetDefaultParameterInt("radius", 5);
    SetMinimumParameterIntValue("radius", 1);

    AddParameter(ParameterType_Float, "sigma", "Sigma value for leveling tolerance");
    SetParameterDescription("sigma", "Sigma value for leveling tolerance, default value is 0.5.");
    SetDefaultParameterFloat("sigma", 0.5);
    SetMinimumParameterFloatValue("sigma", 0);

    AddRAMParameter();

    SetDocExampleParameterValue("in", "ROI_IKO_PAN_LesHalles.tif");
    SetDocExampleParameterValue("channel", "1");
    SetDocExampleParameterValue("structype", "ball");
    SetDocExampleParameterValue("radius", "5");
    SetDocExampleParameterValue("sigma", "0.5");
    SetDocExampleParameterValue("out", "output.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    int nBComp          = inImage->GetNumberOfComponentsPerPixel();
    int selectedChannel = GetParameterInt("channel");

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
    m_ExtractorFilter->SetChannel(static_cast<unsigned int>(selectedChannel));

    unsigned int sigma  = static_cast<unsigned int>(GetParameterInt("sigma"));
    unsigned int radius = static_cast<unsigned int>(GetParameterInt("radius"));

    m_ClassificationFilter = ClassificationFilterType::New();
    m_ClassificationFilter->SetInput(m_ExtractorFilter->GetOutput());
    m_ClassificationFilter->SetSigma(sigma);
    m_ClassificationFilter->SetFlatLabel(0);
    m_ClassificationFilter->SetConvexLabel(1);
    m_ClassificationFilter->SetConcaveLabel(2);

    if (GetParameterString("structype") == "ball")
    {
      performClassification<BallStructuringElementType>(radius);
    }
    else // Cross
    {
      performClassification<CrossStructuringElementType>(radius);
    }

    SetParameterOutputImage("out", m_ClassificationFilter->GetOutput());
  }

  template <typename TStructuringElement>
  void performClassification(unsigned int radius_size)
  {

    typedef otb::GeodesicMorphologyDecompositionImageFilter<FloatImageType, FloatImageType, TStructuringElement> TDecompositionImageFilter;

    typename TDecompositionImageFilter::Pointer decompositionImageFilter;
    decompositionImageFilter = TDecompositionImageFilter::New();
    decompositionImageFilter->SetInput(m_ExtractorFilter->GetOutput());

    typename TStructuringElement::RadiusType radius;
    radius.Fill(radius_size);
    decompositionImageFilter->SetRadius(radius);
    AddProcess(decompositionImageFilter, "Image Decomposition");
    decompositionImageFilter->Update();

    m_ClassificationFilter->SetInputLeveling(decompositionImageFilter->GetOutput());
  }

  ExtractorFilterType::Pointer      m_ExtractorFilter;
  ClassificationFilterType::Pointer m_ClassificationFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MorphologicalClassification)
