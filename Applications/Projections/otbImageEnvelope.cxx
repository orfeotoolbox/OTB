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
  ImageEnvelope()
  {
    SetName("ImageEnvelope");
    SetDescription("Extracts an image envelope.");

    // Documentation
    SetDocName("Image Envelope Application");
    SetDocLongDescription("Build a vector data containing the polygon of the image envelope.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
 
    AddDocTag(Tags::Geometry);
  }

  virtual ~ImageEnvelope()
  {
  }

  void DoCreateParameters()
    {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_OutputVectorData,  "out",   "Output Vector Data");
    SetParameterDescription("out", "Vector data file containing the envelope");
    
    AddParameter(ParameterType_Float, "ae",  "AverageElevation");
    SetParameterDescription("ae", "If no DEM is used, provide the height value (default is 0 meters)");
    SetDefaultParameterFloat("ae", 0.0);
    MandatoryOff("ae");
    
    AddParameter(ParameterType_String, "dem",  "DEMDirectory");
    SetParameterDescription("dem", "Use DEM tiles to derive height values (AverageElevation option is ignored in this case)");
    MandatoryOff("dem");
    
    AddParameter(ParameterType_String, "proj",  "Projection");
    SetParameterDescription("proj", "Projection to be used to compute the envelope (default is WGS84)");
    MandatoryOff("proj");
    
   SetDocCLExample("otbApplicationLauncherCommandLine ImageEnvelope ${OTB-BIN}/bin "
      "--in ${OTB-Data}/Input/sensor_stereo_left.tif --out envelope.shp");

   // Doc example parameter settings
    SetDocExampleParameterValue("in", "sensor_stereo_left.tif");
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
    
    if (HasValue("dem"))
      {
      m_Envelope->SetDEMDirectory(GetParameterString("dem"));
      }
    else
      {
      m_Envelope->SetAverageElevation(GetParameterFloat("ae"));
      }
    
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
