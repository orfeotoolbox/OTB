/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMaximumAutocorrelationFactorImageFilter.h"
#include "otbNAPCAImageFilter.h"
#include "otbLocalActivityVectorImageFilter.h"
#include "otbMaximumAutocorrelationFactorImageFilter.h"
#include "otbFastICAImageFilter.h"

#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

namespace otb
{
namespace Wrapper
{

class DimensionalityReduction: public Application
{
public:
  /** Standard class typedefs. */
  typedef DimensionalityReduction Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Dimensionality reduction typedef
  typedef otb::MaximumAutocorrelationFactorImageFilter<FloatVectorImageType, FloatVectorImageType> MAFFilterType;

  typedef itk::ImageToImageFilter<FloatVectorImageType, FloatVectorImageType> DimensionalityReductionFilter;

  // Reduction dimensio filters
  typedef otb::PCAImageFilter<FloatVectorImageType, FloatVectorImageType, otb::Transform::FORWARD> PCAForwardFilterType;
  typedef otb::PCAImageFilter<FloatVectorImageType, FloatVectorImageType, otb::Transform::INVERSE> PCAInverseFilterType;
  //typedef otb::PCAImageFilter< FloatVectorImageType, FloatVectorImageType, otb::Transform::FORWARD >

  typedef otb::LocalActivityVectorImageFilter<FloatVectorImageType, FloatVectorImageType> NoiseFilterType;

  typedef otb::NAPCAImageFilter<FloatVectorImageType, FloatVectorImageType, NoiseFilterType, otb::Transform::FORWARD>
      NAPCAForwardFilterType;
  typedef otb::NAPCAImageFilter<FloatVectorImageType, FloatVectorImageType, NoiseFilterType, otb::Transform::INVERSE>
      NAPCAInverseFilterType;

  typedef otb::MaximumAutocorrelationFactorImageFilter<FloatVectorImageType, FloatVectorImageType> MAFForwardFilterType;

  typedef otb::FastICAImageFilter<FloatVectorImageType, FloatVectorImageType, otb::Transform::FORWARD>
      ICAForwardFilterType;
  typedef otb::FastICAImageFilter<FloatVectorImageType, FloatVectorImageType, otb::Transform::INVERSE>
      ICAInverseFilterType;

  typedef otb::StreamingStatisticsVectorImageFilter<FloatVectorImageType> StreamingStatisticsVectorImageFilterType;

  typedef StreamingStatisticsVectorImageFilterType::MatrixObjectType::ComponentType                 MatrixType;

  // output rescale
  typedef otb::StreamingMinMaxVectorImageFilter<FloatVectorImageType> MinMaxFilterType;
  typedef otb::VectorRescaleIntensityImageFilter<FloatVectorImageType> RescaleImageFilterType;

  /** Standard macro */
  itkNewMacro(Self)
;

  itkTypeMacro(DimensionalityReduction, otb::Wrapper::Application)
;

private:
  void DoInit() override
  {
    SetName("DimensionalityReduction");
    SetDescription("Perform Dimension reduction of the input image.");
    SetDocLongDescription("Performs dimensionality reduction on input image. PCA,NA-PCA,MAF,ICA methods are available. It is also possible to compute the inverse transform to reconstruct the image and to optionally export the transformation matrix to a text file.");
    SetDocLimitations("This application does not provide the inverse transform and the transformation matrix export for the MAF.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
                  "\"Kernel maximum autocorrelation factor and minimum noise fraction transformations,\" IEEE Transactions on Image Processing, vol. 20, no. 3, pp. 612-624, (2011)");

    AddDocTag(Tags::Filter);
	AddDocTag(Tags::DimensionReduction);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to apply dimensionality reduction.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "output image. Components are ordered by decreasing eigenvalues.");
    MandatoryOff("out");
    AddParameter(ParameterType_Group, "rescale", "Rescale Output");

    MandatoryOff("rescale");
    //  AddChoice("rescale.no","No rescale");
    // AddChoice("rescale.minmax","rescale to min max value");

