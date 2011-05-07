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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include "otbCommandLineArgumentParser.h"
#include <boost/algorithm/string.hpp>

#include "otbStreamingStatisticsVectorImageFilter.h"

#include "otbEigenvalueLikelihoodMaximisation.h"
#include "otbVcaImageFilter.h"

#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbISRAUnmixingImageFilter.h"
#include "otbNCLSUnmixingImageFilter.h"
#include "otbFCLSUnmixingImageFilter.h"
#include "otbCLSPSTOUnmixingImageFilter.h"

#include "otbVectorImageToMatrixImageFilter.h"

#include "otbStandardWriterWatcher.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;

typedef otb::StreamingStatisticsVectorImageFilter<VectorImageType, PixelType> StreamingStatisticsVectorImageFilterType;
typedef otb::EigenvalueLikelihoodMaximisation<PixelType> ELMType;
typedef otb::VCAImageFilter<VectorImageType> VCAFilterType;

typedef otb::UnConstrainedLeastSquareImageFilter<VectorImageType,VectorImageType,PixelType> UCLSUnmixingFilterType;
typedef otb::ISRAUnmixingImageFilter<VectorImageType,VectorImageType,PixelType>             ISRAUnmixingFilterType;
typedef otb::NCLSUnmixingImageFilter<VectorImageType,VectorImageType,PixelType>             NCLSUnmixingFilterType;
typedef otb::FCLSUnmixingImageFilter<VectorImageType,VectorImageType,PixelType>             FCLSUnmixingFilterType;
typedef otb::CLSPSTOUnmixingImageFilter<VectorImageType,VectorImageType,PixelType>          CLSPSTOUnmixingFilterType;

typedef otb::VectorImageToMatrixImageFilter<VectorImageType> VectorImageToMatrixImageFilterType;

typedef vnl_vector<PixelType> VectorType;
typedef vnl_matrix<PixelType> MatrixType;



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
  UnMixingMethod_CLSPSTO
};

const char* UnMixingMethodNames [] = { "NONE", "UCLS", "ISRA", "NCLS", "FCLS", "CLSPSTO" };

