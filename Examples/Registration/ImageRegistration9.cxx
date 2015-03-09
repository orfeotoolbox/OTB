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
//    INPUTS: {QB_Suburb.png}
//    INPUTS: {QB_SuburbR10X13Y17.png}
//    OUTPUTS: {ImageRegistration9Output.png}
//    OUTPUTS: {ImageRegistration9DifferenceBefore.png}
//    OUTPUTS: {ImageRegistration9DifferenceAfter.png}
//    1.0 300
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{itk}{AffineTransform}
// for performing registration. The example code is, for the most part,
// identical to previous ones.
// The main difference is the use of the AffineTransform here instead of the
// \doxygen{itk}{CenteredRigid2DTransform}. We will focus on the most
// relevant changes in the current code and skip the basic elements already
// explained in previous examples.
//
// \index{itk::AffineTransform}
//
// Software Guide : EndLatex

#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "otbImage.h"

#include "itkCenteredTransformInitializer.h"

//  Software Guide : BeginLatex
//
//  Let's start by including the header file of the AffineTransform.
//
//  \index{itk::AffineTransform!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkAffineTransform.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkMeanImageFilter.h"

//
//  The following piece of code implements an observer
//  that will monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command
{
public:
  typedef  CommandIterationUpdate Self;
  typedef  itk::Command           Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  itkNewMacro(Self);
protected:
  CommandIterationUpdate() {}
public:
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
  typedef   const OptimizerType *                  OptimizerPointer;

  void Execute(itk::Object *caller, const itk::EventObject& event)
  {
    Execute((const itk::Object *) caller, event);
  }

  void Execute(const itk::Object * object, const itk::EventObject& event)
  {
    OptimizerPointer optimizer =
      dynamic_cast<OptimizerPointer>(object);
    if (!itk::IterationEvent().CheckEvent(&event))
      {
      return;
      }
    std::cout << optimizer->GetCurrentIteration() << "   ";
    std::cout << optimizer->GetValue() << "   ";
    std::cout << optimizer->GetCurrentPosition();

    // Print the angle for the trace plot
    vnl_matrix<double> p(2, 2);
    p[0][0] = (double) optimizer->GetCurrentPosition()[0];
    p[0][1] = (double) optimizer->GetCurrentPosition()[1];
    p[1][0] = (double) optimizer->GetCurrentPosition()[2];
    p[1][1] = (double) optimizer->GetCurrentPosition()[3];
    vnl_svd<double>    svd(p);
    vnl_matrix<double> r(2, 2);
    r = svd.U() * vnl_transpose(svd.V());
    double angle = asin(r[1][0]);
    std::cout << " AffineAngle: " << angle * 45.0 / atan(1.0) << std::endl;
  }
};

