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
#include "otbSmoothing.h"
#include "otbWrapperNumericalParameter.h"

#include "itkMeanImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
//#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

#include "itkVersion.h"

namespace otb
{
namespace Wrapper
{

enum
{
  Smoothing_Mean,
  Smoothing_Gaussian,
  Smoothing_Anisotropic
};

typedef otb::Wrapper::InputImageParameter::VectorImageType VectorImageType;
typedef otb::Image<VectorImageType::InternalPixelType, 2>  ImageType;

typedef itk::MeanImageFilter<ImageType, ImageType>         MeanFilterType;
typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, MeanFilterType>
  PerBandMeanFilterType;

typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType>  DiscreteGaussianFilterType;
typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, DiscreteGaussianFilterType>
  PerBandDiscreteGaussianFilterType;

typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType>  GradientAnisotropicDiffusionFilterType;
typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, GradientAnisotropicDiffusionFilterType>
  PerBandGradientAnisotropicDiffusionFilterType;


Smoothing::Smoothing()
{
  this->SetName("Smoothing");
  this->SetDescription("Apply a smoothing filter to an image");
}

Smoothing::~Smoothing()
{
}

void Smoothing::DoCreateParameters()
{
  otb::Wrapper::InputImageParameter::Pointer  inImage  = otb::Wrapper::InputImageParameter::New();
  otb::Wrapper::OutputImageParameter::Pointer outImage = otb::Wrapper::OutputImageParameter::New();

  otb::Wrapper::ChoiceParameter::Pointer smoothingType  = otb::Wrapper::ChoiceParameter::New();
  smoothingType->SetName("Smoothing Type");
  smoothingType->SetKey("type");

  otb::Wrapper::RadiusParameter::Pointer meanSmoothingRadius  = otb::Wrapper::RadiusParameter::New();
  meanSmoothingRadius->SetValue(1);
  smoothingType->AddChoice("mean", "Mean", meanSmoothingRadius.GetPointer());

  otb::Wrapper::RadiusParameter::Pointer gaussianSmoothingRadius  = otb::Wrapper::RadiusParameter::New();
  gaussianSmoothingRadius->SetValue(1);
  smoothingType->AddChoice("gaussian", "Gaussian", gaussianSmoothingRadius.GetPointer());

  otb::Wrapper::FloatParameter::Pointer aniDifTimeStep  = otb::Wrapper::FloatParameter::New();
  aniDifTimeStep->SetName("Time Step");
  aniDifTimeStep->SetKey("timestep");
  aniDifTimeStep->SetValue(0.125);
  otb::Wrapper::IntParameter::Pointer aniDifNbIter = otb::Wrapper::IntParameter::New();
  aniDifNbIter->SetName("Nb Iterations");
  aniDifNbIter->SetKey("nbiter");
  aniDifNbIter->SetValue(10);
  otb::Wrapper::ParameterGroup::Pointer aniDifGroup = otb::Wrapper::ParameterGroup::New();
  aniDifGroup->AddParameter(aniDifTimeStep.GetPointer());
  aniDifGroup->AddParameter(aniDifNbIter.GetPointer());
  smoothingType->AddChoice("anidif", "Anisotropic Diffusion", aniDifGroup.GetPointer());

  smoothingType->SetValue(2);

  ParameterGroup* params = GetParameterList();
  params->AddParameter(inImage.GetPointer());
  params->AddParameter(outImage.GetPointer());
  params->AddParameter(smoothingType.GetPointer());
}

void Smoothing::DoUpdateParameters()
{
  // Nothing to do here : all parameters are independent
}

void Smoothing::DoExecute()
{
  ParameterGroup* params = GetParameterList();

  otb::Wrapper::InputImageParameter* inImageParam = dynamic_cast<otb::Wrapper::InputImageParameter*>(params->GetParameterByKey("in").GetPointer());
  VectorImageType::Pointer inImage = boost::any_cast<VectorImageType::Pointer>(inImageParam->GetAnyValue());

  otb::Wrapper::OutputImageParameter* outImageParam = dynamic_cast<otb::Wrapper::OutputImageParameter*>(params->GetParameterByKey("out").GetPointer());

  otb::Wrapper::ChoiceParameter* smoothingTypeParam = dynamic_cast<otb::Wrapper::ChoiceParameter*>(params->GetParameterByKey("type").GetPointer());
  int smoothingType = smoothingTypeParam->GetValue();

  otb::Wrapper::ParameterGroup* subParam = smoothingTypeParam->GetChoiceAssociatedParameter(smoothingType);

  switch (smoothingType)
    {
    case Smoothing_Mean:
      {
      otb::Wrapper::RadiusParameter* radiusParam
        = dynamic_cast<otb::Wrapper::RadiusParameter*>(params->GetParameterByKey("type.mean.radius").GetPointer());

      PerBandMeanFilterType::Pointer perBand = PerBandMeanFilterType::New();

      perBand->SetInput(inImage);

      MeanFilterType::InputSizeType radius;
      radius.Fill(radiusParam->GetValue());
      perBand->GetFilter()->SetRadius(radius);
      ref = perBand;
      outImageParam->SetValue( perBand->GetOutput() );
      }
      break;
    case Smoothing_Gaussian:
      {
      otb::Wrapper::RadiusParameter* radiusParam
        = dynamic_cast<otb::Wrapper::RadiusParameter*>(subParam->GetParameterByKey("type.gaussian.radius").GetPointer());
      int radius = radiusParam->GetValue();

      PerBandDiscreteGaussianFilterType::Pointer perBand = PerBandDiscreteGaussianFilterType::New();

      perBand->SetInput(inImage);

      double variance = radiusParam->GetValue() * radiusParam->GetValue();
      perBand->GetFilter()->SetVariance(variance);
      ref = perBand;
      outImageParam->SetValue( perBand->GetOutput() );
      }
      break;
    case Smoothing_Anisotropic:
      {
      otb::Wrapper::FloatParameter* aniDifTimeStepParam = dynamic_cast<otb::Wrapper::FloatParameter*>(subParam->GetParameterByKey("type.anidif.timestep").GetPointer());
      otb::Wrapper::IntParameter* aniDifNbIterParam = dynamic_cast<otb::Wrapper::IntParameter*>(subParam->GetParameterByKey("type.anidif.nbiter").GetPointer());

      float aniDifTimeStep = aniDifTimeStepParam->GetValue();
      int aniDifNbIter = aniDifNbIterParam->GetValue();

      PerBandGradientAnisotropicDiffusionFilterType::Pointer perBand = PerBandGradientAnisotropicDiffusionFilterType::New();

      perBand->SetInput(inImage);

      perBand->GetFilter()->SetNumberOfIterations(static_cast<unsigned int>(aniDifNbIter));
      perBand->GetFilter()->SetTimeStep(static_cast<double>(aniDifTimeStep));
      // perBand->GetFilter()->SetConductanceParameter()
      perBand->UpdateOutputInformation();
      ref = perBand;
      outImageParam->SetValue( perBand->GetOutput() );
      }
      break;
    }
}


class SmoothingFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef SmoothingFactory              Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const
    {
    return ITK_SOURCE_VERSION;
    }

