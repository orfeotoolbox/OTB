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
#include "otbDisparityMapEstimationMethod.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkTranslationTransform.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkGradientDescentOptimizer.h"
#include "otbThresholdImageToPointSetFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

int otbDisparityMapEstimationMethod(int argc, char* argv[])
{
  const char*        fixedFileName = argv[1];
  const char*        movingFileName = argv[2];
  const char*        pointSetFileName = argv[3];
  const char*        outputFileName = argv[4];
  const unsigned int exploSize = atoi(argv[5]);
  const unsigned int winSize = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef itk::TranslationTransform<double, Dimension>                          TransformType;
  typedef TransformType::ParametersType                                         ParametersType;
  typedef itk::PointSet<ParametersType, Dimension>                              PointSetType;
  typedef otb::DisparityMapEstimationMethod<ImageType, ImageType, PointSetType> DMEstimationType;
  typedef itk::NormalizedCorrelationImageToImageMetric<ImageType, ImageType>    MetricType;
  typedef itk::LinearInterpolateImageFunction<ImageType, double>                InterpolatorType;
  typedef itk::GradientDescentOptimizer                                         OptimizerType;
  typedef otb::ImageFileReader<ImageType>                                       ReaderType;
  typedef otb::ThresholdImageToPointSetFilter<ImageType, PointSetType>          PointSetSourceType;
  typedef PointSetType::PointsContainer::Iterator                               PointSetIteratorType;
  typedef PointSetType::PointDataContainer::Iterator                            PointDataIteratorType;

  //Input images reading
  ReaderType::Pointer fixedReader = ReaderType::New();
  ReaderType::Pointer movingReader = ReaderType::New();
  ReaderType::Pointer pointSetReader = ReaderType::New();

  fixedReader->SetFileName(fixedFileName);
  movingReader->SetFileName(movingFileName);
  pointSetReader->SetFileName(pointSetFileName);
  fixedReader->Update();
  movingReader->Update();
  pointSetReader->Update();

  // Ajout
  typedef itk::MinimumMaximumImageCalculator<ImageType> MinMaxType;
  MinMaxType::Pointer mm = MinMaxType::New();
  mm->SetImage(pointSetReader->GetOutput());
  mm->ComputeMinimum();
  mm->ComputeMaximum();
  std::cout << "min: " << (int) mm->GetMinimum() << " max: " << (int) mm->GetMaximum() << std::endl;

  PointSetSourceType::Pointer pointSetSource = PointSetSourceType::New();
  pointSetSource->SetLowerThreshold(mm->GetMaximum());
  pointSetSource->SetUpperThreshold(mm->GetMaximum());
  pointSetSource->SetInput(0, pointSetReader->GetOutput());
  pointSetSource->Update();

  std::cout << "PointSet size: " << pointSetSource->GetOutput()->GetPoints()->Size() << std::endl;

  // Instantiation
  DMEstimationType::Pointer dmestimator = DMEstimationType::New();
  TransformType::Pointer    transform = TransformType::New();
  OptimizerType::Pointer    optimizer = OptimizerType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  MetricType::Pointer       metric = MetricType::New();

  // Set up
  dmestimator->SetTransform(transform);
  dmestimator->SetOptimizer(optimizer);
  dmestimator->SetInterpolator(interpolator);
  dmestimator->SetMetric(metric);

  // For gradient descent
  optimizer->SetLearningRate(5.0);
  optimizer->SetNumberOfIterations(600);
  DMEstimationType::ParametersType initialParameters(transform->GetNumberOfParameters());
  initialParameters[0] = 0.0;  // Initial offset in mm along X
  initialParameters[1] = 0.0;  // Initial offset in mm along Y

  //Initial parameter set up
  // dmestimator->SetInitialTransformParameters(initialParameters);

  // inputs wiring
  ImageType::SizeType win, explo;
  win.Fill(winSize);
  explo.Fill(exploSize);

  dmestimator->SetFixedImage(fixedReader->GetOutput());
  dmestimator->SetMovingImage(movingReader->GetOutput());
  dmestimator->SetPointSet(pointSetSource->GetOutput());
  dmestimator->SetWinSize(win);
  dmestimator->SetExploSize(explo);
  dmestimator->SetInitialTransformParameters(initialParameters);
  // Estimation trigger
  dmestimator->Update();

  // Point set retrieving
  PointSetType::Pointer pointSet = dmestimator->GetOutput();

  // Writing output transform parameters
  std::ofstream out;
  out.open(outputFileName, std::ios::out);
  out.setf(std::ios::fixed);
  // out.setprecision(10);

  PointSetIteratorType it = pointSet->GetPoints()->Begin();
  //     unsigned int idData=0;
  PointDataIteratorType itData = pointSet->GetPointData()->Begin();
  std::cout << "Point data size: " << pointSet->GetPointData()->Size() << std::endl;
  for (;
       it != pointSet->GetPoints()->End()
       && itData != pointSet->GetPointData()->End();
       ++it, ++itData)
    {
    out << "Point " << it.Value() << " -> transform parameters: ";
    out << itData.Value();
    out << std::endl;

    }
  out.close();

  return EXIT_SUCCESS;
}