int main(int argc, char *argv[])
{
  if (argc < 4)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << "   fixedImageFile  movingImageFile " << std::endl;
    std::cerr << "   outputImagefile  [differenceBeforeRegistration] " <<
    std::endl;
    std::cerr << "   [differenceAfterRegistration] " << std::endl;
    std::cerr << "   [stepLength] [maxNumberOfIterations] " << std::endl;
    return 1;
    }

  //  Software Guide : BeginLatex
  //
  //  We define then the types of the images to be registered.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef  float PixelType;

  typedef otb::Image<PixelType, Dimension> FixedImageType;
  typedef otb::Image<PixelType, Dimension> MovingImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The transform type is instantiated using the code below. The template
  //  parameters of this class are the representation type of the space
  //  coordinates and the space dimension.
  //
  //  \index{itk::AffineTransform!Instantiation}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::AffineTransform<
      double,
      Dimension>     TransformType;
  // Software Guide : EndCodeSnippet

  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
  typedef itk::MeanSquaresImageToImageMetric<
      FixedImageType,
      MovingImageType>    MetricType;
  typedef itk::LinearInterpolateImageFunction<
      MovingImageType,
      double>    InterpolatorType;
  typedef itk::ImageRegistrationMethod<
      FixedImageType,
      MovingImageType>    RegistrationType;

  MetricType::Pointer       metric        = MetricType::New();
  OptimizerType::Pointer    optimizer     = OptimizerType::New();
  InterpolatorType::Pointer interpolator  = InterpolatorType::New();
  RegistrationType::Pointer registration  = RegistrationType::New();

  registration->SetMetric(metric);
  registration->SetOptimizer(optimizer);
  registration->SetInterpolator(interpolator);

  //  Software Guide : BeginLatex
  //
  //  The transform object is constructed below and passed to the registration
  //  method.
  //
  //  \index{itk::AffineTransform!New()}
  //  \index{itk::AffineTransform!Pointer}
  //  \index{itk::RegistrationMethod!SetTransform()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  TransformType::Pointer transform = TransformType::New();
  registration->SetTransform(transform);
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<FixedImageType>  FixedImageReaderType;
  typedef otb::ImageFileReader<MovingImageType> MovingImageReaderType;
  FixedImageReaderType::Pointer  fixedImageReader  = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
  fixedImageReader->SetFileName(argv[1]);
  movingImageReader->SetFileName(argv[2]);

  //  Software Guide : BeginLatex
  //
  //  Since we are working with high resolution images and expected
  //  shifts are larger than the resolution, we will need to smooth
  //  the images in order to avoid the optimizer to get stucked on
  //  local minima. In order to do this, we will use a simple mean filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::MeanImageFilter<
      FixedImageType, FixedImageType>  FixedFilterType;

  typedef itk::MeanImageFilter<
      MovingImageType, MovingImageType>  MovingFilterType;

  FixedFilterType::Pointer  fixedFilter  = FixedFilterType::New();
  MovingFilterType::Pointer movingFilter = MovingFilterType::New();

  FixedImageType::SizeType indexFRadius;

  indexFRadius[0] = 4; // radius along x
  indexFRadius[1] = 4; // radius along y

  fixedFilter->SetRadius(indexFRadius);

  MovingImageType::SizeType indexMRadius;

  indexMRadius[0] = 4; // radius along x
  indexMRadius[1] = 4; // radius along y

  movingFilter->SetRadius(indexMRadius);

  fixedFilter->SetInput(fixedImageReader->GetOutput());
  movingFilter->SetInput(movingImageReader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Now we can plug the output of the smoothing filters at the input
  //  of the registration method.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  registration->SetFixedImage(fixedFilter->GetOutput());
  registration->SetMovingImage(movingFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  fixedImageReader->Update();

  registration->SetFixedImageRegion(
    fixedImageReader->GetOutput()->GetBufferedRegion());

  //  Software Guide : BeginLatex
  //
  //  In this example, we use the
  //  \doxygen{itk}{CenteredTransformInitializer} helper class in order to compute
  //  a reasonable value for the initial center of rotation and the
  //  translation. The initializer is set to use the center of mass of each
  //  image as the initial correspondence correction.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::CenteredTransformInitializer<
      TransformType,
      FixedImageType,
      MovingImageType>  TransformInitializerType;
  TransformInitializerType::Pointer initializer = TransformInitializerType::New();
  initializer->SetTransform(transform);
  initializer->SetFixedImage(fixedImageReader->GetOutput());
  initializer->SetMovingImage(movingImageReader->GetOutput());
  initializer->MomentsOn();
  initializer->InitializeTransform();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Now we pass the parameters of the current transform as the initial
  //  parameters to be used when the registration process starts.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  registration->SetInitialTransformParameters(
    transform->GetParameters());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Keeping in mind that the scale of units in scaling, rotation and
  //  translation are quite different, we take advantage of the scaling
  //  functionality provided by the optimizers. We know that the first $N
  //  \times N$ elements of the parameters array correspond to the rotation
  //  matrix factor, the next $N$ correspond to the rotation center, and the
  //  last $N$ are the components of the translation to be applied after
  //  multiplication with the matrix is performed.
  //
  //  Software Guide : EndLatex

  double translationScale = 1.0 / 1000.0;
  if (argc > 8)
    {
    translationScale = atof(argv[8]);
    }

  // Software Guide : BeginCodeSnippet
  typedef OptimizerType::ScalesType OptimizerScalesType;
  OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());

  optimizerScales[0] =  1.0;
  optimizerScales[1] =  1.0;
  optimizerScales[2] =  1.0;
  optimizerScales[3] =  1.0;
  optimizerScales[4] =  translationScale;
  optimizerScales[5] =  translationScale;

  optimizer->SetScales(optimizerScales);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We also set the usual parameters of the optimization method. In this
  //  case we are using an
  //  \doxygen{itk}{RegularStepGradientDescentOptimizer}. Below, we define the
  //  optimization parameters like initial step length, minimal step length
  //  and number of iterations. These last two act as stopping criteria for
  //  the optimization.
  //
  //  Software Guide : EndLatex

  double steplength = 0.1;

  if (argc > 6)
    {
    steplength = atof(argv[6]);
    }

  unsigned int maxNumberOfIterations = 300;

  if (argc > 7)
    {
    maxNumberOfIterations = atoi(argv[7]);
    }

  // Software Guide : BeginCodeSnippet
  optimizer->SetMaximumStepLength(steplength);
  optimizer->SetMinimumStepLength(0.0001);
  optimizer->SetNumberOfIterations(maxNumberOfIterations);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We also set the optimizer to do minimization by calling the
  //  \code{MinimizeOn()} method.
  //
  //  \index{itk::Regular\-Step\-Gradient\-Descent\-Optimizer!MinimizeOn()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  optimizer->MinimizeOn();
  // Software Guide : EndCodeSnippet

  // Create the Command observer and register it with the optimizer.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver(itk::IterationEvent(), observer);

  //  Software Guide : BeginLatex
  //
  //  Finally we trigger the execution of the registration method by calling
  //  the \code{Update()} method. The call is placed in a \code{try/catch}
  //  block in case any exceptions are thrown.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  try
    {
    registration->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return -1;
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Once the optimization converges, we recover the parameters from the
  //  registration method. This is done with the
  //  \code{GetLastTransformParameters()} method. We can also recover the
  //  final value of the metric with the \code{GetValue()} method and the
  //  final number of iterations with the \code{GetCurrentIteration()}
  //  method.
  //
  //  \index{itk::RegistrationMethod!GetValue()}
  //  \index{itk::RegistrationMethod!GetCurrentIteration()}
  //  \index{itk::RegistrationMethod!GetLastTransformParameters()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  OptimizerType::ParametersType finalParameters =
    registration->GetLastTransformParameters();

  const double finalRotationCenterX = transform->GetCenter()[0];
  const double finalRotationCenterY = transform->GetCenter()[1];
  const double finalTranslationX    = finalParameters[4];
  const double finalTranslationY    = finalParameters[5];

  const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
  const double       bestValue = optimizer->GetValue();
  // Software Guide : EndCodeSnippet

  // Print out results
  //
  std::cout << "Result = " << std::endl;
  std::cout << " Center X      = " << finalRotationCenterX  << std::endl;
  std::cout << " Center Y      = " << finalRotationCenterY  << std::endl;
  std::cout << " Translation X = " << finalTranslationX  << std::endl;
  std::cout << " Translation Y = " << finalTranslationY  << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue          << std::endl;

  //Compute the rotation angle and scaling from SVD of the matrix
  // \todo Find a way to figure out if the scales are along X or along Y.
  // VNL returns the eigenvalues ordered from largest to smallest.

  vnl_matrix<double> p(2, 2);
  p[0][0] = (double) finalParameters[0];
  p[0][1] = (double) finalParameters[1];
  p[1][0] = (double) finalParameters[2];
  p[1][1] = (double) finalParameters[3];
  vnl_svd<double>    svd(p);
  vnl_matrix<double> r(2, 2);
  r = svd.U() * vnl_transpose(svd.V());
  double angle = asin(r[1][0]);

  std::cout << " Scale 1         = " << svd.W(0)                 << std::endl;
  std::cout << " Scale 2         = " << svd.W(1)                 << std::endl;
  std::cout << " Angle (degrees) = " << angle * 45.0 / atan(1.0) << std::endl;

  //  Software Guide : BeginLatex
  //
  //  Let's execute this example over two of the images provided in
  //  \code{Examples/Data}:
  //
  //  \begin{itemize}
  //  \item \code{QB\_Suburb.png}
  //  \item \code{QB\_SuburbR10X13Y17.png}
  //  \end{itemize}
  //
  //  The second image is the result of intentionally rotating the first
  //  image by $10$ degrees and then translating by $(13, 17)$.  Both images
  //  have unit-spacing and are shown in Figure
  //  \ref{fig:FixedMovingImageRegistration9}. We execute the code using the
  //  following parameters: step length=1.0, translation scale= 0.0001 and
  //  maximum number of iterations = 300. With these images and parameters
  //  the registration takes $83$ iterations and produces
  //
  //  \begin{center}
  //  \begin{verbatim}
  //   20.2134   [0.983291, -0.173507, 0.174626, 0.983028, -12.1899, -16.0882]
  //  \end{verbatim}
  //  \end{center}
  //
  //  These results are interpreted as
  //
  //  \begin{itemize}
  //  \item Iterations   = 83
  //  \item Final Metric = 20.2134
  //  \item Center       = $( 134.152, 104.067   )$ pixels
  //  \item Translation  = $(   -12.1899, -16.0882 )$ pixels
  //  \item Affine scales = $(0.999024, 0.997875)$
  //  \end{itemize}
  //
  //  The second component of the matrix values is usually associated with
  //  $\sin{\theta}$. We obtain the rotation through SVD of the affine
  //  matrix. The value is $10.0401$ degrees, which is approximately the
  //  intentional misalignment of $10.0$ degrees.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{QB_SuburbR10X13Y17.eps}
  // \itkcaption[AffineTransform registration]{Fixed and moving images
  // provided as input to the registration method using the AffineTransform.}
  // \label{fig:FixedMovingImageRegistration9}
  // \end{figure}
  //
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{ImageRegistration9Output.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration9DifferenceBefore.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration9DifferenceAfter.eps}
  // \itkcaption[AffineTransform output images]{The resampled moving image
  // (left), and the difference between the fixed and moving images before (center)
  // and after (right) registration with the
  // AffineTransform transform.}
  // \label{fig:ImageRegistration9Outputs}
  // \end{figure}
  //
  // Figure \ref{fig:ImageRegistration9Outputs} shows the output of the
  // registration. The right most image of this figure shows the squared
  // magnitude difference between the fixed image and the resampled
  // moving image.
  //  Software Guide : EndLatex

  //  The following code is used to dump output images to files.
  //  They illustrate the final results of the registration.
  //  We will resample the moving image and write out the difference image
  //  before and after registration. We will also rescale the intensities of the
  //  difference images, so that they look better!
  typedef itk::ResampleImageFilter<
      MovingImageType,
      FixedImageType>    ResampleFilterType;

  TransformType::Pointer finalTransform = TransformType::New();

  finalTransform->SetCenter(transform->GetCenter());
  finalTransform->SetParameters(finalParameters);

  ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  resampler->SetTransform(finalTransform);
  resampler->SetInput(movingImageReader->GetOutput());

  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
  resampler->SetOutputOrigin(fixedImage->GetOrigin());
  resampler->SetOutputSpacing(fixedImage->GetSpacing());
  resampler->SetDefaultPixelValue(100);

  typedef  unsigned char OutputPixelType;

  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef itk::CastImageFilter<
      FixedImageType,
      OutputImageType> CastFilterType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer     writer =  WriterType::New();
  CastFilterType::Pointer caster =  CastFilterType::New();

  writer->SetFileName(argv[3]);

  caster->SetInput(resampler->GetOutput());
  writer->SetInput(caster->GetOutput());
  writer->Update();

  typedef itk::SubtractImageFilter<
      FixedImageType,
      FixedImageType,
      FixedImageType> DifferenceFilterType;

  DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

  difference->SetInput1(fixedImageReader->GetOutput());
  difference->SetInput2(resampler->GetOutput());

  WriterType::Pointer writer2 = WriterType::New();

  typedef itk::RescaleIntensityImageFilter<
      FixedImageType,
      OutputImageType>   RescalerType;

  RescalerType::Pointer intensityRescaler = RescalerType::New();

  intensityRescaler->SetInput(difference->GetOutput());
  intensityRescaler->SetOutputMinimum(0);
  intensityRescaler->SetOutputMaximum(255);

  writer2->SetInput(intensityRescaler->GetOutput());
  resampler->SetDefaultPixelValue(1);

  // Compute the difference image between the
  // fixed and resampled moving image.
  if (argc > 5)
    {
    writer2->SetFileName(argv[5]);
    writer2->Update();
    }

  typedef itk::IdentityTransform<double, Dimension> IdentityTransformType;
  IdentityTransformType::Pointer identity = IdentityTransformType::New();

  // Compute the difference image between the
  // fixed and moving image before registration.
  if (argc > 4)
    {
    resampler->SetTransform(identity);
    writer2->SetFileName(argv[4]);
    writer2->Update();
    }

  return EXIT_SUCCESS;
}