int main(int argc, char * argv[])
{
  /*
   *
   * DEFINE APPLICATION
   *
   */
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Performs Hyperspectral Unmixing");
  parser->AddInputImage();
  parser->AddOption( "--DimReduceMethod", "Dimensionnality reduction method (NONE,PCA,MCF)", "-dr", 1, false );
  parser->AddOption( "--NumEndmembers", "Number of endmembers (if not given or 0, will be estimated with DimensionalityEstimationMethod)", "-ne", 1, false );
  parser->AddOption( "--DimensionalityEstimationMethod", "Dimensionality estimation method (ELM)", "-de", 1, false );
  parser->AddOption( "--EndmembersEstimationMethod", "Endmembers estimation method (VCA)", "-ee", 1, false );
  parser->AddOption( "--InputEndmembers", "Input endmembers image (must have NumComponents components or fit the input image nb of components)", "-ie", 1, false );
  parser->AddOption( "--UnmixingAlgorithm", "Unmixing algorithm (NONE, UCLS, ISRA, NCLS, FCLS, CLSPSTO)", "-ua", 1, false );
  parser->AddOption( "--OutputEndmembers", "Output estimated endmembers image", "-oe", 1, false );
  parser->AddOutputImage();

  /*
   *
   * EXTRACT PARAMETERS
   *
   */
  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    if(descriptionException.find("ParseCommandLine(): Version Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  const char * inputImageName = parseResult->GetInputImage().c_str();
  const char * outputImageName = parseResult->GetOutputImage().c_str();

  DimReductionMethod dimReduction = DimReductionMethod_NONE;
  if ( parseResult->IsOptionPresent("--DimReduceMethod") )
    {
    std::string dimReductionStr = parseResult->GetParameterString("--DimReduceMethod");
    if ( boost::to_upper_copy(dimReductionStr) == "PCA" )
      {
      dimReduction = DimReductionMethod_PCA;
      }
    else if ( boost::to_upper_copy(dimReductionStr) == "MNF" )
      {
      dimReduction = DimReductionMethod_MNF;
      }
    }
  otbMsgDevMacro( << "Using "
                  << (dimReduction == DimReductionMethod_NONE ? "NONE" : (dimReduction == DimReductionMethod_PCA ? "PCA" : "MNF") )
                  << " dimensionality reduction method" );

  unsigned int nbEndmembers = 0;
  if ( parseResult->IsOptionPresent("--NumEndmembers") )
    {
    nbEndmembers = parseResult->GetParameterUInt("--NumEndmembers");
    }

  if ( parseResult->IsOptionPresent("--DimensionalityEstimationMethod") )
    {
    std::string dimEstMethodStr = parseResult->GetParameterString("--DimensionalityEstimationMethod");
    if (boost::to_upper_copy(dimEstMethodStr) != "ELM")
      {
      std::cerr << "Only ELM is supported for parameter DimensionalityEstimationMethod" << std::endl;
      return EXIT_FAILURE;
      }
    }

  if ( parseResult->IsOptionPresent("--EndmembersEstimationMethod") )
    {
    std::string eeEstMethodStr = parseResult->GetParameterString("--EndmembersEstimationMethod");
    if (boost::to_upper_copy(eeEstMethodStr) != "VCA")
      {
      std::cerr << "Only VCA is supported for parameter EndmembersEstimationMethod" << std::endl;
      return EXIT_FAILURE;
      }
    }

  const char * inputEndmembers = parseResult->IsOptionPresent("--InputEndmembers") ?
    parseResult->GetParameterString("--InputEndmembers").c_str() : NULL;

  UnMixingMethod unmixingAlgo = UnMixingMethod_FCLS;
  if ( parseResult->IsOptionPresent("--UnmixingAlgorithm") )
    {
    std::string unmixingAlgoStr = parseResult->GetParameterString("--UnmixingAlgorithm");
    if ( boost::to_upper_copy(unmixingAlgoStr) == "UCLS" )
      {
      unmixingAlgo = UnMixingMethod_UCLS;
      }
    else if ( boost::to_upper_copy(unmixingAlgoStr) == "ISRA" )
      {
      unmixingAlgo = UnMixingMethod_ISRA;
      }
    }
  otbMsgDevMacro( << "Using "
                  << UnMixingMethodNames[unmixingAlgo]
                  << " unmixing algorithm" );

  const char * outputEndmembers = parseResult->IsOptionPresent("--OutputEndmembers") ?
    parseResult->GetParameterString("--OutputEndmembers").c_str() : NULL;

  // Don't write endmembers if they are given as input
  if ( inputEndmembers )
    outputEndmembers = NULL;


  /*
   *
   * PROCESSING
   *
   */

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImageName);

  VectorImageType::Pointer inputImage = readerImage->GetOutput();


  VectorImageType::Pointer endmembersImage;
  itk::ProcessObject::Pointer endmembersRef;

  if ( inputEndmembers == NULL )
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
    endmembersRef = vca;
    }
  else
    {
    /*
     * Read input endmembers
     */
    std::cout << "Read Endmembers " << inputEndmembers << std::endl;

    ReaderType::Pointer readerEndmembers = ReaderType::New();
    readerEndmembers->SetFileName(inputEndmembers);
    endmembersImage = readerEndmembers->GetOutput();

    endmembersRef = readerEndmembers;
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
  VectorImageType::Pointer abundanceMap;
  itk::ProcessObject::Pointer unmixerRef;

  switch (unmixingAlgo)
  {
  case UnMixingMethod_NONE:
    break;
  case UnMixingMethod_UCLS:
    {
    std::cout << "UCLS Unmixing" << std::endl;

    UCLSUnmixingFilterType::Pointer unmixer =
        UCLSUnmixingFilterType::New();

    unmixer->SetInput(readerImage->GetOutput());
    unmixer->SetMatrix(endMembersMatrix);
    unmixer->SetNumberOfThreads(1); // FIXME : currently buggy

    unmixerRef = unmixer;
    abundanceMap = unmixer->GetOutput();
    }
    break;
  case UnMixingMethod_ISRA:
    {
    std::cout << "ISRA Unmixing" << std::endl;

    ISRAUnmixingFilterType::Pointer unmixer =
        ISRAUnmixingFilterType::New();

    unmixer->SetInput(readerImage->GetOutput());
    unmixer->SetEndmembersMatrix(endMembersMatrix);
    unmixerRef = unmixer;
    abundanceMap = unmixer->GetOutput();
    }
    break;
  case UnMixingMethod_NCLS:
    {
    std::cout << "NCLS Unmixing" << std::endl;

    NCLSUnmixingFilterType::Pointer unmixer =
        NCLSUnmixingFilterType::New();

    unmixer->SetInput(readerImage->GetOutput());
    unmixer->SetEndmembersMatrix(endMembersMatrix);
    unmixerRef = unmixer;
    abundanceMap = unmixer->GetOutput();
    }
    break;
  case UnMixingMethod_FCLS:
    {
    std::cout << "FCLS Unmixing" << std::endl;

    FCLSUnmixingFilterType::Pointer unmixer =
        FCLSUnmixingFilterType::New();

    unmixer->SetInput(readerImage->GetOutput());
    unmixer->SetEndmembersMatrix(endMembersMatrix);
    unmixerRef = unmixer;
    abundanceMap = unmixer->GetOutput();
    }
    break;
  case UnMixingMethod_CLSPSTO:
    {
    std::cout << "CLSPSTO Unmixing" << std::endl;

    CLSPSTOUnmixingFilterType::Pointer unmixer =
        CLSPSTOUnmixingFilterType::New();

    unmixer->SetInput(readerImage->GetOutput());
    unmixer->SetEndmembersMatrix(endMembersMatrix);
    unmixerRef = unmixer;
    abundanceMap = unmixer->GetOutput();
    }
    break;
  default:
    break;
  }

  if ( outputEndmembers != NULL )
    {
    /*
     * Write endmembers
     */
    std::cout << "Write endmembers " << outputEndmembers << std::endl;
    WriterType::Pointer endmembersWriter = WriterType::New();
    endmembersWriter->SetFileName(outputEndmembers);
    endmembersWriter->SetInput(endmembersImage);
    endmembersWriter->Update();
    }

  if ( unmixingAlgo != UnMixingMethod_NONE )
    {
    /*
     * Write abundance map
     */
    std::cout << "Write abundance map" << outputImageName << std::endl;


    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputImageName);
    writer->SetInput(abundanceMap);
    otb::StandardWriterWatcher watcher(writer,unmixerRef,"Unmixing");
    writer->Update();
    }

  return EXIT_SUCCESS;
}
