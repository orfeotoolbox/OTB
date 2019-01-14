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


#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkFlatStructuringElement.h"

#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkBinaryMorphologyImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class BinaryMorphologicalOperation : public Application
{
public:
/** Standard class typedefs. */
typedef BinaryMorphologicalOperation          Self;
typedef Application                   Superclass;
typedef itk::SmartPointer<Self>       Pointer;
typedef itk::SmartPointer<const Self> ConstPointer;

typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,FloatVectorImageType::InternalPixelType>
ExtractorFilterType;

typedef itk::FlatStructuringElement<2>                                         StructuringType;
typedef StructuringType::RadiusType                                            RadiusType;

typedef itk::BinaryDilateImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               DilateFilterType;
typedef itk::BinaryErodeImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               ErodeFilterType;
typedef itk::BinaryMorphologicalOpeningImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               OpeningFilterType;
typedef itk::BinaryMorphologicalClosingImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               ClosingFilterType;

typedef ImageList<FloatImageType>                                              ImageListType;
typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType>      ImageListToVectorImageFilterType;

/** Standard macro */
itkNewMacro(Self);
itkTypeMacro(BinaryMorphologicalOperation, otb::Application);

private:

void DoInit() override
{
SetName( "BinaryMorphologicalOperation" );
SetDescription( "Performs morphological operations on an input image channel" );

// Documentation
SetDocName( "Binary Morphological Operation" );
SetDocLongDescription( "This application performs binary morphological "
  "operations on a mono band image or a channel of the input." );
SetDocLimitations( "None" );
SetDocAuthors( "OTB-Team" );
SetDocSeeAlso( "itkBinaryDilateImageFilter, itkBinaryErodeImageFilter, "
  "itkBinaryMorphologicalOpeningImageFilter and "
  "itkBinaryMorphologicalClosingImageFilter classes." );

AddDocTag(Tags::FeatureExtraction);
AddDocTag("Morphology");

AddParameter( ParameterType_InputImage , "in" ,  "Input Image" );
SetParameterDescription( "in" , "The input image to be filtered." );

AddParameter( ParameterType_OutputImage , "out" , "Output Image" );
SetParameterDescription( "out" , "Output image" );

AddParameter( ParameterType_Int ,  "channel" ,  "Selected Channel" );
SetParameterDescription( "channel" , "The selected channel index" );
SetDefaultParameterInt( "channel" , 1 );
SetMinimumParameterIntValue( "channel" , 1 );

AddParameter( ParameterType_Choice , "structype" ,
 "Type of structuring element" );
SetParameterDescription( "structype" ,
  "Choice of the structuring element type");
AddParameter( ParameterType_Int, "xradius", "Structuring element X radius" );
SetParameterDescription( "xradius" , "The structuring element radius along the X axis." );
SetDefaultParameterInt( "xradius" , 5 );
AddParameter( ParameterType_Int, "yradius", "Structuring element Y radius" );
SetParameterDescription( "yradius" , "The structuring element radius along the Y axis." );
SetDefaultParameterInt( "yradius" , 5 );

AddChoice( "structype.box" , "Box" );
AddChoice( "structype.ball" , "Ball" );
AddChoice( "structype.cross" , "Cross" );

AddParameter(ParameterType_Choice, "filter", "Morphological Operation");
SetParameterDescription("filter", "Choice of the morphological operation");
//Dilate
AddChoice( "filter.dilate" , "Dilate" );
AddParameter( ParameterType_Float , "filter.dilate.foreval" ,
 "Foreground value" );
SetParameterDescription( "filter.dilate.foreval" ,
  "Set the foreground value, default is 1.0." );
SetDefaultParameterFloat( "filter.dilate.foreval" , 1.0 );
AddParameter( ParameterType_Float , "filter.dilate.backval" ,
 "Background value" );
SetParameterDescription( "filter.dilate.backval" ,
  "Set the background value, default is 0.0." );
SetDefaultParameterFloat( "filter.dilate.backval" , 0.0 );

//Erode
AddChoice( "filter.erode" , "Erode" );
AddParameter( ParameterType_Float , "filter.erode.foreval" ,
  "Foreground value" );
SetParameterDescription( "filter.erode.foreval" ,
  "Set the foreground value, default is 1.0." );
SetDefaultParameterFloat( "filter.erode.foreval" , 1.0 );
AddParameter( ParameterType_Float , "filter.erode.backval" ,
   "Background value" );
SetParameterDescription("filter.erode.backval",
  "Set the background value, default is 0.0." );
SetDefaultParameterFloat("filter.erode.backval", 0.0);

//Opening
AddChoice( "filter.opening" , "Opening" );
AddParameter( ParameterType_Float , "filter.opening.foreval" ,
  "Foreground value" );
SetParameterDescription( "filter.opening.foreval" ,
  "Set the foreground value, default is 1.0." );
SetDefaultParameterFloat("filter.opening.foreval", 1.0);
AddParameter( ParameterType_Float , "filter.opening.backval" ,
  "Background value" );
SetParameterDescription( "filter.opening.backval" ,
  "Set the background value, default is 0.0." );
SetDefaultParameterFloat( "filter.opening.backval" , 0.0 );
//Closing
AddChoice( "filter.closing" , "Closing" );
AddParameter( ParameterType_Float , "filter.closing.foreval" ,
  "Foreground value" );
SetParameterDescription( "filter.closing.foreval" ,
  "Set the foreground value, default is 1.0." );
SetDefaultParameterFloat( "filter.closing.foreval" , 1.0 );

AddRAMParameter();

// Doc example parameter settings
SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
SetDocExampleParameterValue("out", "opened.tif");
SetDocExampleParameterValue("channel", "1");
SetDocExampleParameterValue("xradius", "5");
SetDocExampleParameterValue("yradius", "5");
SetDocExampleParameterValue("filter", "erode");

SetOfficialDocLink();
}

void DoUpdateParameters() override
{
  // Nothing to do here: all parameters are independent
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

  m_ExtractorFilter = ExtractorFilterType::New();
  m_ExtractorFilter->SetInput(inImage);
  m_ExtractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
  m_ExtractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
  m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
  m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
  m_ExtractorFilter->SetChannel(GetParameterInt("channel"));
  m_ExtractorFilter->UpdateOutputInformation();

  RadiusType rad;
  rad[0] = this->GetParameterInt("xradius");
  rad[1] = this->GetParameterInt("yradius");

  StructuringType se;
  if(GetParameterString("structype") == "box")
    {
    se = StructuringType::Box(rad);
    }
  else if(GetParameterString("structype") == "ball")
    {
    se = StructuringType::Ball(rad);
    }
  else if(GetParameterString("structype") == "cross")
    {
    se = StructuringType::Cross(rad);
    }

  if(GetParameterString("filter") == "dilate")
    {
    m_DilFilter = DilateFilterType::New();
    m_DilFilter->SetKernel(se);
    m_DilFilter->SetInput(m_ExtractorFilter->GetOutput());
    m_DilFilter->SetForegroundValue(GetParameterFloat("filter.dilate.foreval"));
    m_DilFilter->SetBackgroundValue(GetParameterFloat("filter.dilate.backval"));
    SetParameterOutputImage("out", m_DilFilter->GetOutput());
    }
  else if(GetParameterString("filter") == "erode")
    {
    m_EroFilter = ErodeFilterType::New();
    m_EroFilter->SetKernel(se);
    m_EroFilter->SetInput(m_ExtractorFilter->GetOutput());
    m_EroFilter->SetForegroundValue(GetParameterFloat("filter.erode.foreval"));
    m_EroFilter->SetBackgroundValue(GetParameterFloat("filter.erode.backval"));
    SetParameterOutputImage("out", m_EroFilter->GetOutput());
    }
  else if(GetParameterString("filter") == "opening")
    {
    m_OpeFilter = OpeningFilterType::New();
    m_OpeFilter->SetKernel(se);
    m_OpeFilter->SetInput(m_ExtractorFilter->GetOutput());
    m_OpeFilter->SetForegroundValue(GetParameterFloat("filter.opening.foreval"));
    m_OpeFilter->SetBackgroundValue(GetParameterFloat("filter.opening.backval"));
    SetParameterOutputImage("out", m_OpeFilter->GetOutput());
    }
  else if(GetParameterString("filter") == "closing")
    {
    m_CloFilter = ClosingFilterType::New();
    m_CloFilter->SetKernel(se);
    m_CloFilter->SetInput(m_ExtractorFilter->GetOutput());
    m_CloFilter->SetForegroundValue(GetParameterFloat("filter.closing.foreval"));
    SetParameterOutputImage("out", m_CloFilter->GetOutput());
    }
}

ExtractorFilterType::Pointer                m_ExtractorFilter;

DilateFilterType::Pointer                   m_DilFilter;
ErodeFilterType::Pointer                    m_EroFilter;
OpeningFilterType::Pointer                  m_OpeFilter;
ClosingFilterType::Pointer                  m_CloFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::BinaryMorphologicalOperation)