    AddParameter(ParameterType_Float, "rescale.outmin", "Output min value");
    AddParameter(ParameterType_Float, "rescale.outmax", "Output max value");
    SetDefaultParameterFloat("rescale.outmin", 0.0);
    SetParameterDescription("rescale.outmin", "Minimum value of the output image.");
    SetDefaultParameterFloat("rescale.outmax", 255.0);
    SetParameterDescription("rescale.outmax", "Maximum value of the output image.");

    AddParameter(ParameterType_OutputImage, "outinv", " Inverse Output Image");
    SetParameterDescription("outinv", "reconstruct output image.");
    MandatoryOff("outinv");

    AddParameter(ParameterType_Choice, "method", "Algorithm");
    SetParameterDescription("method", "Selection of the reduction dimension method.");

    AddChoice("method.pca", "PCA");
    SetParameterDescription("method.pca", "Principal Component Analysis.");
    AddChoice("method.napca", "NA-PCA");
    SetParameterDescription("method.napca", "Noise Adjusted Principal Component Analysis.");
    AddParameter(ParameterType_Int, "method.napca.radiusx", "Set the x radius of the sliding window");
    SetMinimumParameterIntValue("method.napca.radiusx", 1);
    SetDefaultParameterInt("method.napca.radiusx", 1);
    AddParameter(ParameterType_Int, "method.napca.radiusy", "Set the y radius of the sliding window");
    SetMinimumParameterIntValue("method.napca.radiusy", 1);
    SetDefaultParameterInt("method.napca.radiusy", 1);

    AddChoice("method.maf", "MAF");
    SetParameterDescription("method.maf", "Maximum Autocorrelation Factor.");
    AddChoice("method.ica", "ICA");
    SetParameterDescription("method.ica", "Independent Component Analysis using a stabilized fixed point FastICA algorithm.");
    AddParameter(ParameterType_Int, "method.ica.iter", "number of iterations");
    SetMinimumParameterIntValue("method.ica.iter", 1);
    SetDefaultParameterInt("method.ica.iter", 20);
    MandatoryOff("method.ica.iter");

    AddParameter(ParameterType_Float, "method.ica.mu", "Give the increment weight of W in [0, 1]");
    SetMinimumParameterFloatValue("method.ica.mu", 0.);
    SetMaximumParameterFloatValue("method.ica.mu", 1.);
    SetDefaultParameterFloat("method.ica.mu", 1.);
    MandatoryOff("method.ica.mu");

    AddParameter(ParameterType_Choice, "method.ica.g", "Nonlinearity");
    SetParameterDescription("method.ica.g", "Nonlinearity used in the FastICA algorithm");
    AddChoice("method.ica.g.tanh", "tanh");
    SetParameterDescription("method.ica.g.tanh", "g(x) = tanh(x)");
    AddChoice("method.ica.g.exp", "exp");
    SetParameterDescription("method.ica.g.exp", "g(x) = -exp(-x^2/2)");
    AddChoice("method.ica.g.u3", "u^3");
    SetParameterDescription("method.ica.g.u3", "g(x) = u^3(x)");

    AddParameter(ParameterType_Int, "nbcomp", "Number of Components");
    SetParameterDescription("nbcomp", "Number of relevant components kept. By default all components are kept.");
    SetDefaultParameterInt("nbcomp", 0);
    MandatoryOff("nbcomp");
    SetMinimumParameterIntValue("nbcomp", 0);

    AddParameter(ParameterType_Bool, "normalize", "Normalize");
    SetParameterDescription("normalize", "Center and reduce data before Dimensionality reduction.");