  virtual const char* GetDescription(void) const
    {
    return "Smoothing";
    }

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SmoothingFactory, itk::ObjectFactoryBase);

protected:
  SmoothingFactory()
  {

  }

  virtual ~SmoothingFactory()
  {

  }

  /** This method is provided by sub-classes of ObjectFactoryBase.
   * It should create the named itk object or return 0 if that object
   * is not supported by the factory implementation. */
  virtual LightObject::Pointer CreateObject(const char* itkclassname )
  {
    const std::string classname("otbWrapperApplication");
    LightObject::Pointer ret;
    if ( classname == itkclassname )
      ret = Smoothing::New().GetPointer();

    return ret;
  }

  /** This method creates all the objects with the class overide of
   * itkclass name, which are provide by this object
   */
  virtual std::list<LightObject::Pointer>
  CreateAllObject(const char* itkclassname)
  {
    const std::string classname("otbWrapperApplication");
    std::list<LightObject::Pointer> list;
    if ( classname == itkclassname )
      list.push_back(Smoothing::New().GetPointer());

    return list;
  }

private:
  SmoothingFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};


}

}

static otb::Wrapper::SmoothingFactory::Pointer staticFactory;
itk::ObjectFactoryBase* itkLoad()
{
  staticFactory = otb::Wrapper::SmoothingFactory::New();
  return staticFactory;
}

