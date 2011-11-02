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

#include <boost/algorithm/string.hpp>

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbEigenvalueLikelihoodMaximisation.h"
#include "otbVcaImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbISRAUnmixingImageFilter.h"
#include "otbNCLSUnmixingImageFilter.h"
#include "otbFCLSUnmixingImageFilter.h"

#include "otbVectorImageToMatrixImageFilter.h"


namespace otb
{
namespace Wrapper
{

const unsigned int Dimension = 2;

typedef otb::StreamingStatisticsVectorImageFilter<DoubleVectorImageType> StreamingStatisticsVectorImageFilterType;
typedef otb::EigenvalueLikelihoodMaximisation<double> ELMType;
typedef otb::VCAImageFilter<DoubleVectorImageType> VCAFilterType;

typedef otb::UnConstrainedLeastSquareImageFilter<DoubleVectorImageType, DoubleVectorImageType, double> UCLSUnmixingFilterType;
typedef otb::ISRAUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             ISRAUnmixingFilterType;
typedef otb::NCLSUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             NCLSUnmixingFilterType;
typedef otb::FCLSUnmixingImageFilter<DoubleVectorImageType, DoubleVectorImageType, double>             FCLSUnmixingFilterType;

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
  UnMixingMethod_NONE,
  UnMixingMethod_UCLS,
  UnMixingMethod_ISRA,
  UnMixingMethod_NCLS,
  UnMixingMethod_FCLS,
};

const char* UnMixingMethodNames [] = { "NONE", "UCLS", "ISRA", "NCLS", "FCLS", };


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

