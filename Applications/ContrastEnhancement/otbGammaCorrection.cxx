
#include <iostream>
#include <sstream>


#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbGammaFunctor.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbGammaCorrectionFilter.h"

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{
class GammaCorrection : public Application
{
public:

  typedef GammaCorrection                Self;
  typedef Application                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(GammaCorrection, otb::Application);


private:

  void DoInit()
  {
    this->SetHaveInXML(false);
    this->SetHaveOutXML(false);
    SetName("GammaCorrection");
    SetDescription("This application encode and decode gamma of the input image");

    SetDocName("GammaCorrection");
    SetDocLongDescription("This application encode and decode gamma of the input image. If value gamma is < 1 it does gamma encoding and if > 1 it does gamma decoding.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("color");
    AddDocTag("histogram");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    MandatoryOn("in");

    AddParameter(ParameterType_Float, "gamma", "Input gamma value");
    MandatoryOn("gamma");
    SetDefaultParameterFloat("gamma", 1.0);

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    MandatoryOn("out");

    SetDocExampleParameterValue("in", "rgbInput.tif");
    SetDocExampleParameterValue("gamma", "2.2");
    SetDocExampleParameterValue("out", "gammaCorrected.tif");

  }


  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    double gamma = GetParameterFloat("gamma");
    otbAppLogDEBUG( << "Gamma = " << gamma <<std::endl );

    GammaFilterType::Pointer m_GammaFilter = GammaFilterType::New();

    m_GammaFilter->SetInput(inImage);
    m_GammaFilter->SetGamma(gamma);
    m_GammaFilter->Update();

    SetParameterOutputImage("out", m_GammaFilter->GetOutput());
  }

  typedef typename FloatVectorImageType::PixelType PixelType;
  typedef otb::Functor::GammaFunctor<PixelType, PixelType> GammaFunctorType;
  typedef otb::GammaCorrectionFilter<FloatVectorImageType,FloatVectorImageType,GammaFunctorType> GammaFilterType;

};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::GammaCorrection)
