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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkWindowedSincInterpolateImageFunction.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkGradientDescentOptimizer.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbWarpImageFilter.h"
#include "otbNearestPointDeformationFieldGenerator.h"
#include "otbNNearestPointsLinearInterpolateDeformationFieldGenerator.h"
#include "otbBSplinesInterpolateDeformationFieldGenerator.h"
#include "otbNearestTransformDeformationFieldGenerator.h"
#include "otbNNearestTransformsLinearInterpolateDeformationFieldGenerator.h"
#include "otbBSplinesInterpolateTransformDeformationFieldGenerator.h"

int otbCenteredRigidDeformationFieldEstimation(int argc, char* argv[])
{
  if (argc != 13)
    {
    std::cout << "usage: " << argv[0] <<
    " fixedFileName movingFileName outputFileName explorationSize windowSize learningRate numberOfIterations metricThreshold nbPointToInterpolate pontsetStep centerx centery"
              << std::endl;
    return EXIT_SUCCESS;
    }

  // Input parameters
  const char*        fixedFileName = argv[1];
  const char*        movingFileName = argv[2];
  const char*        outputFileNamePrefix = argv[3];
  const unsigned int exploSize = atoi(argv[4]);
  const unsigned int winSize = atoi(argv[5]);
  const double       learningRate = atof(argv[6]);
  const unsigned int niterations = atoi(argv[7]);
  const double       metricThreshold = atof(argv[8]);
  const unsigned int nbPointsToInterpolate = atoi(argv[9]);
  const unsigned int step = atoi(argv[10]);
  const double       centerx = atof(argv[11]);
  const double       centery = atof(argv[12]);

  // 0. DEFINTIONS

  // Images definition
  const unsigned int Dimension = 2;
  typedef double                              PixelType;
  typedef otb::Image<PixelType, Dimension>    ImageType;
  typedef otb::Image<PixelType, Dimension>    OutputImageType;
  typedef ImageType::IndexType                IndexType;
  typedef ImageType::SizeType                 SizeType;
  typedef otb::VectorImage<double, Dimension> DeformationFieldType;

  // Transform definition
  typedef itk::CenteredRigid2DTransform<double> TransformType;
  typedef TransformType::ParametersType         ParametersType;

  // Pointset definition
  typedef itk::PointSet<ParametersType, Dimension>   PointSetType;
  typedef PointSetType::PointType                    PointType;
  typedef PointSetType::PointsContainer::Iterator    PointSetIteratorType;
  typedef PointSetType::PointsContainer              PointsContainerType;
  typedef PointSetType::PointDataContainer::Iterator PointDataIteratorType;
  typedef PointSetType::PointDataContainer           PointDataContainerType;

  // Disparity map estimator definition
  typedef otb::DisparityMapEstimationMethod<ImageType, ImageType, PointSetType> DMEstimationType;

  // Metric definition
  typedef itk::NormalizedCorrelationImageToImageMetric<ImageType, ImageType> MetricType;

  // Interpolator definition
  typedef itk::Function::HammingWindowFunction<3>
  WindowFunctionType;
  typedef itk::ZeroFluxNeumannBoundaryCondition<ImageType>
  ConditionType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 3, WindowFunctionType, ConditionType,
      double> InterpolatorType;

  // Optimizer definition
  typedef itk::GradientDescentOptimizer OptimizerType;

  // IO definition
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>      WriterType;
  typedef otb::ImageFileWriter<DeformationFieldType> DeformationFieldWriterType;

  // Additional filters
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> RescalerType;

  // Deformation fields generator
  typedef otb::NearestPointDeformationFieldGenerator<PointSetType,
      DeformationFieldType>
  NearestPointGeneratorType;
  typedef otb::NNearestPointsLinearInterpolateDeformationFieldGenerator<PointSetType,
      DeformationFieldType>
  NNearestPointGeneratorType;
  typedef otb::BSplinesInterpolateDeformationFieldGenerator<PointSetType,
      DeformationFieldType> BSplinesGeneratorType;
  typedef otb::NearestTransformDeformationFieldGenerator<PointSetType,
      DeformationFieldType>
  NearestTransformGeneratorType;
  typedef otb::NNearestTransformsLinearInterpolateDeformationFieldGenerator<PointSetType,
      DeformationFieldType>
  NNearestTransformGeneratorType;
  typedef otb::BSplinesInterpolateTransformDeformationFieldGenerator<PointSetType,
      DeformationFieldType>
  BSplinesTransformGeneratorType;

  // Warper
  typedef otb::WarpImageFilter<ImageType, ImageType, DeformationFieldType> ImageWarperType;

  //Input images reading
  ReaderType::Pointer fixedReader = ReaderType::New();
  ReaderType::Pointer movingReader = ReaderType::New();

  fixedReader->SetFileName(fixedFileName);
  movingReader->SetFileName(movingFileName);
  fixedReader->Update();
  movingReader->Update();

  // 1. POINTSET CREATION
  SizeType     fixedSize = fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  unsigned int NumberOfXNodes = (fixedSize[0] - 2 * exploSize - 1) / step;
  unsigned int NumberOfYNodes = (fixedSize[1] - 2 * exploSize - 1) / step;
  unsigned int NumberOfNodes = NumberOfXNodes * NumberOfYNodes;

  std::cout << "PointSet size: " << NumberOfNodes << std::endl;

  IndexType firstNodeIndex;
  firstNodeIndex[0] = exploSize;
  firstNodeIndex[1] = exploSize;

  PointSetType::Pointer nodes = PointSetType::New();
  unsigned int          nodeCounter = 0;

  std::cout << "Node coordinates : " << std::endl;
  for (unsigned int x = 0; x < NumberOfXNodes; x++)
    for (unsigned int y = 0; y < NumberOfYNodes; y++)
      {
      PointType p;
      p[0] = firstNodeIndex[0] + x * step;     //  x coordinate
      p[1] = firstNodeIndex[1] + y * step;     //  y coordinate
      std::cout << "Id: " << nodeCounter << " ->  " <<  p << std::endl;
      nodes->SetPoint(nodeCounter++, p);
      }

  // Fix to avoid recomputing the disparity for each deformation field generation method.
  nodes->SetBufferedRegion(0);

  // 2. DISPARITY MAP ESTIMATION
  DMEstimationType::Pointer dmestimator = DMEstimationType::New();
  TransformType::Pointer    transform = TransformType::New();
  OptimizerType::Pointer    optimizer = OptimizerType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  MetricType::Pointer       metric = MetricType::New();
  metric->SetSubtractMean(true);
  optimizer->MinimizeOn();
  OptimizerType::ScalesType scales(transform->GetNumberOfParameters());
  scales[0] = 3.14;
  scales[1] = 0.0001;
  scales[2] = 0.0001;
  scales[3] = 10;
  scales[4] = 10;
  optimizer->SetScales(scales);

  // Set up
  dmestimator->SetTransform(transform);
  dmestimator->SetOptimizer(optimizer);
  dmestimator->SetInterpolator(interpolator);
  dmestimator->SetMetric(metric);

  // For gradient descent
  optimizer->SetLearningRate(learningRate);
  optimizer->SetNumberOfIterations(niterations);
  DMEstimationType::ParametersType initialParameters(transform->GetNumberOfParameters());
  initialParameters[0] = 0.0;  // Initial angle
  initialParameters[1] = centerx;  // x initial center
  initialParameters[2] = centery;  // y initial center
  initialParameters[3] = 0.0;  // Initial offset in mm along X
  initialParameters[4] = 0.0;  // Initial offset in mm along Y
  //Initial parameter set up
  dmestimator->SetInitialTransformParameters(initialParameters);

  // inputs wiring
  ImageType::SizeType win, explo;
  win.Fill(winSize);
  explo.Fill(exploSize);

  dmestimator->SetFixedImage(fixedReader->GetOutput());
  dmestimator->SetMovingImage(movingReader->GetOutput());
  dmestimator->SetPointSet(nodes);
  dmestimator->SetWinSize(win);
  dmestimator->SetExploSize(explo);
  dmestimator->SetInitialTransformParameters(initialParameters);

  // 3. DEFORMATION FIELDS COMPUTATION
  WriterType::Pointer                 writer = WriterType::New();
  DeformationFieldWriterType::Pointer dfwriter = DeformationFieldWriterType::New();
  ImageWarperType::Pointer            warper = ImageWarperType::New();
  RescalerType::Pointer               rescaler = RescalerType::New();
  std::stringstream                  oss;

  //3.a Nearest point deformation field generator
  NearestPointGeneratorType::Pointer generator1 = NearestPointGeneratorType::New();
  generator1->SetPointSet(dmestimator->GetOutput());
  generator1->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator1->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator1->SetOutputSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  generator1->SetMetricThreshold(metricThreshold);

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator1->GetOutput());
  rescaler->SetInput(warper->GetOutput());
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_np_df.hdr";
  dfwriter->SetFileName(oss.str().c_str());
  dfwriter->SetInput(generator1->GetOutput());
  dfwriter->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_np_oi.tif";
  writer->SetFileName(oss.str().c_str());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //3.b NNearest points deformation field linear interpolate  generator
  writer = WriterType::New();
  dfwriter = DeformationFieldWriterType::New();
  warper = ImageWarperType::New();
  rescaler = RescalerType::New();

  NNearestPointGeneratorType::Pointer generator2 = NNearestPointGeneratorType::New();
  generator2->SetPointSet(dmestimator->GetOutput());
  generator2->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator2->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator2->SetOutputSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  generator2->SetMetricThreshold(metricThreshold);
  generator2->SetNumberOfPoints(nbPointsToInterpolate);

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator2->GetOutput());
  rescaler->SetInput(warper->GetOutput());
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_nnp_df.hdr";
  dfwriter->SetFileName(oss.str().c_str());
  dfwriter->SetInput(generator2->GetOutput());
  dfwriter->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_nnp_oi.tif";
  writer->SetFileName(oss.str().c_str());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //3.c Splines points deformation field linear interpolate  generator
  writer = WriterType::New();
  dfwriter = DeformationFieldWriterType::New();
  warper = ImageWarperType::New();
  rescaler = RescalerType::New();

  BSplinesGeneratorType::Pointer generator3 = BSplinesGeneratorType::New();
  generator3->SetPointSet(dmestimator->GetOutput());
  generator3->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator3->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator3->SetOutputSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  generator3->SetMetricThreshold(metricThreshold);

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator3->GetOutput());
  rescaler->SetInput(warper->GetOutput());
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_bs_df.hdr";
  dfwriter->SetFileName(oss.str().c_str());
  dfwriter->SetInput(generator3->GetOutput());
  dfwriter->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_bs_oi.tif";
  writer->SetFileName(oss.str().c_str());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //3.d Nearest transform deformation field generator
  writer = WriterType::New();
  dfwriter = DeformationFieldWriterType::New();
  warper = ImageWarperType::New();
  rescaler = RescalerType::New();

  NearestTransformGeneratorType::Pointer generator4 = NearestTransformGeneratorType::New();
  generator4->SetPointSet(dmestimator->GetOutput());
  generator4->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator4->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator4->SetOutputSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  generator4->SetMetricThreshold(metricThreshold);
  generator4->SetTransform(transform);

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator4->GetOutput());
  rescaler->SetInput(warper->GetOutput());
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_nt_df.hdr";
  dfwriter->SetFileName(oss.str().c_str());
  dfwriter->SetInput(generator4->GetOutput());
  dfwriter->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_nt_oi.tif";
  writer->SetFileName(oss.str().c_str());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //3.e NNearest transforms deformation field linear interpolation generator
  writer = WriterType::New();
  dfwriter = DeformationFieldWriterType::New();
  warper = ImageWarperType::New();
  rescaler = RescalerType::New();

  NNearestTransformGeneratorType::Pointer generator5 = NNearestTransformGeneratorType::New();
  generator5->SetPointSet(dmestimator->GetOutput());
  generator5->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator5->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator5->SetOutputSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  generator5->SetMetricThreshold(metricThreshold);
  generator5->SetTransform(transform);
  generator5->SetNumberOfPoints(nbPointsToInterpolate);

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator5->GetOutput());
  rescaler->SetInput(warper->GetOutput());
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_nnt_df.hdr";
  dfwriter->SetFileName(oss.str().c_str());
  dfwriter->SetInput(generator5->GetOutput());
  dfwriter->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_nnt_oi.tif";
  writer->SetFileName(oss.str().c_str());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //3.e Transforms deformation field spline interpolation generator
  writer = WriterType::New();
  dfwriter = DeformationFieldWriterType::New();
  warper = ImageWarperType::New();
  rescaler = RescalerType::New();

  BSplinesTransformGeneratorType::Pointer generator6 = BSplinesTransformGeneratorType::New();
  generator6->SetPointSet(dmestimator->GetOutput());
  generator6->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator6->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator6->SetOutputSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  generator6->SetMetricThreshold(metricThreshold);
  generator6->SetTransform(transform);
  generator6->SetSplineOrder(4);
  generator6->SetNumberOfControlPoints(5);

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator6->GetOutput());
  rescaler->SetInput(warper->GetOutput());
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_bst_df.hdr";
  dfwriter->SetFileName(oss.str().c_str());
  dfwriter->SetInput(generator6->GetOutput());
  dfwriter->Update();

  oss.str("");
  oss << outputFileNamePrefix << "_bst_oi.tif";
  writer->SetFileName(oss.str().c_str());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
