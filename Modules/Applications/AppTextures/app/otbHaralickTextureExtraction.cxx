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

#include "otbScalarImageToTexturesFilter.h"
#include "otbScalarImageToAdvancedTexturesFilter.h"
#include "otbScalarImageToHigherOrderTexturesFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbClampImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class HaralickTextureExtraction : public Application
{
public:
/** Standard class typedefs. */
typedef HaralickTextureExtraction     Self;
typedef Application                   Superclass;
typedef itk::SmartPointer<Self>       Pointer;
typedef itk::SmartPointer<const Self> ConstPointer;

typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,FloatVectorImageType::InternalPixelType>
                                                                               ExtractorFilterType;
typedef ClampImageFilter<FloatImageType, FloatImageType>                       ClampFilterType;

typedef ScalarImageToTexturesFilter<FloatImageType, FloatImageType>            HarTexturesFilterType;
typedef ScalarImageToAdvancedTexturesFilter<FloatImageType, FloatImageType>    AdvTexturesFilterType;
typedef ScalarImageToHigherOrderTexturesFilter<FloatImageType, FloatImageType> HigTexturesFilterType;

typedef HarTexturesFilterType::SizeType                                        RadiusType;
typedef HarTexturesFilterType::OffsetType                                      OffsetType;

typedef ImageList<FloatImageType>                                              ImageListType;
typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType>      ImageListToVectorImageFilterType;


/** Standard macro */
itkNewMacro(Self);

itkTypeMacro(HaralickTextureExtraction, otb::Application);

private:

void DoInit() override
{
SetName("HaralickTextureExtraction");
SetDescription("Computes Haralick textural features on the selected channel of the input image");

// Documentation
SetDocName("Haralick Texture Extraction");
SetDocLongDescription(
    "This application computes three sets of Haralick features [1][2].\n\n"

    "* simple: a set of 8 local Haralick features: Energy (texture uniformity) , "
    "Entropy (measure of randomness of intensity image), Correlation (how "
    "correlated a pixel is to its neighborhood), Inverse Difference Moment (measures "
    "the texture homogeneity), Inertia (intensity contrast between a pixel and its "
    "neighborhood), Cluster Shade, Cluster Prominence, Haralick Correlation;\n\n"
    "* advanced: a set of 10 advanced Haralick features: Mean, Variance (measures the "
    "texture heterogeneity), Dissimilarity, Sum Average, Sum Variance, Sum Entropy, "
    "Difference of Entropies, Difference of Variances, IC1, IC2;\n\n"
    "* higher: a set of 11 higher Haralick features: Short Run Emphasis (measures the "
    "texture sharpness), Long Run Emphasis (measures the texture roughness), Grey-Level "
    "Nonuniformity, Run Length Nonuniformity, Run Percentage (measures the texture "
    "sharpness homogeneity), Low Grey-Level Run Emphasis, High Grey-Level Run Emphasis, "
    "Short Run Low Grey-Level Emphasis, Short Run High Grey-Level Emphasis, Long Run Low "
    "Grey-Level Emphasis and Long Run High Grey-Level Emphasis.\n\n"
    "The documentation of textures (description are formulas) are available in the OTB Software Guide (section Textures) and also in corresponding doxygen "
    "documentation of filters (see section *Detailed Description* in doxygen):\n"
    "* **simple**: otbScalarImageToTexturesFilter\n"
    "* **advanced**: otbScalarImageToAdvancedTexturesFilter\n"
    "* **higher**: otbScalarImageToHigherOrderTexturesFilter");

SetDocLimitations("The computation of the features is based on a Gray Level Co-occurrence "
    "matrix (GLCM) from the quantized input image. Consequently the quantization "
    "parameters (min, max, nbbin) must be appropriate to the range of the pixel values.");
SetDocAuthors("OTB-Team");
SetDocSeeAlso("[1] HARALICK, Robert M., SHANMUGAM, Karthikeyan, et al. "
    "Textural features for image classification. IEEE Transactions on systems, "
    "man, and cybernetics, 1973, no 6, p. 610-621.\n"
    "[2] otbScalarImageToTexturesFilter, otbScalarImageToAdvancedTexturesFilter and "
    "otbScalarImageToHigherOrderTexturesFilter classes");

AddDocTag(Tags::FeatureExtraction);
AddDocTag("Textures");

AddParameter(ParameterType_InputImage, "in",  "Input Image");
SetParameterDescription("in", "The input image to compute the features on.");

AddParameter(ParameterType_Int,  "channel",  "Selected Channel");
SetParameterDescription("channel", "The selected channel index");
SetDefaultParameterInt("channel", 1);
SetMinimumParameterIntValue("channel", 1);

AddParameter(ParameterType_Int, "step", "Computation step");
SetParameterDescription("step", "Step (in pixels) to compute output texture values."
  " The first computed pixel position is shifted by (step-1)/2 in both directions.");
SetDefaultParameterInt("step", 1);
SetMinimumParameterIntValue("step", 1);
MandatoryOff("step");

AddParameter(ParameterType_Group, "parameters", "Texture feature parameters");
SetParameterDescription("parameters","This group of parameters allows one to define "
    "texture parameters.");

AddParameter(ParameterType_Int,"parameters.xrad","X Radius");
SetParameterDescription("parameters.xrad", "X Radius");
SetDefaultParameterInt("parameters.xrad", 2);

AddParameter(ParameterType_Int,"parameters.yrad","Y Radius");
SetParameterDescription("parameters.yrad", "Y Radius");
SetDefaultParameterInt("parameters.yrad", 2);

AddParameter(ParameterType_Int,"parameters.xoff","X Offset");
SetParameterDescription("parameters.xoff", "X Offset");
SetDefaultParameterInt("parameters.xoff", 1);

AddParameter(ParameterType_Int,"parameters.yoff","Y Offset");
SetParameterDescription("parameters.yoff", "Y Offset");
SetDefaultParameterInt("parameters.yoff", 1);

AddParameter(ParameterType_Float,"parameters.min","Image Minimum");
SetParameterDescription("parameters.min", "Image Minimum");
SetDefaultParameterFloat("parameters.min", 0);

AddParameter(ParameterType_Float,"parameters.max","Image Maximum");
SetParameterDescription("parameters.max", "Image Maximum");
SetDefaultParameterFloat("parameters.max", 255);

AddParameter(ParameterType_Int,"parameters.nbbin","Histogram number of bin");
SetParameterDescription("parameters.nbbin", "Histogram number of bin");
SetDefaultParameterInt("parameters.nbbin", 8);

AddParameter(ParameterType_Choice, "texture", "Texture Set Selection");
SetParameterDescription("texture", "Choice of The Texture Set");

AddChoice("texture.simple", "Simple Haralick Texture Features");
SetParameterDescription("texture.simple", "This group of parameters defines "
    "the 8 local Haralick texture feature output image. The image channels are: "
    "Energy, Entropy, Correlation, Inverse Difference Moment, Inertia, Cluster "
    "Shade, Cluster Prominence and Haralick Correlation");

AddChoice("texture.advanced", "Advanced Texture Features");
SetParameterDescription("texture.advanced", "This group of parameters defines "
    "the 10 advanced texture feature output image. The image channels are: Mean, "
    "Variance, Dissimilarity, Sum Average, Sum Variance, Sum Entropy, Difference "
    "of Entropies, Difference of Variances, IC1 and IC2");

AddChoice("texture.higher", "Higher Order Texture Features");
SetParameterDescription("texture.higher", "This group of parameters defines the "
    "11 higher order texture feature output image. The image channels are: "
    "Short Run Emphasis, Long Run Emphasis, Grey-Level Nonuniformity, "
    "Run Length Nonuniformity, Run Percentage, Low Grey-Level Run Emphasis, "
    "High Grey-Level Run Emphasis, Short Run Low Grey-Level Emphasis, "
    "Short Run High Grey-Level Emphasis, Long Run Low Grey-Level Emphasis and "
    "Long Run High Grey-Level Emphasis");

AddParameter(ParameterType_OutputImage, "out", "Output Image");
SetParameterDescription("out", "Output image containing the selected texture features.");
MandatoryOff("out");
EnableParameter("out");

AddRAMParameter();

// Doc example parameter settings
SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
SetDocExampleParameterValue("channel", "2");
SetDocExampleParameterValue("parameters.xrad", "3");
SetDocExampleParameterValue("parameters.yrad", "3");
SetDocExampleParameterValue("texture", "simple");
SetDocExampleParameterValue("out", "HaralickTextures.tif");

SetOfficialDocLink();
}

void DoUpdateParameters() override
{
  // Nothing to do here : all parameters are independent
}

void DoExecute() override
{
  FloatVectorImageType::Pointer inImage = GetParameterImage("in");
  inImage->UpdateOutputInformation();
  int nBComp = inImage->GetNumberOfComponentsPerPixel();

  if( GetParameterInt("channel") > nBComp )
    {
    itkExceptionMacro(<< "The specified channel index is invalid.");
    }

  const std::string texType = GetParameterString("texture");

  RadiusType radius;
  radius[0] = GetParameterInt("parameters.xrad");
  radius[1] = GetParameterInt("parameters.yrad");

  OffsetType offset;
  offset[0] = GetParameterInt("parameters.xoff");
  offset[1] = GetParameterInt("parameters.yoff");

  RadiusType stepping;
  stepping.Fill(GetParameterInt("step"));

  OffsetType stepOffset;
  stepOffset.Fill((GetParameterInt("step") - 1) / 2);

  m_ExtractorFilter = ExtractorFilterType::New();
  m_ExtractorFilter->SetInput(inImage);
  m_ExtractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
  m_ExtractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
  m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
  m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
  m_ExtractorFilter->SetChannel(GetParameterInt("channel"));
  m_ExtractorFilter->UpdateOutputInformation();

  m_ClampFilter = ClampFilterType::New();
  m_ClampFilter->SetInput(m_ExtractorFilter->GetOutput());
  m_ClampFilter->SetLower(GetParameterFloat("parameters.min"));
  m_ClampFilter->SetUpper(GetParameterFloat("parameters.max"));

  m_HarTexFilter    = HarTexturesFilterType::New();
  m_HarImageList    = ImageListType::New();
  m_HarConcatener   = ImageListToVectorImageFilterType::New();

  m_AdvTexFilter  = AdvTexturesFilterType::New();
  m_AdvImageList  = ImageListType::New();
  m_AdvConcatener = ImageListToVectorImageFilterType::New();

  m_HigTexFilter  = HigTexturesFilterType::New();
  m_HigImageList  = ImageListType::New();
  m_HigConcatener = ImageListToVectorImageFilterType::New();

  if( texType == "simple" )
    {
    m_HarTexFilter->SetInput(const_cast<FloatImageType*>(m_ClampFilter->GetOutput()));
    m_HarTexFilter->SetRadius(radius);
    m_HarTexFilter->SetOffset(offset);
    m_HarTexFilter->SetInputImageMinimum(GetParameterFloat("parameters.min"));
    m_HarTexFilter->SetInputImageMaximum(GetParameterFloat("parameters.max"));
    m_HarTexFilter->SetNumberOfBinsPerAxis(GetParameterInt("parameters.nbbin"));
    m_HarTexFilter->SetSubsampleFactor(stepping);
    m_HarTexFilter->SetSubsampleOffset(stepOffset);
    m_HarTexFilter->UpdateOutputInformation();
    m_HarImageList->PushBack(m_HarTexFilter->GetEnergyOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetEntropyOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetCorrelationOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetInverseDifferenceMomentOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetInertiaOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetClusterShadeOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetClusterProminenceOutput());
    m_HarImageList->PushBack(m_HarTexFilter->GetHaralickCorrelationOutput());
    m_HarConcatener->SetInput(m_HarImageList);
    SetParameterOutputImage("out", m_HarConcatener->GetOutput());
    }

  if( texType == "advanced" )
    {
    m_AdvTexFilter->SetInput(const_cast<FloatImageType*>(m_ClampFilter->GetOutput()));
    m_AdvTexFilter->SetRadius(radius);
    m_AdvTexFilter->SetOffset(offset);
    m_AdvTexFilter->SetInputImageMinimum(GetParameterFloat("parameters.min"));
    m_AdvTexFilter->SetInputImageMaximum(GetParameterFloat("parameters.max"));
    m_AdvTexFilter->SetNumberOfBinsPerAxis(GetParameterInt("parameters.nbbin"));
    m_AdvTexFilter->SetSubsampleFactor(stepping);
    m_AdvTexFilter->SetSubsampleOffset(stepOffset);
    m_AdvImageList->PushBack(m_AdvTexFilter->GetMeanOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetVarianceOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetDissimilarityOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetSumAverageOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetSumVarianceOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetSumEntropyOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetDifferenceEntropyOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetDifferenceVarianceOutput());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetIC1Output());
    m_AdvImageList->PushBack(m_AdvTexFilter->GetIC2Output());
    m_AdvConcatener->SetInput(m_AdvImageList);
    SetParameterOutputImage("out", m_AdvConcatener->GetOutput());
    }

  if( texType == "higher" )
    {
    m_HigTexFilter->SetInput(const_cast<FloatImageType*>(m_ClampFilter->GetOutput()));
    m_HigTexFilter->SetRadius(radius);
    m_HigTexFilter->SetOffset(offset);
    m_HigTexFilter->SetInputImageMinimum(GetParameterFloat("parameters.min"));
    m_HigTexFilter->SetInputImageMaximum(GetParameterFloat("parameters.max"));
    m_HigTexFilter->SetNumberOfBinsPerAxis(GetParameterInt("parameters.nbbin"));
    m_HigTexFilter->SetSubsampleFactor(stepping);
    m_HigTexFilter->SetSubsampleOffset(stepOffset);
    m_HigImageList->PushBack(m_HigTexFilter->GetShortRunEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetLongRunEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetGreyLevelNonuniformityOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetRunLengthNonuniformityOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetLowGreyLevelRunEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetHighGreyLevelRunEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetShortRunLowGreyLevelEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetShortRunHighGreyLevelEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetLongRunLowGreyLevelEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetLongRunHighGreyLevelEmphasisOutput());
    m_HigConcatener->SetInput(m_HigImageList);
    SetParameterOutputImage("out", m_HigConcatener->GetOutput());
    }

}
ExtractorFilterType::Pointer m_ExtractorFilter;
ClampFilterType::Pointer     m_ClampFilter;

HarTexturesFilterType::Pointer            m_HarTexFilter;
ImageListType::Pointer                    m_HarImageList;
ImageListToVectorImageFilterType::Pointer m_HarConcatener;
AdvTexturesFilterType::Pointer            m_AdvTexFilter;
ImageListType::Pointer                    m_AdvImageList;
ImageListToVectorImageFilterType::Pointer m_AdvConcatener;
HigTexturesFilterType::Pointer            m_HigTexFilter;
ImageListType::Pointer                    m_HigImageList;
ImageListToVectorImageFilterType::Pointer m_HigConcatener;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::HaralickTextureExtraction)
