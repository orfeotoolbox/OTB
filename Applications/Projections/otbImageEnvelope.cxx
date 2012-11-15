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

#include "otbImageToEnvelopeVectorDataFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class ImageEnvelope : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageEnvelope                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageEnvelope, otb::Application);

  /** Filters typedef */
  typedef otb::ImageToEnvelopeVectorDataFilter
      <FloatVectorImageType, VectorDataType>          EnvelopeFilterType;

private:
  void DoInit()
  {
    SetName("ImageEnvelope");
    SetDescription("Extracts an image envelope.");

    // Documentation
    SetDocName("Image Envelope");
    SetDocLongDescription("Build a vector data containing the polygon of the image envelope.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
 
    AddDocTag(Tags::Geometry);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_OutputVectorData,  "out",   "Output Vector Data");
    SetParameterDescription("out", "Vector data file containing the envelope");

    AddParameter(ParameterType_Int, "sr", "Sampling Rate");
    SetParameterDescription("sr", "Sampling rate for image edges (in pixel)");
    SetDefaultParameterInt("sr",0);
    MandatoryOff("sr");
    DisableParameter("sr");
    
    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");
    
    AddParameter(ParameterType_String, "proj",  "Projection");
    SetParameterDescription("proj", "Projection to be used to compute the envelope (default is WGS84)");
    MandatoryOff("proj");
    
   // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_TOULOUSE_MUL_Extract_500_500.tif");
    SetDocExampleParameterValue("out", "ImageEnvelope.shp");
  }

  void DoUpdateParameters()
  {
    // Nothing to be done
  }

  void DoExecute()
  {
    FloatVectorImageType::Pointer input = GetParameterImage("in");
    
    m_Envelope = EnvelopeFilterType::New();
    m_Envelope->SetInput(input);
    m_Envelope->SetSamplingRate(GetParameterInt("sr"));
    
    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");    
    if (HasValue("proj"))
      {
      m_Envelope->SetOutputProjectionRef(GetParameterString("proj"));
      }
    
    SetParameterOutputVectorData("out", m_Envelope->GetOutput());
  }

  EnvelopeFilterType::Pointer m_Envelope;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageEnvelope)