    AddParameter(ParameterType_OutputFilename, "outmatrix", "Transformation matrix output (text format)");
    SetParameterDescription("outmatrix", "Filename to store the transformation matrix (csv format)");
    MandatoryOff("outmatrix");
    DisableParameter("outmatrix");
    

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("out", "FilterOutput.tif");
    SetDocExampleParameterValue("method", "pca");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
              if (HasValue("in"))
              {
                     FloatVectorImageType::Pointer inImage = this->GetParameterImage("in");
                     inImage->UpdateOutputInformation();

                     // Update the values of the channels to be selected
                     unsigned int       nbComponents = inImage->GetNumberOfComponentsPerPixel();
                     unsigned int nbComp = static_cast<unsigned int> (GetParameterInt("nbcomp"));
                     if (nbComp > nbComponents)
                     {
                            SetParameterInt("nbcomp",nbComponents);
                            otbAppLogINFO( << "number of selected components can't exceed image dimension : "<<nbComponents );
                     }

              }
              if (GetParameterInt("method")==2) //maf
              {
                     if (this->GetParameterString("outinv").size()!= 0)
                     {
                            otbAppLogWARNING(<<"This application only provides the forward transform for the MAF method.");
                            this->SetParameterString("outinv", "");
                     }
                     this->DisableParameter("outinv");

                     if (this->GetParameterString("outmatrix").size()!= 0)
                     {
                            otbAppLogWARNING(<<"No transformation matrix available for MAF method.");
                            this->SetParameterString("outmatrix", "");
                     }
                     this->DisableParameter("outmatrix");

                     FloatVectorImageType::Pointer inImage = this->GetParameterImage("in");
                     inImage->UpdateOutputInformation();

                     // Update the values of the channels to be selected
                     unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
                     unsigned int nbComp = static_cast<unsigned int> (GetParameterInt("nbcomp"));
                     if ((nbComp != 0) && (nbComp != nbComponents))
                     {
                            SetParameterInt("nbcomp",nbComponents);
                            otbAppLogINFO( << "all components are kept when using MAF filter method.");
                     }

              }
  }

