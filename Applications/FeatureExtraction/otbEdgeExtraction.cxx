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

#include "itkGradientMagnitudeImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"
//#include "itkCannyEdgeDetectionImageFilter.h"
#include "otbTouziEdgeDetectorImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Wrapper
{

class EdgeExtraction : public Application
{
public:
/** Standard class typedefs. */
typedef EdgeExtraction                Self;
typedef Application                   Superclass;
typedef itk::SmartPointer<Self>       Pointer;
typedef itk::SmartPointer<const Self> ConstPointer;

typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,FloatImageType::InternalPixelType>
                                                    ExtractorFilterType;
typedef itk::GradientMagnitudeImageFilter<FloatImageType, FloatImageType>
                                                    GradientFilterType;
typedef itk::SobelEdgeDetectionImageFilter<FloatImageType, FloatImageType>
                                                    SobelFilterType;
//typedef itk::CannyEdgeDetectionImageFilter<FloatImageType, FloatImageType>
//                                                    CannyFilterType;
typedef TouziEdgeDetectorImageFilter<FloatImageType, FloatImageType>
                                                    TouziFilterType;

/** Standard macro */
itkNewMacro(Self);

itkTypeMacro(EdgeExtraction, otb::Application);

private:

void DoInit()
{
SetName("EdgeExtraction");
SetDescription("Computes edge features on every pixel of the input image selected channel");

// Documentation
SetDocName("Edge Feature Extraction");
SetDocLongDescription("This application computes edge features on a mono band image");
SetDocLimitations("None");
SetDocAuthors("OTB-Team");
SetDocSeeAlso("otb class");

AddDocTag("Edge");
AddDocTag(Tags::FeatureExtraction);

AddParameter(ParameterType_InputImage, "in",  "Input Image");
SetParameterDescription("in", "The input image to compute the features on.");

AddParameter(ParameterType_Int,  "channel",  "Selected Channel");
SetParameterDescription("channel", "The selected channel index");
SetDefaultParameterInt("channel", 1);
SetMinimumParameterIntValue("channel", 1);

AddRAMParameter();

AddParameter(ParameterType_Choice, "filter", "Edge feature");
SetParameterDescription("filter", "Choice of edge feature");

//Gradient Section
AddChoice("filter.gradient", "Gradient");

//Sobel Section
AddChoice("filter.sobel", "Sobel");

//Touzi Section
AddChoice("filter.touzi", "Touzi");
AddParameter(ParameterType_Int, "filter.touzi.xradius", "The X Radius");
SetDefaultParameterInt("filter.touzi.xradius", 1);
AddParameter(ParameterType_Int, "filter.touzi.yradius", "The Y Radius");
SetDefaultParameterInt("filter.touzi.yradius", 1);

//Canny Section
/*
AddChoice("filter.canny", "Canny");
AddParameter(ParameterType_Float, "filter.canny.variance", "");
SetDefaultParameterFloat("filter.canny.variance", 1.0);
AddParameter(ParameterType_Float, "filter.canny.maximumerror", "");
SetDefaultParameterFloat("filter.canny.maximumerror", 0.01);
AddParameter(ParameterType_Float, "filter.canny.upperthreshold", "");
SetDefaultParameterFloat("filter.canny.upperthreshold", 15.0);
AddParameter(ParameterType_Float, "filter.canny.lowerthreshold", "");
SetDefaultParameterFloat("filter.canny.lowerthreshold", 3.0);
*/

AddParameter(ParameterType_OutputImage, "out", "Feature Output Image");
SetParameterDescription("out", "Output image containing the local statistic features.");

// Doc example parameter settings
SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
SetDocExampleParameterValue("channel", "1");
SetDocExampleParameterValue("out", "Edges.tif");
}

void DoUpdateParameters()
{
  // Nothing to do here : all parameters are independent
}

void DoExecute()
{
  FloatVectorImageType::Pointer inImage = GetParameterImage("in");
  inImage->UpdateOutputInformation();

  if( GetParameterInt("channel") > inImage->GetNumberOfComponentsPerPixel() )
    {
    otbAppLogCRITICAL("Selected band is not available...");
    return;
    }

  const std::string edgeType = GetParameterString("filter");

  m_ExtractorFilter = ExtractorFilterType::New();
  m_ExtractorFilter->SetInput(inImage);
  m_ExtractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
  m_ExtractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
  m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
  m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
  m_ExtractorFilter->SetChannel(GetParameterInt("channel"));
  m_ExtractorFilter->UpdateOutputInformation();

  m_GradientFilter    = GradientFilterType::New();
  m_SobelFilter       = SobelFilterType::New();
  m_TouziFilter       = TouziFilterType::New();
  //m_CannyFilter       = CannyFilterType::New();

  if( edgeType == "gradient" )
    {
    m_GradientFilter->SetInput(const_cast<FloatImageType*>(m_ExtractorFilter->GetOutput()));
    SetParameterOutputImage("out", m_GradientFilter->GetOutput());
    }

  if( edgeType == "sobel" )
    {
    m_SobelFilter->SetInput(const_cast<FloatImageType*>(m_ExtractorFilter->GetOutput()));
    SetParameterOutputImage("out", m_SobelFilter->GetOutput());
    }

  if( edgeType == "touzi" )
    {
    m_TouziFilter->SetInput(const_cast<FloatImageType*>(m_ExtractorFilter->GetOutput()));
    TouziFilterType::SizeType rad;
    rad[0] = this->GetParameterInt("filter.touzi.xradius");
    rad[1] = this->GetParameterInt("filter.touzi.yradius");
    m_TouziFilter->SetRadius(rad);
    SetParameterOutputImage("out", m_TouziFilter->GetOutput());
    }
/*
  if( edgeType == "canny" )
    {

    m_CannyFilter->SetInput(const_cast<FloatImageType*>(m_ExtractorFilter->GetOutput()));
    m_CannyFilter->SetVariance(GetParameterFloat("filter.canny.variance"));
    m_CannyFilter->SetMaximumError(GetParameterFloat("filter.canny.maximumerror"));
    m_CannyFilter->SetUpperThreshold(GetParameterFloat("filter.canny.upperthreshold"));
    m_CannyFilter->SetLowerThreshold(GetParameterFloat("filter.canny.lowerthreshold"));

    SetParameterOutputImage("out", m_CannyFilter->GetOutput());
    }
*/
}

ExtractorFilterType::Pointer m_ExtractorFilter;
GradientFilterType::Pointer  m_GradientFilter;
SobelFilterType::Pointer     m_SobelFilter;
TouziFilterType::Pointer     m_TouziFilter;
//CannyFilterType::Pointer     m_CannyFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::EdgeExtraction)
