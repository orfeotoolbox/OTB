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


#include "itkHistogram.h"
#include "otbHistogramStatisticsFunction.h"

int otbHistogramStatisticsFunction(int itkNotUsed(argc), char* argv[])
{
  unsigned int NbOfBins((unsigned int)::atoi(argv[1]));

  typedef float MeasurementType;
  typedef itk::Statistics::Histogram<MeasurementType, itk::Statistics::DenseFrequencyContainer2> HistogramType;
  HistogramType::Pointer histogram = HistogramType::New();

  // initialize histogram
  HistogramType::SizeType size(1);
  size.Fill(NbOfBins);
  HistogramType::MeasurementVectorType lowerBound(1);
  HistogramType::MeasurementVectorType upperBound(1);
  lowerBound[0] = 0.0;
  upperBound[0] = NbOfBins;

  histogram->SetMeasurementVectorSize(1);
  histogram->Initialize(size, lowerBound, upperBound);

  // create histogram with same value for each frequency
  for (HistogramType::Iterator iter = histogram->Begin(); iter != histogram->End(); ++iter)
  {
    iter.SetFrequency(1.0);
  }

  typedef otb::HistogramStatisticsFunction<HistogramType, MeasurementType> HistogramStatisticsFunctionType;
  HistogramStatisticsFunctionType::Pointer HistogramStatisticsFunction = HistogramStatisticsFunctionType::New();

  HistogramStatisticsFunction->SetInputHistogram(histogram.GetPointer());

  MeasurementType Entropy;
  MeasurementType Mean;
  MeasurementType Covariance;

  std::cout << "Update OK : " << std::endl;

  std::cout << "Entropy 1 : " << HistogramStatisticsFunction->GetEntropy()[0] << std::endl;

  Entropy = HistogramStatisticsFunction->GetEntropy()[0];
  std::cout << "Entropy 1 : " << Entropy << std::endl;

  if (std::abs(Entropy - std::log(static_cast<double>(NbOfBins))) > 0.00001)
  {
    std::cout << "Error in entropy estimation" << std::endl;
    return EXIT_FAILURE;
  }

  Mean = HistogramStatisticsFunction->GetMean()[0];
  std::cout << "Mean 1 : " << Mean << std::endl;

  if (Mean != NbOfBins / 2.)
  {
    std::cout << "Error in mean estimation" << std::endl;
    return EXIT_FAILURE;
  }

  Covariance = HistogramStatisticsFunction->GetCovariance()[0];
  std::cout << "Covariance 1 : " << Covariance << std::endl;

  if (Covariance != 0)
  {
    std::cout << "Error in covariance estimation" << std::endl;
    return EXIT_FAILURE;
  }

  // create histogram just all value equal to zero except the first one
  for (HistogramType::Iterator iter = histogram->Begin(); iter != histogram->End(); ++iter)
  {
    if (iter == histogram->Begin())
    {
      iter.SetFrequency(1.0);
    }
    else
    {
      iter.SetFrequency(0.0);
    }
  }

  HistogramStatisticsFunction->Compute();

  Entropy = HistogramStatisticsFunction->GetEntropy()[0];
  std::cout << "Entropy 2 : " << Entropy << std::endl;

  if (Entropy != 0.0)
  {
    std::cout << "Error in entropy estimation" << std::endl;
    return EXIT_FAILURE;
  }

  Mean = HistogramStatisticsFunction->GetMean()[0];
  std::cout << "Mean 2 : " << Mean << std::endl;

  if (Mean != 0.5)
  {
    std::cout << "Error in mean estimation" << std::endl;
    return EXIT_FAILURE;
  }

  Covariance = HistogramStatisticsFunction->GetCovariance()[0];
  std::cout << "Covariance 2 : " << Covariance << std::endl;

  if (Covariance != 0)
  {
    std::cout << "Error in covariance estimation" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
