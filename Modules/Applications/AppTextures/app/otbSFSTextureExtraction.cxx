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

#include "otbSFSTexturesImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "itkTimeProbe.h"

namespace otb
{
namespace Wrapper
{

class SFSTextureExtraction : public Application
{
public:
/** Standard class typedefs. */
typedef SFSTextureExtraction          Self;
typedef Application                   Superclass;
typedef itk::SmartPointer<Self>       Pointer;
typedef itk::SmartPointer<const Self> ConstPointer;

typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,FloatVectorImageType::InternalPixelType>
ExtractorFilterType;

typedef SFSTexturesImageFilter<FloatImageType, FloatImageType>                 SFSTexturesFilterType;
typedef ImageList<FloatImageType>                                              ImageListType;
typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType>      ImageListToVectorImageFilterType;

/** Standard macro */
itkNewMacro(Self);

itkTypeMacro(SFSTextureExtraction, otb::Application);

private:

void DoInit()
{
SetName("SFSTextureExtraction");
SetDescription("Computes Structural Feature Set textures on every pixel of the input image selected channel");

// Documentation
SetDocName("SFS Texture Extraction");
SetDocLongDescription("This application computes SFS textures on a mono band image");
SetDocLimitations("None");
SetDocAuthors("OTB-Team");
SetDocSeeAlso("otbSFSTexturesImageFilter class");

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
SetParameterDescription("parameters","This group of parameters allows one to define SFS texture parameters.\
    The available texture features are SFS'Length, SFS'Width, SFS'PSI, SFS'W-Mean, SFS'Ratio and SFS'SD.\
    They are provided in this exact order in the output image.");

AddParameter(ParameterType_Float,"parameters.spethre","Spectral Threshold");
SetParameterDescription("parameters.spethre", "Spectral Threshold");
SetDefaultParameterFloat("parameters.spethre", 50.0);

AddParameter(ParameterType_Int,"parameters.spathre","Spatial Threshold");
SetParameterDescription("parameters.spathre", "Spatial Threshold");
SetDefaultParameterInt("parameters.spathre", 100);

AddParameter(ParameterType_Int,"parameters.nbdir","Number of Direction");
SetParameterDescription("parameters.nbdir", "Number of Direction");
SetDefaultParameterInt("parameters.nbdir", 20);

AddParameter(ParameterType_Float,"parameters.alpha","Alpha");
SetParameterDescription("parameters.alpha", "Alpha");
SetDefaultParameterFloat("parameters.alpha", 1.0);

AddParameter(ParameterType_Int,"parameters.maxcons","Ratio Maximum Consideration Number");
SetParameterDescription("parameters.maxcons", "Ratio Maximum Consideration Number");
SetDefaultParameterInt("parameters.maxcons", 5);

AddParameter(ParameterType_OutputImage, "out", "Feature Output Image");
SetParameterDescription("out", "Output image containing the SFS texture features.");

// Doc example parameter settings
SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
SetDocExampleParameterValue("channel", "1");
SetDocExampleParameterValue("parameters.spethre", "50.0");
SetDocExampleParameterValue("parameters.spathre", "100");
SetDocExampleParameterValue("out", "SFSTextures.tif");
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

  m_ExtractorFilter = ExtractorFilterType::New();
  m_ExtractorFilter->SetInput(inImage);
  m_ExtractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
  m_ExtractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
  m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
  m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
  m_ExtractorFilter->SetChannel(GetParameterInt("channel"));
  m_ExtractorFilter->UpdateOutputInformation();

  m_TexFilter    = SFSTexturesFilterType::New();
  m_ImageList    = ImageListType::New();
  m_Concatener   = ImageListToVectorImageFilterType::New();


  m_TexFilter->SetInput(const_cast<FloatImageType*>(m_ExtractorFilter->GetOutput()));
  m_TexFilter->SetSpectralThreshold(GetParameterFloat("parameters.spethre"));
  m_TexFilter->SetSpatialThreshold(GetParameterInt("parameters.spathre"));
  m_TexFilter->SetNumberOfDirections(GetParameterInt("parameters.nbdir"));
  m_TexFilter->SetAlpha(GetParameterFloat("parameters.alpha"));
  m_TexFilter->SetRatioMaxConsiderationNumber(GetParameterInt("parameters.maxcons"));
  m_TexFilter->SetFeatureStatus(SFSTexturesFilterType::LENGTH, true);
  m_TexFilter->SetFeatureStatus(SFSTexturesFilterType::WIDTH, true);
  m_TexFilter->SetFeatureStatus(SFSTexturesFilterType::PSI, true);
  m_TexFilter->SetFeatureStatus(SFSTexturesFilterType::WMEAN, true);
  m_TexFilter->SetFeatureStatus(SFSTexturesFilterType::RATIO, true);
  m_TexFilter->SetFeatureStatus(SFSTexturesFilterType::SD, true);
  m_TexFilter->UpdateOutputInformation();

  m_ImageList->PushBack(m_TexFilter->GetLengthOutput());
  m_ImageList->PushBack(m_TexFilter->GetWidthOutput());
  m_ImageList->PushBack(m_TexFilter->GetPSIOutput());
  m_ImageList->PushBack(m_TexFilter->GetWMeanOutput());
  m_ImageList->PushBack(m_TexFilter->GetRatioOutput());
  m_ImageList->PushBack(m_TexFilter->GetSDOutput());
  m_Concatener->SetInput(m_ImageList);
  SetParameterOutputImage("out", m_Concatener->GetOutput());

}
ExtractorFilterType::Pointer m_ExtractorFilter;

SFSTexturesFilterType::Pointer            m_TexFilter;
ImageListType::Pointer                    m_ImageList;
ImageListToVectorImageFilterType::Pointer m_Concatener;

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SFSTextureExtraction)
