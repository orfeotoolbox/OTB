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

#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

namespace otb
{
namespace Wrapper
{


class Rescale : public Application
{
public:
  /** Standard class typedefs. */
  typedef Rescale                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Rescale, otb::Application);

  /** Filters typedef */
  typedef otb::StreamingMinMaxVectorImageFilter<FloatVectorImageType>  MinMaxFilterType;
  typedef otb::VectorRescaleIntensityImageFilter<FloatVectorImageType> RescaleImageFilterType;

private:
   Rescale()
  {
    SetName("Rescale");
    SetDescription("Rescale the image between two given values.");

    SetDocName("Rescale Image Application");
    SetDocLongDescription("This application scale the given image pixel intensity between two given values. "
      "By default min (resp. max) value is set to 0 (resp. 255).");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine Rescale ${OTB-BIN}/bin"
      " --in ${OTB-DATA}/Input/poupees.tif --out rescaledImage.tif --outmin 20 --outmax 150");
    AddDocTag("Image Manipulation");
  }

  virtual ~Rescale()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription( "in", "The image to scale." );
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out" , "The rescaled image filename." );
    AddParameter(ParameterType_Float,      "outmin", "Output min value");
    AddParameter(ParameterType_Float,      "outmax", "Output max value");
    SetDefaultParameterFloat("outmin", 0.0);
    SetParameterDescription( "outmin", "Minimum value of the output image." );
    SetDefaultParameterFloat("outmax", 255.0);
    SetParameterDescription( "outmax", "Maximum value of the output image." );

    MandatoryOff("outmin");
    MandatoryOff("outmax");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute()
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    otbAppLogDEBUG( << "Starting Min/Max computation" )

    m_MinMaxFilter = MinMaxFilterType::New();
    m_MinMaxFilter->SetInput( inImage );
    m_MinMaxFilter->GetStreamer()->SetNumberOfLinesStrippedStreaming( 50 );

    AddProcess(m_MinMaxFilter->GetStreamer(), "Min/Max computing");
    m_MinMaxFilter->Update();

    otbAppLogDEBUG( << "Min/Max computation done : min=" << m_MinMaxFilter->GetMinimum()
                    << " max=" << m_MinMaxFilter->GetMaximum() )

    FloatVectorImageType::PixelType inMin, inMax;

    m_RescaleFilter = RescaleImageFilterType::New();
    m_RescaleFilter->SetInput( inImage );
    m_RescaleFilter->SetInputMinimum( m_MinMaxFilter->GetMinimum() );
    m_RescaleFilter->SetInputMaximum( m_MinMaxFilter->GetMaximum() );

    FloatVectorImageType::PixelType outMin, outMax;
    outMin.SetSize( inImage->GetNumberOfComponentsPerPixel() );
    outMax.SetSize( inImage->GetNumberOfComponentsPerPixel() );
    outMin.Fill( GetParameterFloat("outmin") );
    outMax.Fill( GetParameterFloat("outmax") );

    m_RescaleFilter->SetOutputMinimum( outMin );
    m_RescaleFilter->SetOutputMaximum( outMax );
    m_RescaleFilter->UpdateOutputInformation();
    
    SetParameterOutputImage("out", m_RescaleFilter->GetOutput());
  }
  
  RescaleImageFilterType::Pointer m_RescaleFilter;
  MinMaxFilterType::Pointer       m_MinMaxFilter;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rescale)
