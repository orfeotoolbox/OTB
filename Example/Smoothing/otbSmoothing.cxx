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

typedef otb::Image<VectorImageType::InternalPixelType, 2>  ImageType;


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
  AddParameter(ParameterType_InputImage,  "in",   "Input Image");
  AddParameter(ParameterType_OutputImage, "out",  "Output Image");

  AddParameter(ParameterType_Choice,      "type", "Smoothing Type");

  AddChoice("type.mean",     "Mean");
  AddParameter(ParameterType_Radius, "type.mean.radius", "Radius");
  SetParameterInt("type.mean.radius", 2);

  AddChoice("type.gaussian", "Gaussian");
  AddParameter(ParameterType_Radius, "type.gaussian.radius", "Radius");
  SetParameterInt("type.gaussian.radius", 2);

  AddChoice("type.anidif",   "Anisotropic Diffusion");
  AddParameter(ParameterType_Float,  "type.anidif.timestep", "Time Step");
  AddParameter(ParameterType_Int,  "type.anidif.nbiter", "Nb Iterations");
  SetParameterFloat("type.anidif.timestep",   0.125);
  SetParameterInt("type.anidif.nbiter",     10);

  SetParameterInt("type", 2);
}

void Smoothing::DoUpdateParameters()
{
  // Nothing to do here : all parameters are independent
}

void Smoothing::DoExecute()
{
  VectorImageType::Pointer inImage = this->GetParameterImage("in");

  switch (this->GetParameterInt("type"))
    {
    case Smoothing_Mean:
      {
      typedef itk::MeanImageFilter<ImageType, ImageType>         MeanFilterType;
      typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, MeanFilterType>
        PerBandMeanFilterType;

      PerBandMeanFilterType::Pointer perBand
        = PerBandMeanFilterType::New();
      perBand->SetInput(inImage);

      MeanFilterType::InputSizeType radius;
      radius.Fill( this->GetParameterInt("type.mean.radius") );
      perBand->GetFilter()->SetRadius(radius);
      ref = perBand;
      this->SetParameterOutputImage("out", perBand->GetOutput());
      }
      break;
    case Smoothing_Gaussian:
      {
      typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType>  DiscreteGaussianFilterType;
      typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, DiscreteGaussianFilterType>
        PerBandDiscreteGaussianFilterType;

      PerBandDiscreteGaussianFilterType::Pointer perBand
        = PerBandDiscreteGaussianFilterType::New();
      perBand->SetInput(inImage);

      int radius = this->GetParameterInt("type.gaussian.radius");
      double variance = static_cast<double>(radius) * radius;
      perBand->GetFilter()->SetVariance(variance);
      ref = perBand;
      this->SetParameterOutputImage("out", perBand->GetOutput());
      }
      break;
    case Smoothing_Anisotropic:
      {
      typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType>  GradientAnisotropicDiffusionFilterType;
      typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, GradientAnisotropicDiffusionFilterType>
        PerBandGradientAnisotropicDiffusionFilterType;

      PerBandGradientAnisotropicDiffusionFilterType::Pointer perBand
        = PerBandGradientAnisotropicDiffusionFilterType::New();
      perBand->SetInput(inImage);

      int aniDifNbIter = this->GetParameterInt("type.anidif.nbiter");
      perBand->GetFilter()->SetNumberOfIterations(static_cast<unsigned int>(aniDifNbIter));

      float aniDifTimeStep = this->GetParameterFloat("type.anidif.timestep");
      perBand->GetFilter()->SetTimeStep(static_cast<double>(aniDifTimeStep));
      // perBand->GetFilter()->SetConductanceParameter()
      perBand->UpdateOutputInformation();
      ref = perBand;
      this->SetParameterOutputImage("out", perBand->GetOutput());
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

