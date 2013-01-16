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

#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryCrossStructuringElement.h"

#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "itkTimeProbe.h"

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

typedef itk::BinaryBallStructuringElement<FloatImageType::PixelType, 2>        BallStructuringType;
typedef BallStructuringType::RadiusType                                        RadiusType;
typedef BallStructuringType::Superclass                                        StructuringType;
typedef itk::BinaryCrossStructuringElement<FloatImageType::PixelType, 2>       CrossStructuringType;

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

void DoInit()
{
SetName("BinaryMorphologicalOperation");
SetDescription("Performs morphological operations on an input image channel");

// Documentation
SetDocName("Binary Morphological Operation");
SetDocLongDescription("his application performs binary morphological operations on a mono band image");
SetDocLimitations("None");
SetDocAuthors("OTB-Team");
SetDocSeeAlso("itkBinaryDilateImageFilter, itkBinaryErodeImageFilter, itkBinaryMorphologicalOpeningImageFilter and itkBinaryMorphologicalClosingImageFilter classes");

AddDocTag("MorphologicalOperations");
AddDocTag(Tags::FeatureExtraction);

AddParameter(ParameterType_InputImage, "in",  "Input Image");
SetParameterDescription("in", "The input image to be filtered.");

AddParameter(ParameterType_OutputImage, "out", "Feature Output Image");
SetParameterDescription("out", "Output image containing the filtered output image.");

AddParameter(ParameterType_Int,  "channel",  "Selected Channel");
SetParameterDescription("channel", "The selected channel index");
SetDefaultParameterInt("channel", 1);
SetMinimumParameterIntValue("channel", 1);

AddRAMParameter();

AddParameter(ParameterType_Choice, "structype", "Structuring Element Type");
SetParameterDescription("structype", "Choice of the structuring element type");
//Ball
AddChoice("structype.ball", "Ball");
AddParameter(ParameterType_Int, "structype.ball.xradius", "The Structuring Element X Radius");
SetParameterDescription("structype.ball.xradius", "The Structuring Element X Radius");
SetDefaultParameterInt("structype.ball.xradius", 5);
AddParameter(ParameterType_Int, "structype.ball.yradius", "The Structuring Element Y Radius");
SetParameterDescription("structype.ball.yradius", "The Structuring Element Y Radius");
SetDefaultParameterInt("structype.ball.yradius", 5);
//Cross
AddChoice("structype.cross", "Cross");

AddParameter(ParameterType_Choice, "filter", "Morphological Operation");
SetParameterDescription("filter", "Choice of the morphological operation");
//Dilate
AddChoice("filter.dilate", "Dilate");
AddParameter(ParameterType_Float, "filter.dilate.foreval", "Foreground Value");
SetParameterDescription("filter.dilate.foreval", "The Foreground Value");
SetDefaultParameterFloat("filter.dilate.foreval", 1.0);
AddParameter(ParameterType_Float, "filter.dilate.backval", "Background Value");
SetParameterDescription("filter.dilate.backval", "The Background Value");
SetDefaultParameterFloat("filter.dilate.backval", 0.0);

//Erode
AddChoice("filter.erode", "Erode");
AddParameter(ParameterType_Float, "filter.erode.foreval", "Foreground Value");
SetParameterDescription("filter.erode.foreval", "The Foreground Value");
SetDefaultParameterFloat("filter.erode.foreval", 1.0);
AddParameter(ParameterType_Float, "filter.erode.backval", "Background Value");
SetParameterDescription("filter.erode.backval", "The Background Value");
SetDefaultParameterFloat("filter.erode.backval", 0.0);
//Opening
AddChoice("filter.opening", "Opening");
AddParameter(ParameterType_Float, "filter.opening.foreval", "Foreground Value");
SetParameterDescription("filter.opening.foreval", "The Foreground Value");
SetDefaultParameterFloat("filter.opening.foreval", 1.0);
AddParameter(ParameterType_Float, "filter.opening.backval", "Background Value");
SetParameterDescription("filter.opening.backval", "The Background Value");
SetDefaultParameterFloat("filter.opening.backval", 0.0);
//Closing
AddChoice("filter.closing", "Closing");
AddParameter(ParameterType_Float, "filter.closing.foreval", "Foreground Value");
SetParameterDescription("filter.closing.foreval", "The Foreground Value");
SetDefaultParameterFloat("filter.closing.foreval", 1.0);

// Doc example parameter settings
SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
SetDocExampleParameterValue("out", "opened.tif");
SetDocExampleParameterValue("channel", "1");
SetDocExampleParameterValue("structype.ball.xradius", "5");
SetDocExampleParameterValue("structype.ball.yradius", "5");
SetDocExampleParameterValue("filter", "erode");
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

  if(GetParameterString("structype") == "ball")
    {
    BallStructuringType se;
    RadiusType rad;
    rad[0] = this->GetParameterInt("structype.ball.xradius");
    rad[1] = this->GetParameterInt("structype.ball.yradius");
    se.SetRadius(rad);
    se.CreateStructuringElement();

    if(GetParameterString("filter") == "dilate")
      {
      m_DilFilter = DilateFilterType::New();
      m_DilFilter->SetKernel(se);
      m_DilFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_DilFilter->SetForegroundValue(GetParameterFloat("filter.dilate.foreval"));
      m_DilFilter->SetBackgroundValue(GetParameterFloat("filter.dilate.backval"));
      SetParameterOutputImage("out", m_DilFilter->GetOutput());
      }

    if(GetParameterString("filter") == "erode")
      {
      m_EroFilter = ErodeFilterType::New();
      m_EroFilter->SetKernel(se);
      m_EroFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_EroFilter->SetForegroundValue(GetParameterFloat("filter.erode.foreval"));
      m_EroFilter->SetBackgroundValue(GetParameterFloat("filter.erode.backval"));
      SetParameterOutputImage("out", m_EroFilter->GetOutput());
      }

    if(GetParameterString("filter") == "opening")
      {
      m_OpeFilter = OpeningFilterType::New();
      m_OpeFilter->SetKernel(se);
      m_OpeFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_OpeFilter->SetForegroundValue(GetParameterFloat("filter.opening.foreval"));
      m_OpeFilter->SetBackgroundValue(GetParameterFloat("filter.opening.backval"));
      SetParameterOutputImage("out", m_OpeFilter->GetOutput());
      }

    if(GetParameterString("filter") == "closing")
      {
      m_CloFilter = ClosingFilterType::New();
      m_CloFilter->SetKernel(se);
      m_CloFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_CloFilter->SetForegroundValue(GetParameterFloat("filter.closing.foreval"));
      SetParameterOutputImage("out", m_CloFilter->GetOutput());
      }
    }
  if(GetParameterString("structype") == "cross")
    {
    CrossStructuringType se;
    se.CreateStructuringElement();

    if(GetParameterString("filter") == "dilate")
      {
      m_DilFilter = DilateFilterType::New();
      m_DilFilter->SetKernel(se);
      m_DilFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_DilFilter->SetForegroundValue(GetParameterFloat("filter.dilate.foreval"));
      m_DilFilter->SetBackgroundValue(GetParameterFloat("filter.dilate.backval"));
      SetParameterOutputImage("out", m_DilFilter->GetOutput());
      }

    if(GetParameterString("filter") == "erode")
      {
      m_EroFilter = ErodeFilterType::New();
      m_EroFilter->SetKernel(se);
      m_EroFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_EroFilter->SetForegroundValue(GetParameterFloat("filter.erode.foreval"));
      m_EroFilter->SetBackgroundValue(GetParameterFloat("filter.erode.backval"));
      SetParameterOutputImage("out", m_EroFilter->GetOutput());
      }

    if(GetParameterString("filter") == "opening")
      {
      m_OpeFilter = OpeningFilterType::New();
      m_OpeFilter->SetKernel(se);
      m_OpeFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_OpeFilter->SetForegroundValue(GetParameterFloat("filter.opening.foreval"));
      m_OpeFilter->SetBackgroundValue(GetParameterFloat("filter.opening.backval"));
      SetParameterOutputImage("out", m_OpeFilter->GetOutput());
      }

    if(GetParameterString("filter") == "closing")
      {
      m_CloFilter = ClosingFilterType::New();
      m_CloFilter->SetKernel(se);
      m_CloFilter->SetInput(m_ExtractorFilter->GetOutput());
      m_CloFilter->SetForegroundValue(GetParameterFloat("filter.closing.foreval"));
      SetParameterOutputImage("out", m_CloFilter->GetOutput());
      }
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
