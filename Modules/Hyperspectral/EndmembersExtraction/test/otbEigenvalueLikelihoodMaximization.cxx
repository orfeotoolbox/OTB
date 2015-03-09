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
#include "otbEigenvalueLikelihoodMaximisation.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;

typedef otb::EigenvalueLikelihoodMaximisation<double> ELMType;

int otbEigenvalueLikelihoodMaximizationNewTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  ELMType::Pointer elm = ELMType::New();
  std::cout << elm << std::endl;
  return EXIT_SUCCESS;
}


int otbEigenvalueLikelihoodMaximizationTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  StreamingStatisticsVectorImageFilterType::Pointer stats = StreamingStatisticsVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  stats->SetInput(reader->GetOutput());
  stats->Update();

  ELMType::Pointer elm = ELMType::New();
  elm->SetCovariance(stats->GetCovariance().GetVnlMatrix());
  elm->SetCorrelation(stats->GetCorrelation().GetVnlMatrix());
  elm->SetNumberOfPixels(reader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels());
  elm->Compute();

  std::ofstream file;
  file.open(outfname);
  file.precision(5);
  file << "Number of Endmembers: " << elm->GetNumberOfEndmembers() << std::endl;
  file << "Likelihood: " << elm->GetLikelihood() << std::endl;
  file.close();

  std::cout << "Number of Endmembers: " << elm->GetNumberOfEndmembers() << std::endl;
  std::cout << "Likelihood: " << elm->GetLikelihood() << std::endl;

  return EXIT_SUCCESS;
}