  HyperspectralUnmixing()
  {
    SetName("HyperspectralUnmixing");
    SetDescription("Unmix an hyperspectral image");

    // Documentation
    SetDocName("Hyper spectral unmixing application");
    SetDocLongDescription("This application \"unmix\" an hyperspectral image.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample(" ");
    AddDocTag("Image manipulation");
    AddDocTag("Hyperspectral");
  }

  virtual ~HyperspectralUnmixing()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    MandatoryOff("out");

    AddParameter(ParameterType_Choice, "dr", "Dimension reduction");
    AddChoice("dr.pca", "PCA");
    AddChoice("dr.mnf", "MNF");
    AddChoice("dr.napca", "NAPCA");
    MandatoryOff("dr");

    AddParameter(ParameterType_Int, "ne", "Number of endmembers");
    SetDefaultParameterInt("ne", 1);
    MandatoryOff("ne");

    AddParameter(ParameterType_Choice, "de", "Dimensionnality estimation");
    MandatoryOff("de");
    AddChoice("de.elm", "ELM (Eigen Value Likelihood Estimation)");

    AddParameter(ParameterType_Choice, "ee", "Endmembers estimation method");
    MandatoryOff("ee");
    AddChoice("ee.vca", "VCA (Vertex Component Analysis)");

    AddParameter(ParameterType_InputImage, "ie", "Input endmembers image");
    MandatoryOff("ie");

    AddParameter(ParameterType_Choice, "ua", "Unmixing algorithm");
    MandatoryOff("ua");
    AddChoice("ua.none", "None");
    AddChoice("ua.ucls", "UCLS");
    AddChoice("ua.isra", "ISRA");
    AddChoice("ua.ncls", "NCLS");
    AddChoice("ua.fcls", "FCLS");
    //SetParameterString("ua", "none");

    AddParameter(ParameterType_OutputImage, "oe", "Output Endmembers");
    MandatoryOff("oe");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    m_ProcessObjects.clear();

    /*
     *
     * EXTRACT PARAMETERS
     *
     */
    // Dimension Reduction
    DimReductionMethod dimReduction = DimReductionMethod_NONE;
    if ( IsParameterEnabled("dr") && HasValue("dr") )
      {
      std::string dimReductionStr = GetParameterString("dr");
      if ( boost::to_upper_copy(dimReductionStr) == "PCA" )
        {
        dimReduction = DimReductionMethod_PCA;
        }
      else if ( boost::to_upper_copy(dimReductionStr) == "MNF" )
        {
        dimReduction = DimReductionMethod_MNF;
        }
      }
    otbAppLogDEBUG(<< "Using "
                   << (dimReduction == DimReductionMethod_NONE ? "NONE" : (dimReduction == DimReductionMethod_PCA ? "PCA" : "MNF") )
                   << " dimensionality reduction method")

    // Number of endmembers
    unsigned int nbEndmembers = 0;
    if ( IsParameterEnabled("ne") && HasValue("ne") )
      {
      nbEndmembers = GetParameterInt("ne");
      otbAppLogDEBUG(<< "nbEndmembers: " << nbEndmembers)
      }

    // Dimensionnality estimation
    if ( IsParameterEnabled("de") && HasValue("de") )
      {
      std::string dimEstMethodStr = GetParameterString("de");
      if (boost::to_upper_copy(dimEstMethodStr) != "ELM")
        {
        std::cerr << "Only ELM is supported for parameter DimensionalityEstimationMethod" << std::endl;
        }
      otbAppLogDEBUG(<< "dimEstMethodStr: " << dimEstMethodStr)

      }

    if ( IsParameterEnabled("ee") && HasValue("ee") )
      {
      std::string eeEstMethodStr = GetParameterString("ee");
      if (boost::to_upper_copy(eeEstMethodStr) != "VCA")
        {
        std::cerr << "Only VCA is supported for parameter EndmembersEstimationMethod" << std::endl;
        }
      otbAppLogDEBUG(<< "eeEstMethodStr: " << eeEstMethodStr)
      }

    std::string inputEndmembers;
    if ( IsParameterEnabled("ie") && HasValue("ie") )
      {
      inputEndmembers = GetParameterString("ie");
      otbAppLogDEBUG("inputEndmembers: " << inputEndmembers)
      }

    UnMixingMethod unmixingAlgo = UnMixingMethod_NONE;
    if ( IsParameterEnabled("ua") && HasValue("ua") )
      {
      std::string unmixingAlgoStr = GetParameterString("ua");

      if ( boost::to_upper_copy(unmixingAlgoStr) == "UCLS" )
        {
        unmixingAlgo = UnMixingMethod_UCLS;
        }
      else if ( boost::to_upper_copy(unmixingAlgoStr) == "NCLS" )
        {
        unmixingAlgo = UnMixingMethod_NCLS;
        }
      else if ( boost::to_upper_copy(unmixingAlgoStr) == "FCLS" )
        {
        unmixingAlgo = UnMixingMethod_FCLS;
        }
      else if ( boost::to_upper_copy(unmixingAlgoStr) == "ISRA" )
        {
        unmixingAlgo = UnMixingMethod_ISRA;
        }
      }
    otbAppLogDEBUG( << "Using "
                    << UnMixingMethodNames[unmixingAlgo]
                    << " unmixing algorithm")

    std::string outputEndmembers;
    if ( IsParameterEnabled("oe") && HasValue("oe") && inputEndmembers.empty() )
      {
      outputEndmembers = GetParameterString("oe");
      }
    otbAppLogDEBUG( << "IsParameterEnabled(oe) " << IsParameterEnabled("oe") )
    otbAppLogDEBUG( << "HasValue(oe) " << HasValue("oe") )
    otbAppLogDEBUG( << "inputEndmembers.empty() " << inputEndmembers.empty() )
    otbAppLogDEBUG( << "outputEndmembers: " << outputEndmembers )


    /*
     *
     * PROCESSING
     *
     */

    DoubleVectorImageType::Pointer inputImage = GetParameterImage<DoubleVectorImageType>("in");

    DoubleVectorImageType::Pointer endmembersImage;
    if ( inputEndmembers.empty() )
      {
      if( nbEndmembers == 0 )
        {
        /*
         * Compute stats of input image
         */

        std::cout << "Computing stats" << std::endl;

        StreamingStatisticsVectorImageFilterType::Pointer stats = StreamingStatisticsVectorImageFilterType::New();

        stats->SetInput(inputImage);
        otb::StandardWriterWatcher watcher(stats->GetStreamer(), stats->GetFilter(), "Computing image stats");
        stats->Update();

        VectorType mean (stats->GetMean().GetDataPointer(), stats->GetMean().GetSize());
        MatrixType covariance  = stats->GetCovariance().GetVnlMatrix();
        MatrixType correlation = stats->GetCorrelation().GetVnlMatrix();

        /*
         * Estimate Endmembers Numbers
         */
        std::cout << "Estimate Endmembers Numbers by ELM" << std::endl;

        ELMType::Pointer elm = ELMType::New();
        elm->SetCovariance(covariance);
        elm->SetCorrelation(correlation);
        elm->SetNumberOfPixels(inputImage->GetLargestPossibleRegion().GetNumberOfPixels());
        elm->Compute();

        nbEndmembers = elm->GetNumberOfEndmembers();

        std::cout << "ELM : " << nbEndmembers << " estimated endmembers" << std::endl;

        std::cout << "ELM likelihood values: " << elm->GetLikelihood() << std::endl;

        }
      else
        {
        std::cout << "Using " << nbEndmembers << " endmembers" << std::endl;
        }

      /*
       * Estimate Endmembers
       */
      std::cout << "Estimate Endmembers by VCA" << std::endl;

      VCAFilterType::Pointer vca = VCAFilterType::New();
      vca->SetNumberOfEndmembers(nbEndmembers);
      vca->SetInput(inputImage);

      endmembersImage = vca->GetOutput();
      m_ProcessObjects.push_back(vca.GetPointer());
      }
    else
      {
      /*
       * Read input endmembers
       */
      std::cout << "Read Endmembers " << inputEndmembers << std::endl;
      endmembersImage = GetParameterImage<DoubleVectorImageType>("ie");

      }
  //  endmembersRef->Update();

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
    switch (unmixingAlgo)
    {
    case UnMixingMethod_NONE:
      break;
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
    default:
      break;
    }

    if ( !outputEndmembers.empty() )
      {
      /*
       * Write endmembers
       */
      std::cout << "Write endmembers " << outputEndmembers << std::endl;

      SetParameterOutputImage<DoubleVectorImageType>("oe", endmembersImage);
      }

    if ( unmixingAlgo != UnMixingMethod_NONE )
      {
      /*
       * Write abundance map
       */
      //std::cout << "Write abundance map" << outputImageName << std::endl;

      SetParameterOutputImage<DoubleVectorImageType>("out", abundanceMap);
      }
  }

  std::vector<itk::ProcessObject::Pointer> m_ProcessObjects;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::HyperspectralUnmixing)