  void DoExecute() override
  {

    // Get Parameters
    int nbComp = GetParameterInt("nbcomp");
    bool normalize = GetParameterInt("normalize");
    bool rescale = IsParameterEnabled("rescale");

    bool invTransform = HasValue("outinv") && IsParameterEnabled("outinv");
    switch (GetParameterInt("method"))
      {
      // PCA Algorithm
      case 0:
        {

        otbAppLogDEBUG( << "PCA Algorithm ");
        PCAForwardFilterType::Pointer filter = PCAForwardFilterType::New();
        m_ForwardFilter = filter;
        PCAInverseFilterType::Pointer invFilter = PCAInverseFilterType::New();
        m_InverseFilter = invFilter;


        filter->SetInput(GetParameterFloatVectorImage("in"));
        filter->SetNumberOfPrincipalComponentsRequired(nbComp);
        filter->SetUseNormalization(normalize);        
        m_ForwardFilter->GetOutput()->UpdateOutputInformation();
        
        if (invTransform)
          {  
          invFilter->SetInput(m_ForwardFilter->GetOutput());
          if (normalize)
            {
            otbAppLogINFO( << "Normalization MeanValue:"<<filter->GetMeanValues() );
            invFilter->SetMeanValues(filter->GetMeanValues());
            // By default normalization by std dev is deactivated in
            //forward filter, and GetStdDevValues() returns an empty
            //vector, which confuses the invFilter.
            //invFilter->SetStdDevValues(filter->GetStdDevValues());
            }

          invFilter->SetTransformationMatrix(filter->GetTransformationMatrix());
          m_TransformationMatrix = invFilter->GetTransformationMatrix();
          }

        m_TransformationMatrix = filter->GetTransformationMatrix();

        break;
        }
      case 1:
        {
        otbAppLogDEBUG( << "NA-PCA Algorithm ");

        // NA-PCA

        unsigned int radiusX = static_cast<unsigned int> (GetParameterInt("method.napca.radiusx"));
        unsigned int radiusY = static_cast<unsigned int> (GetParameterInt("method.napca.radiusy"));

        // Noise filtering
        NoiseFilterType::RadiusType radius = { { radiusX, radiusY } };

        NAPCAForwardFilterType::Pointer filter = NAPCAForwardFilterType::New();
        m_ForwardFilter = filter;
        NAPCAInverseFilterType::Pointer invFilter = NAPCAInverseFilterType::New();
        m_InverseFilter = invFilter;


        filter->SetInput(GetParameterFloatVectorImage("in"));
        filter->SetNumberOfPrincipalComponentsRequired(nbComp);
        filter->SetUseNormalization(normalize);
        filter->GetNoiseImageFilter()->SetRadius(radius);

        m_ForwardFilter->GetOutput()->UpdateOutputInformation();
        
        if (invTransform)
          {
          otbAppLogDEBUG( << "Compute Inverse Transform");
          invFilter->SetInput(m_ForwardFilter->GetOutput());
          otbAppLogINFO( << "Normalization MeanValue:"<<filter->GetMeanValues() );
          invFilter->SetMeanValues(filter->GetMeanValues());
          if (normalize)
            {
            otbAppLogINFO( << "Normalization StdDevValue:"<<filter->GetStdDevValues() );
            invFilter->SetStdDevValues(filter->GetStdDevValues());
            
            }
          invFilter->SetUseNormalization(normalize);
          invFilter->SetTransformationMatrix(filter->GetTransformationMatrix());
          m_TransformationMatrix = invFilter->GetTransformationMatrix();
          }

        m_TransformationMatrix = filter->GetTransformationMatrix();

        break;
        }
      case 2:
        {
        otbAppLogDEBUG( << "MAF Algorithm ");
        MAFForwardFilterType::Pointer filter = MAFForwardFilterType::New();
        m_ForwardFilter = filter;
        filter->SetInput(GetParameterFloatVectorImage("in"));
        otbAppLogINFO( << "V :"<<std::endl<<filter->GetV()<<"Auto-Correlation :"<<std::endl <<filter->GetAutoCorrelation() );

        break;
        }
      case 3:
        {
        otbAppLogDEBUG( << "Fast ICA Algorithm ");

        unsigned int nbIterations = static_cast<unsigned int> (GetParameterInt("method.ica.iter"));
        double mu = static_cast<double> (GetParameterFloat("method.ica.mu"));

        ICAForwardFilterType::Pointer filter = ICAForwardFilterType::New();
        m_ForwardFilter = filter;
        ICAInverseFilterType::Pointer invFilter = ICAInverseFilterType::New();
        m_InverseFilter = invFilter;

        filter->SetInput(GetParameterFloatVectorImage("in"));
        filter->SetNumberOfPrincipalComponentsRequired(nbComp);
        filter->SetNumberOfIterations(nbIterations);
        filter->SetMu(mu);
        
        switch (GetParameterInt("method.ica.g"))
          {
          // tanh
          case 0:
            {
            otbAppLogDEBUG( << "Using tanh nonlinearity");
            auto nonLinearity = [](double x) {return std::tanh(x);};
            auto nonLinearityDerivative = [](double x)
              {return 1-std::pow( std::tanh(x), 2. );};
            filter->SetNonLinearity(nonLinearity, nonLinearityDerivative);
            break;
            }
          // exp
          case 1:
            {
            otbAppLogDEBUG( << "Using u*exp(-u^2/2) nonlinearity");
            auto nonLinearity = [](double x) 
              {return x*std::exp( - 0.5* std::pow(x,2));};
            auto nonLinearityDerivative = [](double x)
              {return (1-std::pow(x,2))*std::exp( - 0.5* std::pow(x,2));};
            filter->SetNonLinearity(nonLinearity, nonLinearityDerivative);
            break;
            }
          // u^3
          case 2:
            {
            otbAppLogDEBUG( << "Using u^3 nonlinearity");
            auto nonLinearity = [](double x) {return std::pow(x,3);};
            auto nonLinearityDerivative = [](double x) 
              {return 3*std::pow(x,2);};
            filter->SetNonLinearity(nonLinearity, nonLinearityDerivative);
            break;
            }
          default:
            {
            otbAppLogFATAL(<<"non defined nonlinearity "<<GetParameterString("method.ica.g")<<std::endl);
            break;
            }
          }
        
        m_ForwardFilter->GetOutput()->UpdateOutputInformation();
        
        if (invTransform)
          {
          otbAppLogDEBUG( << "Compute Inverse Transform");
          invFilter->SetInput(m_ForwardFilter->GetOutput());
          otbAppLogINFO( << "Normalization MeanValue:"<<filter->GetMeanValues() );
          invFilter->SetMeanValues(filter->GetMeanValues());
          otbAppLogINFO( << "Normalization StdDevValue:"<<filter->GetStdDevValues() );
          invFilter->SetStdDevValues(filter->GetStdDevValues());

          invFilter->SetPCATransformationMatrix(filter->GetPCATransformationMatrix());
          invFilter->SetTransformationMatrix(filter->GetTransformationMatrix());
          }

        m_TransformationMatrix = filter->GetTransformationMatrix();

        break;
        }

      default:
        {
        otbAppLogFATAL(<<"non defined method "<<GetParameterInt("method")<<std::endl);
        break;

        }
        return;
      }

    if (invTransform)
      {
      if (GetParameterInt("method") == 2) //MAF
        {
        this->DisableParameter("outinv");
        otbAppLogWARNING(<<"This application only provides the forward transform for the MAF method.");
        }
      else SetParameterOutputImage("outinv", m_InverseFilter->GetOutput());
      }

    //Write transformation matrix
    if (this->GetParameterString("outmatrix").size() != 0)
      {
      if (GetParameterInt("method") == 2) //MAF
        {
        otbAppLogWARNING(<<"No transformation matrix available for MAF.");
        }
      else
        {
        //Write transformation matrix
        std::ofstream outFile;
        outFile.open(this->GetParameterString("outmatrix"));
        outFile << std::fixed;
        outFile.precision(10);

        outFile << m_TransformationMatrix;
        outFile.close();
        }
      }

    if (!rescale)
      {
      SetParameterOutputImage("out", m_ForwardFilter->GetOutput());
      }
    else
      {
      otbAppLogDEBUG( << "Rescaling " )
      otbAppLogDEBUG( << "Starting Min/Max computation" )

      m_MinMaxFilter = MinMaxFilterType::New();
      m_MinMaxFilter->SetInput(m_ForwardFilter->GetOutput());
      //m_MinMaxFilter->GetStreamer()->SetNumberOfLinesStrippedStreaming(50);
      m_MinMaxFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));

