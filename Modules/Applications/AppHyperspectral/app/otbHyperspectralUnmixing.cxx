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

#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbISRAUnmixingImageFilter.h"
#include "otbMDMDNMFImageFilter.h"


namespace otb
{
namespace Wrapper
{
typedef otb::UnConstrainedLeastSquareImageFilter<DoubleVectorImageType, DoubleVectorImageType, double> UCLSUnmixingFilterType;
typedef otb::ISRAUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             ISRAUnmixingFilterType;
typedef otb::MDMDNMFImageFilter<DoubleVectorImageType, DoubleVectorImageType>                          MDMDNMFUnmixingFilterType;

typedef otb::VectorImageToMatrixImageFilter<DoubleVectorImageType> VectorImageToMatrixImageFilterType;

typedef vnl_vector<double> VectorType;
typedef vnl_matrix<double> MatrixType;


enum DimReductionMethod
{
  DimReductionMethod_NONE,
  DimReductionMethod_PCA,
  DimReductionMethod_MNF
};

enum DimensionalityEstimationMethod
{
  DimensionalityEstimationMethod_ELM
};

enum EndmembersEstimationMethod
{
  EndmembersEstimationMethod_VCA
};

enum UnMixingMethod
{
  UnMixingMethod_UCLS,
  //UnMixingMethod_FCLS,
  // UnMixingMethod_NCLS,
  UnMixingMethod_ISRA,
  UnMixingMethod_MDMDNMF,
};

const char* UnMixingMethodNames [] = { "UCLS", "ISRA", "MDMDNMF", };


class HyperspectralUnmixing : public Application
{
public:
  /** Standard class typedefs. */
  typedef HyperspectralUnmixing         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(HyperspectralUnmixing, otb::Application);

private:
  void DoInit() override
  {
    SetName("HyperspectralUnmixing");
    SetDescription("Estimate abundance maps from an hyperspectral image and a set of endmembers.");

    // Documentation
    SetDocLongDescription("The application applies a linear unmixing algorithm "
    "to an hyperspectral data cube. This method supposes that the mixture between "
    "aterials in the scene is macroscopic and simulates a linear mixing model of "
    "spectra.\n\n"
    "The Linear Mixing Model (LMM) acknowledges that reflectance "
    "spectrum associated with each pixel is a linear combination of pure "
    "materials in the recovery area, commonly known as endmembers. Endmembers can "
    "be estimated using the VertexComponentAnalysis application.\n\n"
    "The application allows estimating the abundance maps with several algorithms:\n\n"
    "* Unconstrained Least Square (ucls)\n"
    "* Image Space Reconstruction Algorithm (isra)\n"
    "* Least Square (ncls)\n"
    "* Minimum Dispersion Constrained Non Negative Matrix Factorization (MDMDNMF)."
    );
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("VertexComponentAnalysis");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image Filename");
    SetParameterDescription("in","The hyperspectral data cube input");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out","The output abundance map. The abundance "
      "fraction are stored in a multispectral image where band N corresponds "
      "to the fraction of endmembers N in each pixel.");

    AddParameter(ParameterType_InputImage,  "ie",   "Input endmembers");
    SetParameterDescription("ie","The endmembers (estimated pure pixels) to "
      "use for unmixing. Must be stored as a multispectral image, where "
      "each pixel is interpreted as an endmember.");

    AddParameter(ParameterType_Choice, "ua", "Unmixing algorithm");
    SetParameterDescription("ua", "The algorithm to use for unmixing");
    MandatoryOff("ua");
    AddChoice("ua.ucls", "UCLS");
    SetParameterDescription("ua.ucls", "Unconstrained Least Square");

    AddChoice("ua.isra", "ISRA");
    SetParameterDescription("ua.isra", "Image Space Reconstruction Algorithm");

    AddChoice("ua.mdmdnmf", "MDMDNMF");
    SetParameterDescription("ua.mdmdnmf", "Minimum Dispersion Constrained Non Negative Matrix Factorization");
    SetParameterString("ua", "ucls");
    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("ie", "cupriteEndmembers.tif");
    SetDocExampleParameterValue("out", "HyperspectralUnmixing.tif double");
    SetDocExampleParameterValue("ua", "ucls");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    m_ProcessObjects.clear();

    DoubleVectorImageType::Pointer inputImage = GetParameterDoubleVectorImage("in");
    DoubleVectorImageType::Pointer endmembersImage = GetParameterDoubleVectorImage("ie");

    /*
     * Transform Endmembers image to matrix representation
     */
    otbAppLogINFO("Endmembers extracted");
    otbAppLogINFO("Converting endmembers to matrix");
    VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
    endMember2Matrix->SetInput(endmembersImage);
    endMember2Matrix->Update();

    MatrixType endMembersMatrix = endMember2Matrix->GetMatrix();
    otbAppLogDEBUG("Endmembers matrix : " << endMembersMatrix);

    /*
     * Unmix
     */
    DoubleVectorImageType::Pointer abundanceMap;

    switch ( static_cast<UnMixingMethod>(GetParameterInt("ua")) )
    {
    case UnMixingMethod_UCLS:
      {
      otbAppLogINFO("UCLS Unmixing");

      UCLSUnmixingFilterType::Pointer unmixer =
          UCLSUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->GetModifiableFunctor().SetMatrix(endMembersMatrix);
      unmixer->SetNumberOfThreads(1); // FIXME : currently buggy

      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
    case UnMixingMethod_ISRA:
      {
      otbAppLogINFO("ISRA Unmixing");

      ISRAUnmixingFilterType::Pointer unmixer =
          ISRAUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->GetModifiableFunctor().SetEndmembersMatrix(endMembersMatrix);
      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
    case UnMixingMethod_MDMDNMF:
      {
      otbAppLogINFO("MDMD-NMF Unmixing");

      MDMDNMFUnmixingFilterType::Pointer unmixer =
          MDMDNMFUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->SetEndmembersMatrix(endMembersMatrix);
      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
    default:
      break;
    }

    SetParameterOutputImage<DoubleVectorImageType>("out", abundanceMap);

  }

  std::vector<itk::ProcessObject::Pointer> m_ProcessObjects;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::HyperspectralUnmixing)
