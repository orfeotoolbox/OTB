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

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbISRAUnmixingImageFilter.h"
#include "otbNCLSUnmixingImageFilter.h"
//#include "otbFCLSUnmixingImageFilter.h"
#include "otbMDMDNMFImageFilter.h"


namespace otb
{
namespace Wrapper
{

typedef otb::StreamingStatisticsVectorImageFilter<DoubleVectorImageType> StreamingStatisticsVectorImageFilterType;

typedef otb::UnConstrainedLeastSquareImageFilter<DoubleVectorImageType, DoubleVectorImageType, double> UCLSUnmixingFilterType;
typedef otb::ISRAUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             ISRAUnmixingFilterType;
typedef otb::NCLSUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             NCLSUnmixingFilterType;
//typedef otb::FCLSUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             FCLSUnmixingFilterType;
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
  UnMixingMethod_NCLS,
  UnMixingMethod_ISRA,
  UnMixingMethod_MDMDNMF,
};

const char* UnMixingMethodNames [] = { "UCLS", "FCLS", "NCLS", "ISRA", "MDMDNMF", };


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
  void DoInit()
  {
    SetName("HyperspectralUnmixing");
    SetDescription("Estimate abundance maps from an hyperspectral image and a set of endmembers.");

    // Documentation
    SetDocName("Hyperspectral data unmixing");
    SetDocLongDescription("The application applies a linear unmixing algorithm to an hyperspectral data cube. This method supposes that the mixture between materials in the scene is macroscopic and simulates a linear mixing model of spectra.\nThe Linear Mixing Model (LMM) acknowledges that reflectance spectrum associated with each pixel is a linear combination of pure materials in the recovery area, commonly known as endmembers. Endmembers can be estimated using the VertexComponentAnalysis application.\nThe application allows one to estimate the abundance maps with several algorithms : Unconstrained Least Square (ucls), Fully Constrained Least Square (fcls), Image Space Reconstruction Algorithm (isra) and Non-negative constrained Least Square (ncls) and Minimum Dispersion Constrained Non Negative Matrix Factorization (MDMDNMF).\n");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("VertexComponentAnalysis");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image Filename");
    SetParameterDescription("in","The hyperspectral data cube to unmix");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out","The output abundance map");

    AddParameter(ParameterType_InputImage,  "ie",   "Input endmembers");
    SetParameterDescription("ie","The endmembers (estimated pure pixels) to use for unmixing. Must be stored as a multispectral image, where each pixel is interpreted as an endmember");

    AddParameter(ParameterType_Choice, "ua", "Unmixing algorithm");
    SetParameterDescription("ua", "The algorithm to use for unmixing");
    MandatoryOff("ua");
    AddChoice("ua.ucls", "UCLS");
    SetParameterDescription("ua.ucls", "Unconstrained Least Square");

//    AddChoice("ua.fcls", "FCLS");
//    SetParameterDescription("ua.fcls", "Fully constrained Least Square");

    AddChoice("ua.ncls", "NCLS");
    SetParameterDescription("ua.ncls", "Non-negative constrained Least Square");

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
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    m_ProcessObjects.clear();

    DoubleVectorImageType::Pointer inputImage = GetParameterDoubleVectorImage("in");
    DoubleVectorImageType::Pointer endmembersImage = GetParameterDoubleVectorImage("ie");

    /*
     * Transform Endmembers image to matrix representation
     */
    std::cout << "Endmembers extracted" << std::endl;
    std::cout << "Converting endmembers to matrix" << std::endl;
    VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
    endMember2Matrix->SetInput(endmembersImage);
    endMember2Matrix->Update();

    MatrixType endMembersMatrix = endMember2Matrix->GetMatrix();
    std::cout << "Endmembers matrix : " << endMembersMatrix << std::endl;

    /*
     * Unmix
     */
    DoubleVectorImageType::Pointer abundanceMap;

    switch ( static_cast<UnMixingMethod>(GetParameterInt("ua")) )
    {
    case UnMixingMethod_UCLS:
      {
      std::cout << "UCLS Unmixing" << std::endl;

      UCLSUnmixingFilterType::Pointer unmixer =
          UCLSUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->SetMatrix(endMembersMatrix);
      unmixer->SetNumberOfThreads(1); // FIXME : currently buggy

      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
    case UnMixingMethod_ISRA:
      {
      std::cout << "ISRA Unmixing" << std::endl;

      ISRAUnmixingFilterType::Pointer unmixer =
          ISRAUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->SetEndmembersMatrix(endMembersMatrix);
      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
    case UnMixingMethod_NCLS:
      {
      std::cout << "NCLS Unmixing" << std::endl;

      NCLSUnmixingFilterType::Pointer unmixer =
          NCLSUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->SetEndmembersMatrix(endMembersMatrix);
      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
      /*
    case UnMixingMethod_FCLS:
      {
      std::cout << "FCLS Unmixing" << std::endl;

      FCLSUnmixingFilterType::Pointer unmixer =
          FCLSUnmixingFilterType::New();

      unmixer->SetInput(inputImage);
      unmixer->SetEndmembersMatrix(endMembersMatrix);
      abundanceMap = unmixer->GetOutput();
      m_ProcessObjects.push_back(unmixer.GetPointer());

      }
      break;
      */
    case UnMixingMethod_MDMDNMF:
      {
      std::cout << "MDMD-NMF Unmixing" << std::endl;

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
