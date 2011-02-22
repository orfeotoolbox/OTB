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

#include "otbStreamingStatisticsVectorImageFilter2.h"
#include "otbEigenvalueLikelihoodMaximisation.h"
#include "otbVcaImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbISRAUnmixingImageFilter.h"

const unsigned int Dimension = 2;
typedef float PixelType;

typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;

typedef otb::StreamingStatisticsVectorImageFilter2<VectorImageType> StreamingStatisticsVectorImageFilterType;
typedef otb::EigenvalueLikelihoodMaximisation<PixelType> ELMType;
typedef otb::VCAImageFilter<VectorImageType> VCAFilterType;

typedef enum DimReduction
{
  DimReduction_NONE,
  DimReduction_PCA,
  DimReduction_MNF
};

typedef enum UnMixingAlgo
{
  UnMixingAlgo_UCLS,
  UnMixingAlgo_ISRA,
};

int main(int argc, char * argv[])
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Performs Hyperspectral Unmixing");
  parser->AddInputImage();
  parser->AddOption( "--DimReduceMethod", "Dimensionnality reduction method (NONE,PCA,MCF)", "-dr", 1, false );
  parser->AddOption( "--NumEndmembers", "Number of endmembers (if not given or 0, will be estimated with DimensionalityEstimationMethod)", "-ne", 1, false );
  parser->AddOption( "--DimensionalityEstimationMethod", "Dimensionality estimation method (ELM)", "-de", 1, false );
  parser->AddOption( "--EndmembersEstimationMethod", "Endmembers estimation method (VCA)", "-ee", 1, false );
  parser->AddOption( "--InputEndmembers", "Input endmembers image (must have NumComponents components or fit the input image nb of components)", "-ie", 1, false );
  parser->AddOption( "--UnmixingAlgorithm", "Unmixing algorithm (UCLS, ISRA)", "-ua", 1, false );
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
    //std::cerr << argv[0] << " applies MNF transformations\n";
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

  DimReduction dimReduction = DimReduction_NONE;
  if ( parseResult->IsOptionPresent("--DimReduceMethod") )
    {
    std::string dimReductionStr = parseResult->GetParameterString("--DimReduceMethod");
    if ( boost::to_upper(dimReductionStr) == "PCA" )
      {
      dimReduction = DimReduction_PCA;
      }
    else if ( boost::to_upper(dimReductionStr) == "MNF" )
      {
      dimReduction = DimReduction_MNF;
      }
    }
  otbMsgDevMacro(<< "Using "
                 << (dimReduction == DimReduction_NONE ? "NONE" : (dimReduction == DimReduction_PCA ? : "PCA" : DimReduction_MNF) )
                 << " dimensionality reduction method"
                 );

  unsigned int nbEndmembers = 0;
  if ( parseResult->IsOptionPresent("--NumEndmembers") )
    {
    nbEndmembers = parseResult->GetParameterUInt("--NumEndmembers");
    }

  if ( parseResult->IsOptionPresent("--DimensionalityEstimationMethod") )
    {
    std::string dimEstMethodStr = parseResult->GetParameterString("--DimensionalityEstimationMethod");
    if (boost::to_upper(dimEstMethodStr) != "ELM")
      {
      std::cerr << "Only ELM is supported for parameter DimensionalityEstimationMethod" << std::endl;
      return EXIT_FAILURE;
      }
    }

  if ( parseResult->IsOptionPresent("--EndmembersEstimationMethod") )
    {
    std::string eeEstMethodStr = parseResult->GetParameterString("--EndmembersEstimationMethod");
    if (boost::to_upper(eeEstMethodStr) != "VCA")
      {
      std::cerr << "Only VCA is supported for parameter EndmembersEstimationMethod" << std::endl;
      return EXIT_FAILURE;
      }
    }

  const char * inputEndmembers = parseResult->IsOptionPresent("--InputEndmembers") ?
    parseResult->GetParameterString("--InputEndmembers").c_str() : NULL;

  UnMixingAlgo unmixingAlgo = UnMixingAlgo_UCLS;
  if ( parseResult->IsOptionPresent("--DimReduceMethod") )
    {
    std::string unmixingAlgoStr = parseResult->GetParameterString("--DimReduceMethod");
    if ( boost::to_upper(unmixingAlgoStr) == "UCLS" )
      {
      unmixingAlgo = UnMixingAlgo_UCLS;
      }
    else if ( boost::to_upper(unmixingAlgoStr) == "ISRA" )
      {
      unmixingAlgo = UnMixingAlgo_ISRA;
      }
    }
  otbMsgDevMacro(<< "Using "
                 << (unmixingAlgo == UnMixingAlgo_UCLS ? "UCLS" : "ISRA" )
                 << " unmixing algorithm"
                 );

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


  /*
   * Compute stats of input image, we will need this all along the road
   */

  if ( inputEndmembers == NULL )
    {
    if( nbEndmembers == 0 )
      {
      /*
       * Estimate Endmembers Numbers
       */

      }

    /*
     * Estimate Endmembers
     */

    }
  else
    {
    /*
     * Read input endmembers
     */


    }


  /*
   * Unmix
   */


  /*
   * Write abundance map
   */
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageName);
  //writer->SetInput(scale->GetOutput());
  writer->Update();


  if ( outputEndmembers != NULL )
    {
    /*
     * Write endmembers
     */
    WriterType::Pointer endmembersWriter = WriterType::New();
    endmembersWriter->SetFileName(outputEndmembers);
    //endmembersWriter->SetInput(scale->GetOutput());
    endmembersWriter->Update();
    }


  return EXIT_SUCCESS;
}
