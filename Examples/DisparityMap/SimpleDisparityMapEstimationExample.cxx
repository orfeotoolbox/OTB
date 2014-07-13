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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {StereoFixed.png}, {StereoMoving.png}
//    OUTPUTS: {deformationFieldOutput.png}, {resampledMovingOutput.png}
//    5 5 9 7 10 100 0.8
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the \doxygen{otb}{DisparityMapEstimationMethod}, along with
// the \doxygen{otb}{NearestPointDisplacementFieldGenerator}. The first filter performs deformation estimation
// according to a given transform, using embedded ITK registration framework. It takes as input a possibly non regular
// point set and produces a point set with associated point data representing the deformation.
//
// The second filter generates a deformation field by using nearest neighbor interpolation on the deformation values from
// the point set. More advanced methods for deformation field interpolation are also available.
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbDisparityMapEstimationMethod.h"
#include "itkTranslationTransform.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkWindowedSincInterpolateImageFunction.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkGradientDescentOptimizer.h"
#include "otbBSplinesInterpolateDisplacementFieldGenerator.h"
#include "otbWarpImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

int main(int argc, char* argv[])
{
  if (argc != 12)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedFileName movingFileName fieldOutName imageOutName ";
    std::cerr <<
    "pointSetStepX pointSetStepY explorationSize windowSize learningRate ";
    std::cerr << "nbIterations metricThreshold";

    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;

  // Software Guide : BeginLatex
  //
  // Then we must decide what pixel type to use for the image. We choose to do
  // all the computation in floating point precision and rescale the results
  // between 0 and 255 in order to export PNG images.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double        PixelType;
  typedef unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the dimension. Please note that
  //  the \doxygen{otb}{NearestPointDisplacementFieldGenerator} generates a
  //  \doxygen{otb}{VectorImage} to represent the deformation field in both
  //  image directions.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The next step is to define the transform we have chosen to model the deformation. In this
  // example the deformation is modeled as a \doxygen{itk}{TranslationTransform}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::TranslationTransform<double, Dimension> TransformType;
  typedef TransformType::ParametersType                ParametersType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then we define the metric we will use to evaluate the local registration between the fixed and
  // the moving image. In this example we choosed the \doxygen{itk}{NormalizedCorrelationImageToImageMetric}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::NormalizedCorrelationImageToImageMetric<ImageType,
      ImageType> MetricType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Disparity map estimation implies evaluation of the moving image at non-grid position. Therefore, an
  // interpolator is needed. In this example we choosed the \doxygen{itk}{WindowedSincInterpolateImageFunction}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Function::HammingWindowFunction<3>          WindowFunctionType;
  typedef itk::ZeroFluxNeumannBoundaryCondition<ImageType> ConditionType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 3,
      WindowFunctionType,
      ConditionType,
      double> InterpolatorType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // To perform local registration, an optimizer is needed. In this example we choosed the
  // \doxygen{itk}{GradientDescentOptimizer}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::GradientDescentOptimizer OptimizerType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we will define the point set to represent the point where to compute local disparity.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::PointSet<ParametersType, Dimension> PointSetType;
  // Software Guide : EndCodeSnippet

  typedef PointSetType::PointType                    PointType;

  // Software Guide : BeginLatex
  //
  // Now we define the disparity map estimation filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::DisparityMapEstimationMethod<ImageType,
      ImageType,
      PointSetType> DMEstimationType;
  typedef DMEstimationType::SizeType SizeType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The input image reader also has to be defined.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Two readers are instantiated : one for the fixed image, and one for the moving image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer fixedReader = ReaderType::New();
  ReaderType::Pointer movingReader = ReaderType::New();

  fixedReader->SetFileName(argv[1]);
  movingReader->SetFileName(argv[2]);
  fixedReader->UpdateOutputInformation();
  movingReader->UpdateOutputInformation();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We will the create a regular point set where to compute the local disparity.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  SizeType fixedSize =
    fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  unsigned int NumberOfXNodes = (fixedSize[0] - 2 * atoi(argv[7]) - 1)
                                / atoi(argv[5]);
  unsigned int NumberOfYNodes = (fixedSize[1] - 2 * atoi(argv[7]) - 1)
                                / atoi(argv[6]);

  ImageType::IndexType firstNodeIndex;
  firstNodeIndex[0] = atoi(argv[7]);
  firstNodeIndex[1] = atoi(argv[7]);

  PointSetType::Pointer nodes = PointSetType::New();
  unsigned int          nodeCounter = 0;

  for (unsigned int x = 0; x < NumberOfXNodes; x++)
    {
    for (unsigned int y = 0; y < NumberOfYNodes; y++)
      {
      PointType                    p;
      p[0] = firstNodeIndex[0] + x*atoi(argv[5]);
      p[1] = firstNodeIndex[1] + y*atoi(argv[6]);
      nodes->SetPoint(nodeCounter++, p);
      }
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We build the transform, interpolator, metric and optimizer for the disparity map
  // estimation filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  TransformType::Pointer transform = TransformType::New();

  OptimizerType::Pointer optimizer = OptimizerType::New();
  optimizer->MinimizeOn();
  optimizer->SetLearningRate(atof(argv[9]));
  optimizer->SetNumberOfIterations(atoi(argv[10]));

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  MetricType::Pointer metric = MetricType::New();
  metric->SetSubtractMean(true);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We then set up the disparity map estimation filter. This filter will perform a local registration at each
  // point of the given point set using the ITK registration framework. It will produce a point set whose point data
  // reflects the disparity locally around the associated point.
  //
  // Point data will contains the following data :
  //
  // \begin{enumerate}
  // \item{The final metric value found in the registration process, }
  // \item{the deformation value in the first image direction, }
  // \item{the deformation value in the second image direction, }
  // \item{the final parameters of the transform.}
  // \end{enumerate}
  //
  // Please note that in the case of a \doxygen{itk}{TranslationTransform}, the deformation values and
  // the transform parameters are the same.
  //
  // Software Guide : EndLatex

  // Software Guide :BeginCodeSnippet

  DMEstimationType::Pointer dmestimator = DMEstimationType::New();

  dmestimator->SetTransform(transform);
  dmestimator->SetOptimizer(optimizer);
  dmestimator->SetInterpolator(interpolator);
  dmestimator->SetMetric(metric);

  SizeType windowSize, explorationSize;
  explorationSize.Fill(atoi(argv[7]));
  windowSize.Fill(atoi(argv[8]));

  dmestimator->SetWinSize(windowSize);
  dmestimator->SetExploSize(explorationSize);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The initial transform parameters can be set via the \code{SetInitialTransformParameters()} method.
  // In our case, we simply fill the parameter array with null values.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  DMEstimationType::ParametersType
  initialParameters(transform->GetNumberOfParameters());
  initialParameters[0] = 0.0;
  initialParameters[1] = 0.0;
  dmestimator->SetInitialTransformParameters(initialParameters);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we can set the input for the deformation field estimation filter. Fixed image can be set using the \code{SetFixedImage()}
  // method, moving image can be set using the \code{SetMovingImage()}, and input point set can be set using the
  // \code{SetPointSet()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  dmestimator->SetFixedImage(fixedReader->GetOutput());
  dmestimator->SetMovingImage(movingReader->GetOutput());
  dmestimator->SetPointSet(nodes);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Once the estimation has been performed by the \doxygen{otb}{DisparityMapEstimationMethod}, one can generate
  // the associated deformation field (that means translation in first and second image direction).
  // It will be represented as a \doxygen{otb}{VectorImage}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType, Dimension> DisplacementFieldType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // For the deformation field estimation, we will use the \doxygen{otb}{BSplinesInterpolateDisplacementFieldGenerator}.
  // This filter will perform a nearest neighbor interpolation on the deformation values in the point set data.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::BSplinesInterpolateDisplacementFieldGenerator<PointSetType,
      DisplacementFieldType>
  GeneratorType;

  // Software GUide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The disparity map estimation filter is instanciated.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  GeneratorType::Pointer generator = GeneratorType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We must then specify the input point set using the \code{SetPointSet()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  generator->SetPointSet(dmestimator->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // One must also specify the origin, size and spacing of the output deformation field.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  generator->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator->SetOutputSize(fixedReader->GetOutput()
                           ->GetLargestPossibleRegion().GetSize());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The local registration process can lead to wrong deformation values and transform parameters. To Select only
  // points in point set for which the registration process was succesful, one can set a threshold on the final metric
  // value : points for which the absolute final metric value is below this threshold will be discarded. This
  // threshold can be set with the \code{SetMetricThreshold()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  generator->SetMetricThreshold(atof(argv[11]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // \relatedClasses
  // \begin{itemize}
  // \item \doxygen{otb}{NNearestPointsLinearInterpolateDisplacementFieldGenerator}
  // \item \doxygen{otb}{BSplinesInterpolateDisplacementFieldGenerator}
  // \item \doxygen{otb}{NearestTransformDisplacementFieldGenerator}
  // \item \doxygen{otb}{NNearestTransformsLinearInterpolateDisplacementFieldGenerator}
  // \item \doxygen{otb}{BSplinesInterpolateTransformDisplacementFieldGenerator}
  // \end{itemize}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginLatex
  //
  // Now we can warp our fixed image according to the estimated deformation field. This will be performed by the
  // \doxygen{itk}{WarpImageFilter}. First, we define this filter.
  //
  // Software Guide : EndLatex

  // Software Guide :  BeginCodeSnippet

  typedef otb::WarpImageFilter<ImageType, ImageType,
      DisplacementFieldType> ImageWarperType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then we instantiate it.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageWarperType::Pointer warper = ImageWarperType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide  : BeginLatex
  //
  // We set the input image to warp using the \code{SetInput()} method, and the deformation field
  // using the \code{SetDisplacementField()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  warper->SetInput(movingReader->GetOutput());
  warper->SetDisplacementField(generator->GetOutput());
  warper->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  warper->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // In order to write the result to a PNG file, we will rescale it on a proper range.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputImageType> RescalerType;

  RescalerType::Pointer outputRescaler = RescalerType::New();
  outputRescaler->SetInput(warper->GetOutput());
  outputRescaler->SetOutputMaximum(255);
  outputRescaler->SetOutputMinimum(0);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now write the image to a file. The filters are executed by invoking
  // the \code{Update()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer outputWriter = WriterType::New();
  outputWriter->SetInput(outputRescaler->GetOutput());
  outputWriter->SetFileName(argv[4]);
  outputWriter->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We also want to write the deformation field along the first direction to a file.
  // To achieve this we will use the \doxygen{otb}{MultiToMonoChannelExtractROI} filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MultiToMonoChannelExtractROI<PixelType,
      PixelType>
  ChannelExtractionFilterType;

  ChannelExtractionFilterType::Pointer channelExtractor
    = ChannelExtractionFilterType::New();

  channelExtractor->SetInput(generator->GetOutput());
  channelExtractor->SetChannel(1);

  RescalerType::Pointer fieldRescaler = RescalerType::New();
  fieldRescaler->SetInput(channelExtractor->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);

  WriterType::Pointer fieldWriter = WriterType::New();
  fieldWriter->SetInput(fieldRescaler->GetOutput());
  fieldWriter->SetFileName(argv[3]);
  fieldWriter->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:SIMPLEDISPARITYMAPESTIMATIONOUTPUT} shows the result of applying disparity map estimation on a stereo pair
  // using a regular point set, followed by deformation field estimation using Splines and fixed image resampling.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.20\textwidth]{StereoFixed.eps}
  // \includegraphics[width=0.20\textwidth]{StereoMoving.eps}
  // \includegraphics[width=0.20\textwidth]{deformationFieldOutput.eps}
  // \includegraphics[width=0.20\textwidth]{resampledMovingOutput.eps}
  // \itkcaption[Displacement field and resampling from disparity map estimation]{From left
  // to right and top to bottom: fixed input image, moving image with a sinusoid deformation,
  // estimated deformation field in the horizontal direction, resampled moving image.}
  // \label{fig:SIMPLEDISPARITYMAPESTIMATIONOUTPUT}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
