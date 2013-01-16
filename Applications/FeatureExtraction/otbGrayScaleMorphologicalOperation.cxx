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

#include "itkGrayscaleDilateImageFilter.h"
#include "itkGrayscaleErodeImageFilter.h"
#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "itkTimeProbe.h"

namespace otb
{
namespace Wrapper
{

class GrayScaleMorphologicalOperation : public Application
{
public:
/** Standard class typedefs. */
typedef GrayScaleMorphologicalOperation          Self;
typedef Application                   Superclass;
typedef itk::SmartPointer<Self>       Pointer;
typedef itk::SmartPointer<const Self> ConstPointer;

typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,FloatVectorImageType::InternalPixelType>
ExtractorFilterType;

typedef itk::BinaryBallStructuringElement<FloatImageType::PixelType, 2>        BallStructuringType;
typedef BallStructuringType::RadiusType                                        RadiusType;
typedef BallStructuringType::Superclass                                        StructuringType;
typedef itk::BinaryCrossStructuringElement<FloatImageType::PixelType, 2>       CrossStructuringType;

typedef itk::GrayscaleDilateImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               DilateFilterType;
typedef itk::GrayscaleErodeImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               ErodeFilterType;
typedef itk::GrayscaleMorphologicalOpeningImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               OpeningFilterType;
typedef itk::GrayscaleMorphologicalClosingImageFilter<FloatImageType, FloatImageType, StructuringType>
                                                                               ClosingFilterType;

typedef ImageList<FloatImageType>                                              ImageListType;
typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType>      ImageListToVectorImageFilterType;

/** Standard macro */
itkNewMacro(Self);
itkTypeMacro(GrayScaleMorphologicalOperation, otb::Application);

private:

void DoInit()
{
SetName("GrayScaleMorphologicalOperation");
SetDescription("Performs morphological operations on a grayscale input image");

// Documentation
SetDocName("Grayscale Morphological Operation");
SetDocLongDescription("This application performs grayscale morphological operations on a mono band image");
SetDocLimitations("None");
SetDocAuthors("OTB-Team");
SetDocSeeAlso("itkGrayscaleDilateImageFilter, itkGrayscaleErodeImageFilter, itkGrayscaleMorphologicalOpeningImageFilter and itkGrayscaleMorphologicalClosingImageFilter classes");

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

//Erode
AddChoice("filter.erode", "Erode");

//Opening
AddChoice("filter.opening", "Opening");

//Closing
AddChoice("filter.closing", "Closing");


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
      SetParameterOutputImage("out", m_DilFilter->GetOutput());
      }

    if(GetParameterString("filter") == "erode")
      {
      m_EroFilter = ErodeFilterType::New();
      m_EroFilter->SetKernel(se);
      m_EroFilter->SetInput(m_ExtractorFilter->GetOutput());
      SetParameterOutputImage("out", m_EroFilter->GetOutput());
      }

    if(GetParameterString("filter") == "opening")
      {
      m_OpeFilter = OpeningFilterType::New();
      m_OpeFilter->SetKernel(se);
      m_OpeFilter->SetInput(m_ExtractorFilter->GetOutput());
      SetParameterOutputImage("out", m_OpeFilter->GetOutput());
      }

    if(GetParameterString("filter") == "closing")
      {
      m_CloFilter = ClosingFilterType::New();
      m_CloFilter->SetKernel(se);
      m_CloFilter->SetInput(m_ExtractorFilter->GetOutput());
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
      SetParameterOutputImage("out", m_DilFilter->GetOutput());
      }

    if(GetParameterString("filter") == "erode")
      {
      m_EroFilter = ErodeFilterType::New();
      m_EroFilter->SetKernel(se);
      m_EroFilter->SetInput(m_ExtractorFilter->GetOutput());
      SetParameterOutputImage("out", m_EroFilter->GetOutput());
      }

    if(GetParameterString("filter") == "opening")
      {
      m_OpeFilter = OpeningFilterType::New();
      m_OpeFilter->SetKernel(se);
      m_OpeFilter->SetInput(m_ExtractorFilter->GetOutput());
      SetParameterOutputImage("out", m_OpeFilter->GetOutput());
      }

    if(GetParameterString("filter") == "closing")
      {
      m_CloFilter = ClosingFilterType::New();
      m_CloFilter->SetKernel(se);
      m_CloFilter->SetInput(m_ExtractorFilter->GetOutput());
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

OTB_APPLICATION_EXPORT(otb::Wrapper::GrayScaleMorphologicalOperation)
