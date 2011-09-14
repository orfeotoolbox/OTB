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

#include "itkMeanImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
//#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

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

class Smoothing : public Application
{
public:
  /** Standard class typedefs. */
  typedef Smoothing                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Smoothing, otb::Application);

private:
  Smoothing()
  {
    SetName("Smoothing");
    SetDescription("Apply a smoothing filter to an image");
  }

  virtual ~Smoothing()
  {
  }

  void DoCreateParameters()
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

    SetParameterString("type", "anidif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute");


    VectorImageType::Pointer inImage = GetParameterImage("in");

    switch ( GetParameterInt("type") )
      {
      case Smoothing_Mean:
        {
        GetLogger()->Debug("Using mean");

        typedef itk::MeanImageFilter<ImageType, ImageType>         MeanFilterType;
        typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, MeanFilterType>
          PerBandMeanFilterType;

        PerBandMeanFilterType::Pointer perBand
          = PerBandMeanFilterType::New();
        perBand->SetInput(inImage);

        MeanFilterType::InputSizeType radius;
        radius.Fill( GetParameterInt("type.mean.radius") );
        perBand->GetFilter()->SetRadius(radius);
        perBand->UpdateOutputInformation();
        m_FilterRef = perBand;
        SetParameterOutputImage("out", perBand->GetOutput());
        }
        break;
      case Smoothing_Gaussian:
        {
        GetLogger()->Debug("Using gaussian");

        typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType>  DiscreteGaussianFilterType;
        typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, DiscreteGaussianFilterType>
          PerBandDiscreteGaussianFilterType;

        PerBandDiscreteGaussianFilterType::Pointer perBand
          = PerBandDiscreteGaussianFilterType::New();
        perBand->SetInput(inImage);

        int radius = GetParameterInt("type.gaussian.radius");
        double variance = static_cast<double>(radius) * radius;
        perBand->GetFilter()->SetVariance(variance);
        perBand->UpdateOutputInformation();
        m_FilterRef = perBand;
        SetParameterOutputImage("out", perBand->GetOutput());
        }
        break;
      case Smoothing_Anisotropic:
        {
        GetLogger()->Debug("Using anisotropic diffusion");

        typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType>  GradientAnisotropicDiffusionFilterType;
        typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, GradientAnisotropicDiffusionFilterType>
          PerBandGradientAnisotropicDiffusionFilterType;

        PerBandGradientAnisotropicDiffusionFilterType::Pointer perBand
          = PerBandGradientAnisotropicDiffusionFilterType::New();
        perBand->SetInput(inImage);

        int aniDifNbIter = GetParameterInt("type.anidif.nbiter");
        perBand->GetFilter()->SetNumberOfIterations(static_cast<unsigned int>(aniDifNbIter));

        float aniDifTimeStep = GetParameterFloat("type.anidif.timestep");
        perBand->GetFilter()->SetTimeStep(static_cast<double>(aniDifTimeStep));
        // perBand->GetFilter()->SetConductanceParameter()
        perBand->UpdateOutputInformation();
        m_FilterRef = perBand;
        SetParameterOutputImage("out", perBand->GetOutput());
        }
        break;
      }
  }

  itk::LightObject::Pointer m_FilterRef;
};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Smoothing)

