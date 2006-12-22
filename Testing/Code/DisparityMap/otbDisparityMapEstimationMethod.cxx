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
try
  {
    const char* fixedFileName = argv[1];
    const char* movingFileName = argv[2];
    const char* pointSetFileName = argv[3];
    const char* outputFileName = argv[4];
    const unsigned int exploSize = atoi(argv[5]);
    const unsigned int winSize = 20;

    const unsigned int Dimension=2;
    typedef unsigned char PixelType;
    typedef otb::Image<PixelType,Dimension> ImageType;
    typedef otb::DisparityMapEstimationMethod<ImageType,ImageType> DMEstimationType;
    typedef itk::TranslationTransform<double,Dimension> TransformType;
    typedef itk::NormalizedCorrelationImageToImageMetric<ImageType,ImageType> MetricType;
    typedef itk::LinearInterpolateImageFunction<ImageType,double> InterpolatorType;
    typedef itk::GradientDescentOptimizer                         OptimizerType;
    typedef DMEstimationType::PointSetType PointSetType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    typedef otb::ThresholdImageToPointSetFilter<ImageType,PointSetType> PointSetSourceType;
    typedef PointSetType::PointsContainer::Iterator PointSetIteratorType;
    typedef PointSetType::PointDataContainer::Iterator PointDataIteratorType;
    
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
    std::cout<<"min: "<<mm->GetMinimum()<<" max: "<<mm->GetMaximum()<<std::endl;



    PointSetSourceType::Pointer pointSetSource = PointSetSourceType::New();
    pointSetSource->SetLowerThreshold(1);
    pointSetSource->SetUpperThreshold(256);
    pointSetSource->SetInput(0,pointSetReader->GetOutput());
    pointSetSource->Update();

    std::cout<<"PointSet size: "<<pointSetSource->GetOutput()->GetPoints()->Size()<<std::endl;
    
    // Instantiation
    DMEstimationType::Pointer dmestimator = DMEstimationType::New();
    TransformType::Pointer transform = TransformType::New();
    OptimizerType::Pointer optimizer = OptimizerType::New();
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    MetricType::Pointer metric = MetricType::New();

    // Set up
    dmestimator->SetTransform(transform);
    dmestimator->SetOptimizer(optimizer);
    dmestimator->SetInterpolator(interpolator);
    dmestimator->SetMetric(metric);

    // For gradient descent
    optimizer->SetLearningRate( 15.0 );
    optimizer->SetNumberOfIterations( 200 );
    DMEstimationType::ParametersType initialParameters(transform->GetNumberOfParameters() );
    initialParameters[0] = 0.0;  // Initial offset in mm along X
    initialParameters[1] = 0.0;  // Initial offset in mm along Y

    //Initial parameter set up
    // dmestimator->SetInitialTransformParameters(initialParameters);

    // inputs wiring
    dmestimator->SetFixedImage(fixedReader->GetOutput());
    dmestimator->SetMovingImage(movingReader->GetOutput());
    dmestimator->SetPointSet(pointSetSource->GetOutput());
    dmestimator->SetWinSize(winSize);
    dmestimator->SetExploSize(exploSize);
    // Estimation trigger
    dmestimator->Update();

    // Point set retrieving
    PointSetType::Pointer pointSet = dmestimator->GetPointSet();
    
    // Writing output transform parameters
    std::ofstream out;
    out.open(outputFileName,std::ios::out);
    PointSetIteratorType it = pointSet->GetPoints()->Begin();
  //   int idData=0;
   //  PointDataIteratorType itData = pointSet->GetPointData()->Begin();
    for(;it!=pointSet->GetPoints()->End()
// &&itData!=pointSet->GetPointData()->End()
;++it)
      {
	out<<"Point "<<it.Value()<<" -> transform parameters: ";
	out<<std::endl;
      }
    out.close();
  }
catch( itk::ExceptionObject & err ) 

  { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
catch( ... ) 
  { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
  } 
 return EXIT_SUCCESS;
}
