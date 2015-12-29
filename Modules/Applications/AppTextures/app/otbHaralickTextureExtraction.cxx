/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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

void DoInit()
{
SetName("HaralickTextureExtraction");
SetDescription("Computes textures on every pixel of the input image selected channel");

// Documentation
SetDocName("Haralick Texture Extraction");
SetDocLongDescription("This application computes Haralick, advanced and higher order textures on a mono band image");
SetDocLimitations("None");
SetDocAuthors("OTB-Team");
SetDocSeeAlso("otbScalarImageToTexturesFilter, otbScalarImageToAdvancedTexturesFilter and otbScalarImageToHigherOrderTexturesFilter classes");

AddDocTag("Textures");
AddDocTag(Tags::FeatureExtraction);

AddParameter(ParameterType_InputImage, "in",  "Input Image");
SetParameterDescription("in", "The input image to compute the features on.");

AddParameter(ParameterType_Int,  "channel",  "Selected Channel");
SetParameterDescription("channel", "The selected channel index");
SetDefaultParameterInt("channel", 1);
SetMinimumParameterIntValue("channel", 1);

AddRAMParameter();

AddParameter(ParameterType_Group, "parameters", "Texture feature parameters");
SetParameterDescription("parameters","This group of parameters allows one to define texture parameters.");

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
SetParameterDescription("texture.simple","This group of parameters defines the 8 local Haralick texture feature output image.\
    The image channels are: Energy, Entropy, Correlation, Inverse Difference Moment,\
    Inertia, Cluster Shade, Cluster Prominence and Haralick Correlation");

AddChoice("texture.advanced", "Advanced Texture Features");
SetParameterDescription("texture.advanced","This group of parameters defines the 9 advanced texture feature output image.\
    The image channels are: Mean, Variance, Sum Average, Sum Variance,\
    Sum Entropy, Difference of Entropies, Difference of Variances, IC1 and IC2");

AddChoice("texture.higher", "Higher Order Texture Features");
SetParameterDescription("texture.higher","This group of parameters defines the 11 higher order texture feature output image.\
    The image channels are: Short Run Emphasis, Long Run Emphasis, Grey-Level Nonuniformity, Run Length Nonuniformity, Run Percentage, \
    Low Grey-Level Run Emphasis, High Grey-Level Run Emphasis, Short Run Low Grey-Level Emphasis, Short Run High Grey-Level Emphasis, \
    Long Run Low Grey-Level Emphasis and Long Run High Grey-Level Emphasis");

AddParameter(ParameterType_OutputImage, "out", "Output Image");
SetParameterDescription("out", "Output image containing the selected texture features.");
MandatoryOff("out");
EnableParameter("out");


// Doc example parameter settings
SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
SetDocExampleParameterValue("channel", "2");
SetDocExampleParameterValue("parameters.xrad", "3");
SetDocExampleParameterValue("parameters.yrad", "3");
SetDocExampleParameterValue("texture", "simple");
SetDocExampleParameterValue("out", "HaralickTextures.tif");
}

void DoUpdateParameters()
{
  // Nothing to do here : all parameters are independent
}

void DoExecute()
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
    m_HigImageList->PushBack(m_HigTexFilter->GetShortRunEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetLongRunEmphasisOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetGreyLevelNonuniformityOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetRunLengthNonuniformityOutput());
    m_HigImageList->PushBack(m_HigTexFilter->GetRunPercentageOutput());
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