      AddProcess(m_MinMaxFilter->GetStreamer(), "Min/Max computing");
      m_MinMaxFilter->Update();

      otbAppLogDEBUG( << "Min/Max computation done : min=" << m_MinMaxFilter->GetMinimum()
          << " max=" << m_MinMaxFilter->GetMaximum() )

      FloatVectorImageType::PixelType inMin, inMax;

      m_RescaleFilter = RescaleImageFilterType::New();
      m_RescaleFilter->SetInput(m_ForwardFilter->GetOutput());
      m_RescaleFilter->SetInputMinimum(m_MinMaxFilter->GetMinimum());
      m_RescaleFilter->SetInputMaximum(m_MinMaxFilter->GetMaximum());

      FloatVectorImageType::PixelType outMin, outMax;
      outMin.SetSize(m_ForwardFilter->GetOutput()->GetNumberOfComponentsPerPixel());
      outMax.SetSize(m_ForwardFilter->GetOutput()->GetNumberOfComponentsPerPixel());
      outMin.Fill(GetParameterFloat("rescale.outmin"));
      outMax.Fill(GetParameterFloat("rescale.outmax"));

      m_RescaleFilter->SetOutputMinimum(outMin);
      m_RescaleFilter->SetOutputMaximum(outMax);
      m_RescaleFilter->UpdateOutputInformation();

      SetParameterOutputImage("out", m_RescaleFilter->GetOutput());
      }


  }

  MinMaxFilterType::Pointer               m_MinMaxFilter;
  RescaleImageFilterType::Pointer         m_RescaleFilter;
  DimensionalityReductionFilter::Pointer  m_ForwardFilter;
  DimensionalityReductionFilter::Pointer  m_InverseFilter;
  MatrixType                              m_TransformationMatrix;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DimensionalityReduction)
