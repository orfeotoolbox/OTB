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



#include "itkHistogram.h"
#include "otbHistogramStatisticsFunction.h"

int otbHistogramStatisticsFunction(int argc, char* argv[])
{
  unsigned int NbOfBins((unsigned int) ::atoi(argv[1]));

  typedef float                                          MeasurementType;
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

  std::cout << "Update OK : " <<  std::endl;

  std::cout << "Entropy 1 : " << HistogramStatisticsFunction->GetEntropy()[0] << std::endl;

  Entropy = HistogramStatisticsFunction->GetEntropy()[0];
  std::cout << "Entropy 1 : " << Entropy << std::endl;

  if (vcl_abs(Entropy - vcl_log(static_cast<double>(NbOfBins))) > 0.00001)
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

  HistogramStatisticsFunction->Update();

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
